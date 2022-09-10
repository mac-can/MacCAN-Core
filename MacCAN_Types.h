/*  SPDX-License-Identifier: BSD-2-Clause OR GPL-3.0-or-later */
/*
 *  MacCAN - macOS User-Space Driver for USB-to-CAN Interfaces
 *
 *  Copyright (c) 2012-2022 Uwe Vogt, UV Software, Berlin (info@mac-can.com)
 *  All rights reserved.
 *
 *  This file is part of MacCAN-Core.
 *
 *  MacCAN-Core is dual-licensed under the BSD 2-Clause "Simplified" License and
 *  under the GNU General Public License v3.0 (or any later version).
 *  You can choose between one of them if you use this file.
 *
 *  BSD 2-Clause "Simplified" License:
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright notice, this
 *     list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 *  MacCAN-Core IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 *  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF MacCAN-Core, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  GNU General Public License v3.0 or later:
 *  MacCAN-Core is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  MacCAN-Core is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with MacCAN-Core.  If not, see <http://www.gnu.org/licenses/>.
 */
/** @file        MacCAN_Types.h
 *
 *  @brief       Driver-independent CAN Message - Data Types and Defines
 *
 *  @author      $Author$
 *
 *  @version     $Rev$
 *
 *  @addtogroup  mac_can
 *  @{
 */
#ifndef MACCAN_TYPES_H_INCLUDED
#define MACCAN_TYPES_H_INCLUDED

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/** @name  CAN Identifier
 *  @brief CAN Identifier range
 *  @{ */
#define CANMSG_MAX_STD_ID           0x7FF  /**< highest 11-bit identifier */
#define CANMSG_MAX_XTD_ID      0x1FFFFFFF  /**< highest 29-bit identifier */
/** @} */

/** @name  CAN Data Length
 *  @brief CAN payload length and DLC definition
 *  @{ */
#define CANMSG_MAX_DLC                  8  /**< max. data length code (CAN 2.0) */
#define CANMSG_MAX_LEN                  8  /**< max. payload length (CAN 2.0) */
/** @} */

/** @name  CAN FD Data Length
 *  @brief CAN FD payload length and DLC definition
 *  @{ */
#define CANMSG_MAX_FD_DLC               15  /**< max. data length code (CAN FD) */
#define CANMSG_MAX_FD_LEN               64  /**< max. payload length (CAN FD) */
/** @} */

/** @name  Internal flags
 *  @brief Additional message flags for internal use
 *  @{ */
#define CANMSG_FLAG_OVERRUN           0x80  /**< flag to mark a queue overrun */
/** @} */

/** @brief       CAN identifier (11-bit or 29-bit)
 */
typedef uint32_t CANMSG_CanId_t;

/** @brief       Message flags (16-bit, driver-secific)
 */
typedef uint16_t CANMSG_Flags_t;

/** @brief       Internal flags (8-bit, for internal use)
 */
typedef uint8_t  CANMSG_Extra_t;

/** @brief       CAN Data Length Code (0 .. 8 or 15 for CAN FD)
 */
typedef uint8_t  CANMSG_CanDlc_t;

/** @brief       CAN data (up to 8 or 64 for CAN FD)
 */
typedef uint8_t  CANMSG_CanData_t;

/** @brief       Time-stamp (64-bit, driver-specific)
 */
typedef uint64_t CANMSG_TimeStamp_t;

/** @brief       CAN Message (with Time-stamp):
 */
typedef struct canmsg_message_t_ {
    CANMSG_CanId_t canId;                   /**< CAN identifier (11-bit or 29-bit) */
    CANMSG_Flags_t flags;                   /**< message flags (16-bit, driver-secific) */
    CANMSG_Extra_t extra;                   /**< additional flags (8-bit, for internal use) */
    CANMSG_CanDlc_t canDlc;                 /**< CAN Data Length Code (0 .. 8 or 15) */
    CANMSG_CanData_t canData[CANMSG_MAX_FD_LEN];  /**< CAN data (to hold CAN FD payload) */
    CANMSG_TimeStamp_t timeStamp;           /**< time-stamp (64-bit, driver-specific) */
    // TODO: system time
    // TODO: frame counter
    // TODO: check-sum
    // TODO: info bytes
} CANMSG_CanMessage_t;

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif
#endif /* MACCAN_TYPES_H_INCLUDED */
/** @}
 */
/* * $Id$ *** (c) UV Software, Berlin ***
 */
