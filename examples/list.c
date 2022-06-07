/**
 * @example list.c
 *
 * This example scans for all availe servo on every network device.
 */

#include "utils.h"

#include <stdio.h>

#include "external/log.c/src/log.h"

int main(int argc, const char *argv[])
{
	il_net_prot_t prot;
	il_net_dev_list_t *devs, *dev;
	il_net_servos_list_t *servo_ids, *servo_id;

	if (argc < 2) {
		log_error("Usage: ./list PROT");
		return -1;
	}

	prot = str2prot(argv[1]);

	devs = il_net_dev_list_get(prot);
	il_net_dev_list_foreach(dev, devs) {
		il_net_t *net;
		il_net_opts_t opts;

		/* create network */
		opts.port = dev->port;
		opts.timeout_rd = IL_NET_TIMEOUT_RD_DEF;
		opts.timeout_wr = IL_NET_TIMEOUT_WR_DEF;

		/*net = il_net_eusb_create(&opts);*/
		net = il_net_create(prot, &opts);
		if (!net)
			continue;

		/* scan (use callback to print on the go) */
		log_info("Scanning %s...", dev->port);

		servo_ids = il_net_servos_list_get(net, NULL, NULL);
		il_net_servos_list_foreach(servo_id, servo_ids) {
			il_servo_t *servo;
			il_servo_info_t info;

			servo = il_servo_create(net, servo_id->id, NULL);
			if (!servo)
				continue;

			if (il_servo_info_get(servo, &info) < 0)
				goto cleanup_servo;

			log_info("-------------------------------------------");
			log_info("%s, 0x%02x", info.name, servo_id->id);
			log_info("\tSerial number: %u", info.serial);
			log_info("\tSoftware version: %s", info.sw_version);
			log_info("\tHardware variant: %s", info.hw_variant);
			log_info("\tProduct code: 0x%08x", info.prod_code);
			log_info("\tProduct revision: 0x%08x", info.revision);
			log_info("-------------------------------------------");

cleanup_servo:
			il_servo_destroy(servo);
		}

		/* free resources */
		il_net_servos_list_destroy(servo_ids);
		il_net_destroy(net);
	}

	il_net_dev_list_destroy(devs);

	return 0;
}
