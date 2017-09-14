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

#ifndef PUBLIC_INGENIALINK_AXIS_H_
#define PUBLIC_INGENIALINK_AXIS_H_

#include "net.h"
#include "registers.h"

IL_BEGIN_DECL

/**
 * @file ingenialink/axis.h
 * @brief Axis.
 * @defgroup IL_AXIS Axis
 * @ingroup IL
 * @{
 */

/** IngeniaLink axis instance. */
typedef struct il_axis il_axis_t;

/** Default communications timeout (ms). */
#define IL_AXIS_TIMEOUT_DEF	1000

/** Axis operation modes. */
typedef enum {
	/** Profile position. */
	IL_AXIS_MODE_PP = 1,
	/** Profile velocity. */
	IL_AXIS_MODE_PV = 3,
	/** Profile torque. */
	IL_AXIS_MODE_PT = 4,
	/** Homing. */
	IL_AXIS_MODE_HOMING = 6
} il_axis_mode_t;

/** Torque units. */
typedef enum {
	/** Native. */
	IL_UNITS_TORQUE_NATIVE,
	/** Millinewtons. */
	IL_UNITS_TORQUE_MN,
	/** Newtons. */
	IL_UNITS_TORQUE_N
} il_units_torque_t;

/** Position units. */
typedef enum {
	/** Native. */
	IL_UNITS_POS_NATIVE,
	/** Revolutions. */
	IL_UNITS_POS_REV,
	/** Radians. */
	IL_UNITS_POS_RAD,
	/** Degrees. */
	IL_UNITS_POS_DEG,
	/** Micrometers. */
	IL_UNITS_POS_UM,
	/** Millimeters. */
	IL_UNITS_POS_MM,
	/** Meters. */
	IL_UNITS_POS_M,
} il_units_pos_t;

/** Velocity units. */
typedef enum {
	/** Native. */
	IL_UNITS_VEL_NATIVE,
	/** Revolutions per second. */
	IL_UNITS_VEL_RPS,
	/** Revolutions per minute. */
	IL_UNITS_VEL_RPM,
	/** Radians/second. */
	IL_UNITS_VEL_RAD_S,
	/** Degrees/second. */
	IL_UNITS_VEL_DEG_S,
	/** Micrometers/second. */
	IL_UNITS_VEL_UM_S,
	/** Millimeters/second. */
	IL_UNITS_VEL_MM_S,
	/** Meters/second. */
	IL_UNITS_VEL_M_S,
} il_units_vel_t;

/** Acceleration units. */
typedef enum {
	/** Native. */
	IL_UNITS_ACC_NATIVE,
	/** Revolutions/second^2. */
	IL_UNITS_ACC_REV_S2,
	/** Radians/second^2. */
	IL_UNITS_ACC_RAD_S2,
	/** Degrees/second^2. */
	IL_UNITS_ACC_DEG_S2,
	/** Micrometers/second^2. */
	IL_UNITS_ACC_UM_S2,
	/** Millimeters/second^2. */
	IL_UNITS_ACC_MM_S2,
	/** Meters/second^2. */
	IL_UNITS_ACC_M_S2,
} il_units_acc_t;

/** IngeniaLink units. */
typedef struct {
	/** Torque. */
	il_units_torque_t torque;
	/** Position. */
	il_units_pos_t pos;
	/** Velocity. */
	il_units_vel_t vel;
	/** Acceleration. */
	il_units_acc_t acc;
} il_units_t;

/**
 * Create IngeniaLink axis instance.
 *
 * @param [in] net
 *	IngeniaLink network.
 * @param [in] id
 *	Axis id.
 * @param [in] timeout
 *	Communications timeout (ms).
 *
 * @return
 *	Axis instance (NULL if it could not be created).
 */
IL_EXPORT il_axis_t *il_axis_create(il_net_t *net, uint8_t id, int timeout);

/**
 * Destroy an IngeniaLink axis instance.
 *
 * @param [in] axis
 *	IngeniaLink axis instance.
 */
IL_EXPORT void il_axis_destroy(il_axis_t *axis);

/**
 * Read data from a register.
 *
 * @param [in] axis
 *	IngeniaLink axis.
 * @param [in] reg
 *	Register.
 * @param [out] buf
 *	Buffer where to store received data.
 * @param [in] sz
 *	Buffer size.
 * @param [out] recvd
 *	Variable where the number of bytes received will be stored (optional).
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_axis_raw_read(il_axis_t *axis, const il_reg_t *reg, void *buf,
			       size_t sz, size_t *recvd);

/**
 * Read unsigned 8-bit value from a register.
 *
 * @param [in] axis
 *	IngeniaLink axis.
 * @param [in] reg
 *	Register.
 * @param [out] buf
 *	Buffer where to store received data.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_axis_raw_read_u8(il_axis_t *axis, const il_reg_t *reg,
				  uint8_t *buf);

/**
 * Read signed 8-bit value from a register.
 *
 * @param [in] axis
 *	IngeniaLink axis.
 * @param [in] reg
 *	Register.
 * @param [out] buf
 *	Buffer where to store received data.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_axis_raw_read_s8(il_axis_t *axis, const il_reg_t *reg,
				  int8_t *buf);

/**
 * Read unsigned 16-bit value from a register.
 *
 * @param [in] axis
 *	IngeniaLink axis.
 * @param [in] reg
 *	Register.
 * @param [out] buf
 *	Buffer where to store received data.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_axis_raw_read_u16(il_axis_t *axis, const il_reg_t *reg,
				   uint16_t *buf);

/**
 * Read signed 16-bit value from a register.
 *
 * @param [in] axis
 *	IngeniaLink axis.
 * @param [in] reg
 *	Register.
 * @param [out] buf
 *	Buffer where to store received data.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_axis_raw_read_s16(il_axis_t *axis, const il_reg_t *reg,
				   int16_t *buf);

/**
 * Read unsigned 32-bit value from a register.
 *
 * @param [in] axis
 *	IngeniaLink axis.
 * @param [in] reg
 *	Register.
 * @param [out] buf
 *	Buffer where to store received data.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_axis_raw_read_u32(il_axis_t *axis, const il_reg_t *reg,
				   uint32_t *buf);

/**
 * Read signed 32-bit value from a register.
 *
 * @param [in] axis
 *	IngeniaLink axis.
 * @param [in] reg
 *	Register.
 * @param [out] buf
 *	Buffer where to store received data.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_axis_raw_read_s32(il_axis_t *axis, const il_reg_t *reg,
				   int32_t *buf);

/**
 * Read unsigned 64-bit value from a register.
 *
 * @param [in] axis
 *	IngeniaLink axis.
 * @param [in] reg
 *	Register.
 * @param [out] buf
 *	Buffer where to store received data.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_axis_raw_read_u64(il_axis_t *axis, const il_reg_t *reg,
				   uint64_t *buf);

/**
 * Read signed 64-bit value from a register.
 *
 * @param [in] axis
 *	IngeniaLink axis.
 * @param [in] reg
 *	Register.
 * @param [out] buf
 *	Buffer where to store received data.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_axis_raw_read_s64(il_axis_t *axis, const il_reg_t *reg,
				   int64_t *buf);

/**
 * Read a register.
 *
 * This function will read from a register and also perform a unit conversion to
 * match the current operating units.
 *
 * @param [in] axis
 *	IngeniaLink axis.
 * @param [in] reg
 *	Register.
 * @param [out] buf
 *	Buffer where adjusted register content will be stored.
 *
 * @returns
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_axis_read(il_axis_t *axis, const il_reg_t *reg, double *buf);

/**
 * Write data to a register.
 *
 * @param [in] axis
 *	IngeniaLink axis.
 * @param [in] reg
 *	Register.
 * @param [in] buf
 *	Buffer where to pick data from.
 * @param [in] sz
 *	Buffer size.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_axis_raw_write(il_axis_t *axis, const il_reg_t *reg,
				const void *buf, size_t sz);

/**
 * Write unsigned 8-bit integer to a register.
 *
 * @param [in] axis
 *	IngeniaLink axis.
 * @param [in] reg
 *	Register.
 * @param [in] val
 *	Value.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_axis_raw_write_u8(il_axis_t *axis, const il_reg_t *reg,
				   uint8_t val);

/**
 * Write signed 8-bit integer to a register.
 *
 * @param [in] axis
 *	IngeniaLink axis.
 * @param [in] reg
 *	Register.
 * @param [in] val
 *	Value.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_axis_raw_write_s8(il_axis_t *axis, const il_reg_t *reg,
				   int8_t val);

/**
 * Write unsigned 16-bit integer to a register.
 *
 * @param [in] axis
 *	IngeniaLink axis.
 * @param [in] reg
 *	Register.
 * @param [in] val
 *	Value.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_axis_raw_write_u16(il_axis_t *axis, const il_reg_t *reg,
				    uint16_t val);

/**
 * Write signed 16-bit integer to a register.
 *
 * @param [in] axis
 *	IngeniaLink axis.
 * @param [in] reg
 *	Register.
 * @param [in] val
 *	Value.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_axis_raw_write_s16(il_axis_t *axis, const il_reg_t *reg,
				    int16_t val);

/**
 * Write unsigned 32-bit integer to a register.
 *
 * @param [in] axis
 *	IngeniaLink axis.
 * @param [in] reg
 *	Register.
 * @param [in] val
 *	Value.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_axis_raw_write_u32(il_axis_t *axis, const il_reg_t *reg,
				    uint32_t val);

/**
 * Write signed 32-bit integer to a register.
 *
 * @param [in] axis
 *	IngeniaLink axis.
 * @param [in] reg
 *	Register.
 * @param [in] val
 *	Value.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_axis_raw_write_s32(il_axis_t *axis, const il_reg_t *reg,
				    int32_t val);

/**
 * Write unsigned 64-bit integer to a register.
 *
 * @param [in] axis
 *	IngeniaLink axis.
 * @param [in] reg
 *	Register.
 * @param [in] val
 *	Value.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_axis_raw_write_u64(il_axis_t *axis, const il_reg_t *reg,
				    uint64_t val);

/**
 * Write signed 64-bit integer to a register.
 *
 * @param [in] axis
 *	IngeniaLink axis.
 * @param [in] reg
 *	Register.
 * @param [in] val
 *	Value.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_axis_raw_write_s64(il_axis_t *axis, const il_reg_t *reg,
				    int64_t val);

/**
 * Write to a register.
 *
 * This function will write to a register and also perform a unit conversion to
 * match the current operating units.
 *
 * @param [in] axis
 *	IngeniaLink axis.
 * @param [in] reg
 *	Register.
 * @param [in] val
 *	Value.
 *
 * @returns
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_axis_write(il_axis_t *axis, const il_reg_t *reg, double val);

/**
 * Set the operating units of an axis.
 *
 * @param [in] axis
 *	IngeniaLink axis.
 * @param [in] units
 *	Units.
 *
 * @returns
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_axis_units_set(il_axis_t *axis, const il_units_t *units);

/**
 * Obtain the operating units of an axis.
 *
 * @param [in] axis
 *	IngeniaLink axis.
 * @param [out] units
 *	Where current units will be stored.
 *
 * @returns
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_axis_units_get(il_axis_t *axis, il_units_t *units);

/**
 * Disable axis PDS.
 *
 * @param [in] axis
 *	IngeniaLink axis.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_axis_disable(il_axis_t *axis);

/**
 * Enable axis PDS.
 *
 * @param [in] axis
 *	IngeniaLink axis.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_axis_enable(il_axis_t *axis);

/**
 * Reset the drive fault state.
 *
 * @param [in] axis
 *	IngeniaLink axis.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_axis_fault_reset(il_axis_t *axis);

/**
 * Set the axis operation mode.
 *
 * @param [in] axis
 *	IngeniaLink axis.
 * @param [in] mode
 *	Mode.
 *
 */
IL_EXPORT int il_axis_mode_set(il_axis_t *axis, il_axis_mode_t mode);

/**
 * Start homing.
 *
 * @note
 *	This assumes that the drive is in the appropriate mote and that the PDS
 *	is enabled.
 *
 * @param [in] axis
 *	IngeniaLink axis.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_axis_homing_start(il_axis_t *axis);

/**
 * Wait until homing completes.
 *
 * @param [in] axis
 *	IngeniaLink axis.
 * @param [in] timeout
 *	Timeout (ms).
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_axis_homing_wait(il_axis_t *axis, int timeout);

/**
 * Get the actual axis torque.
 *
 * @param [in] axis
 *	IngeniaLink axis.
 * @param [out] torque
 *	Where the actual torque will be stored.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_axis_torque_get(il_axis_t *axis, double *torque);

/**
 * Set the axis target torque.
 *
 * @param [in] axis
 *	IngeniaLink axis.
 * @param [in] torque
 *	Torque.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_axis_torque_set(il_axis_t *axis, double torque);

/**
 * Get the actual axis position.
 *
 * @param [in] axis
 *	IngeniaLink axis.
 * @param [out] pos
 *	Where the actual position will be stored.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_axis_position_get(il_axis_t *axis, double *pos);

/**
 * Set the axis target position.
 *
 * @param [in] axis
 *	IngeniaLink axis.
 * @param [in] pos
 *	Position.
 * @param [in] immediate
 *	If set, the position will be set immediately, otherwise will be added
 *	to the internal axis queue.
 * @param [in] relative
 *	If set, the position is taken as a relative value, otherwise it is taken
 *	as an absolute value.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_axis_position_set(il_axis_t *axis, double pos, int immediate,
				   int relative);

/**
 * Get the actual axis velocity.
 *
 * @param [in] axis
 *	IngeniaLink axis.
 * @param [out] vel
 *	Where the actual velocity will be stored.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_axis_velocity_get(il_axis_t *axis, double *vel);

/**
 * Set the axis target velocity.
 *
 * @param [in] axis
 *	IngeniaLink axis.
 * @param [in] vel
 *	Velocity.
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_axis_velocity_set(il_axis_t *axis, double vel);

/**
 * Wait until the axis does a target reach.
 *
 * @param [in] axis
 *	IngeniaLink axis.
 * @param [in] timeout
 *	Timeout (ms).
 *
 * @return
 *	0 on success, error code otherwise.
 */
IL_EXPORT int il_axis_wait_reached(il_axis_t *axis, int timeout);

/** @} */

IL_END_DECL

#endif