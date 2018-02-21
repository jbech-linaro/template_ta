/*
 * Copyright (c) 2017, Linaro Limited
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */
#ifndef __HOTP_TA_H__
#define __HOTP_TA_H__

/*
 * This TA implements HOTP according to:
 * https://www.ietf.org/rfc/rfc4226.txt
 */

/*
#define TA_HOTP_UUID \
	{ 0x484d4143, 0x2d53, 0x4841, \
		{ 0x31, 0x20, 0x4a, 0x6f, 0x63, 0x6b, 0x65, 0x42 } }
*/

#define TEMPLATE_TA_UUID { 0x11111111, 0x2222, 0x3333, \
	{ 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb} }

/* The function ID(s) implemented in this TA */
#define TA_HOTP_CMD_REGISTER_SHARED_KEY	0
#define TA_HOTP_CMD_GET_HOTP		1

#endif
