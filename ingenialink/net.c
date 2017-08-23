/*
 * MIT License
 *
 * Copyright (c) 2017 Ingenia-CAT S.L.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "net.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>

#if defined(__unix__)
#  include <unistd.h>
#elif defined(_WIN32)
#  include <Windows.h>
#else
#  error Unsupported platform
#endif

#include "ingenialink/err.h"
#include "ingenialink/frame.h"

/*******************************************************************************
 * Private
 ******************************************************************************/

/**
 * Monitor event callback.
 */
void on_event(void *ctx, ser_dev_evt_t evt, const ser_dev_t *dev)
{
	il_net_dev_mon_t *mon = ctx;

	if (evt == SER_DEV_EVT_ADDED) {
		/* FIX: some devices will not respond immediately... a dirty
		 * solution is to wait before notifying
		 */
#if defined(__unix__)
		usleep(MONITOR_WAIT_TIME * 1000);
#elif defined(_WIN32)
		Sleep(MONITOR_WAIT_TIME);
#endif

		mon->on_added(mon->ctx, dev->path);
	}
}

/*******************************************************************************
 * Internal
 ******************************************************************************/

int il_net__send(il_net_t *net, uint8_t id, uint16_t idx, uint8_t sidx,
		 const void *buf, size_t sz)
{
	int r;
	il_frame_t frame;

	il_frame__init(&frame, id, idx, sidx, buf, sz);

	r = ser_write(net->ser, frame.buf, frame.sz, NULL);
	if (r < 0)
		return ilerr__ser(r);

	return 0;
}

int il_net__recv(il_net_t *net, uint8_t id, uint16_t idx, uint8_t sidx,
		 void *buf, size_t sz, size_t *recvd)
{
	int done = 0;
	il_frame_t frame = IL_FRAME_INIT_DEF;

	while (!done) {
		int r;
		uint8_t c;

		/* read next byte */
		r = ser_read(net->ser, &c, sizeof(c), NULL);
		if (r == SER_EEMPTY) {
			r = ser_read_wait(net->ser);
			if (r < 0)
				return ilerr__ser(r);
			continue;
		} else if (r < 0) {
			return ilerr__ser(r);
		}

		/* push to the frame (and update its state) */
		r = il_frame__push(&frame, c);
		if (r < 0) {
			/* likely garbage (reset to retry, will eventually
			 * timeout)
			 */
			il_frame__reset(&frame);
			continue;
		}

		/* validate */
		if (frame.state == IL_FRAME_STATE_COMPLETE) {
			uint8_t id_ = il_frame__get_id(&frame);
			uint16_t idx_ = il_frame__get_idx(&frame);
			uint8_t sidx_ = il_frame__get_sidx(&frame);
			size_t sz_ = il_frame__get_sz(&frame);
			void *data = il_frame__get_data(&frame);

			int resp = il_frame__is_resp(&frame);

			/* skip any async EMCY frame or daisy chain */
			if (((id != id_) && (id != 0)) || (idx != idx_) ||
			    (sidx != sidx_) || (!resp)) {
				il_frame__reset(&frame);
				continue;
			}

			if (sz < sz_) {
				ilerr__set("Buffer too small");
				return IL_ENOMEM;
			}

			memcpy(buf, data, sz_);

			if (recvd)
				*recvd = sz_;

			done = 1;
		}
	}

	return 0;
}

/*******************************************************************************
 * Public
 ******************************************************************************/

il_net_t *il_net_create(const char *port, unsigned int timeout)
{
	il_net_t *net;
	ser_opts_t sopts = SER_OPTS_INIT;
	int r;
	uint8_t val;

	/* validate options */
	if (!port) {
		ilerr__set("Invalid port (NULL)");
		return NULL;
	}

	if (!timeout) {
		ilerr__set("Invalid timeout (zero)");
		return NULL;
	}

	/* allocate net */
	net = malloc(sizeof(*net));
	if (!net) {
		ilerr__set("Could not allocate network (%s)", strerror(errno));
		return NULL;
	}

	/* allocate serial port */
	net->ser = ser_create();
	if (!net->ser) {
		ilerr__set("Could not create serial port(%s)", sererr_last());
		goto cleanup_net;
	}

	/* open serial port */
	sopts.port = port;
	sopts.baudrate = BAUDRATE_DEF;
	sopts.timeouts.rd = timeout;
	sopts.timeouts.wr = TIMEOUT_WR_DEF;

	r = ser_open(net->ser, &sopts);
	if (r < 0) {
		ilerr__set("Could not open port (%s)", sererr_last());
		goto cleanup_ser;
	}

	/* send ascii message to force binary */
	r = ser_write(net->ser, MSG_A2B, sizeof(MSG_A2B) - 1, NULL);
	if (r < 0) {
		ilerr__set("Could not boadcast ascii to binary message (%s)",
			   sererr_last());
		goto close_ser;
	}

	/* send the same message in binary (will flush if already on binary) */
	val = 1;
	(void)il_net__send(net, 0, UARTCFG_BIN_IDX, UARTCFG_BIN_SIDX, &val,
			   sizeof(val));

	/* flush serial queues */
	(void)ser_flush(net->ser, SER_QUEUE_ALL);

	return net;

close_ser:
	ser_close(net->ser);

cleanup_ser:
	ser_destroy(net->ser);

cleanup_net:
	free(net);

	return NULL;
}

void il_net_destroy(il_net_t *net)
{
	/* validate net */
	if (!net)
		return;

	/* free resources */
	ser_close(net->ser);

	ser_destroy(net->ser);
	free(net);
}

il_net_dev_list_t *il_net_dev_list_get()
{
	il_net_dev_list_t *lst = NULL;
	il_net_dev_list_t *prev;

	ser_dev_list_t *ser_devs;
	ser_dev_list_t *ser_dev;

	/* obtain all serial ports */
	ser_devs = ser_dev_list_get();
	if (!ser_devs)
		return NULL;

	/* create the device list */
	ser_dev_list_foreach(ser_dev, ser_devs) {
		/* allocate new list entry */
		prev = lst;
		lst = malloc(sizeof(*lst));
		if (!lst) {
			il_net_dev_list_destroy(prev);
			break;
		}

		lst->next = prev;

		/* store port */
		strncpy(lst->port, ser_dev->dev.path, sizeof(lst->port));
	}

	ser_dev_list_destroy(ser_devs);

	return lst;
}

void il_net_dev_list_destroy(il_net_dev_list_t *lst)
{
	il_net_dev_list_t *curr;

	curr = lst;
	while (curr) {
		il_net_dev_list_t *tmp;

		tmp = curr->next;
		free(curr);
		curr = tmp;
	}
}

il_net_dev_mon_t *il_net_dev_mon_create(il_net_dev_on_added_t on_added,
					void *ctx)
{
	il_net_dev_mon_t *mon;

	/* validate arguments */
	if (!on_added) {
		ilerr__set("Invalid callback (NULL)");
		return NULL;
	}

	/* allocate monitor */
	mon = malloc(sizeof(*mon));
	if (!mon) {
		ilerr__set("Could not allocate monitor (%s)", strerror(errno));
		return NULL;
	}

	/* store context and bring up monitor */
	mon->ctx = ctx;
	mon->on_added = on_added;
	mon->mon = ser_dev_monitor_init(on_event, mon);
	if (!mon->mon) {
		ilerr__set("Could not initialize monitor (%s)", sererr_last());
		goto cleanup_mon;
	}

	return mon;

cleanup_mon:
	free(mon);

	return NULL;
}

void il_net_dev_mon_destroy(il_net_dev_mon_t *mon)
{
	/* validate monitor */
	if (!mon)
		return;

	ser_dev_monitor_stop(mon->mon);
	free(mon);
}

il_net_nodes_list_t *il_net_nodes_list_get(il_net_t *net,
					   il_net_nodes_on_found_t on_found,
					   void *ctx)
{
	int r;

	il_net_nodes_list_t *lst = NULL;
	il_net_nodes_list_t *prev;

	/* validate network */
	if (!net) {
		ilerr__set("Invalid network (NULL)");
		return NULL;
	}

	/* broadcast "read node id" message */
	r = il_net__send(net, 0, UARTCFG_ID_IDX, UARTCFG_ID_SIDX, NULL, 0);
	if (r < 0)
		return NULL;

	/* read any response until error (likely timeout) */
	while (r == 0) {
		uint8_t id;
		size_t recvd = 0;

		r = il_net__recv(net, 0, UARTCFG_ID_IDX, UARTCFG_ID_SIDX, &id,
				 sizeof(id), &recvd);
		if ((r == 0) && (recvd > 0)) {
			/* allocate new list entry */
			prev = lst;
			lst = malloc(sizeof(*lst));
			if (!lst) {
				il_net_nodes_list_destroy(prev);
				break;
			}

			lst->next = prev;
			lst->id = id;

			if (on_found)
				on_found(ctx, id);
		}
	}

	return lst;
}

void il_net_nodes_list_destroy(il_net_nodes_list_t *lst)
{
	il_net_nodes_list_t *curr;

	curr = lst;
	while (curr) {
		il_net_nodes_list_t *tmp;

		tmp = curr->next;
		free(curr);
		curr = tmp;
	}
}