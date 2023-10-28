/*  SPDX-License-Identifier: BSD-2-Clause OR GPL-3.0-or-later */
/*
 *  MacCAN - macOS User-Space Driver for USB-to-CAN Interfaces
 *
 *  Copyright (c) 2012-2023 Uwe Vogt, UV Software, Berlin (info@mac-can.com)
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
#include "MacCAN_IOUsbEP.h"
#include "MacCAN_IOUsbKit.h"
#include "MacCAN_Debug.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>
#include <inttypes.h>

#define PIPE2EP(x)  (((x) - 1U) >> 1)

CANEPE_Return_t CANEPE_QueryEndpoints(CANEPE_Handle_t handle, CANEPE_Endpoints_t *endpoints) {
    CANEPE_Return_t  retVal = CANUSB_ERROR_FATAL;

    /* sanity check */
    if (!endpoints)
        return CANUSB_ERROR_NULLPTR;

    /* initialization */
    bzero(endpoints, sizeof(CANEPE_Endpoints_t));

    /* get number of pipes from USB interface */
    UInt8 numPipes = 0U;
    retVal = CANUSB_GetInterfaceNumEndpoints(handle, &numPipes);
    if (retVal != CANUSB_SUCCESS) {
        MACCAN_DEBUG_ERROR("+++ Unable to read number of endpoints from device #%i (%i)\n", handle, retVal);
        return retVal;
    }
    /* note: IOUsbKit counts pipes not endpoints and also not EP0 */
    endpoints->numEndpoints = (numPipes / 2U) + 1U;

    /* loop over all pipes of the USB interface */
    for (UInt8 pipeRef = 1U; pipeRef <= numPipes; pipeRef++) {
        if (PIPE2EP(pipeRef) >= CANEPE_MAX_ENDPOINTS)
            break;

        /* get the tranfer type of the pipe: only bulk transfers supported */
        UInt8 transferType = USBPIPE_TYPE_ANY;
        retVal = CANUSB_GetInterfaceEndpointTransferType(handle, pipeRef, &transferType);
        if (retVal != CANUSB_SUCCESS) {
            MACCAN_DEBUG_ERROR("+++ Unable to read transfer type of pipe #%u from device #%i (%i)\n", pipeRef, handle, retVal);
            return retVal;
        }
        if (transferType != USBPIPE_TYPE_BULK) {
            MACCAN_DEBUG_ERROR("+++ Sorry, wrong transfer type of pipe #%u of device #%i (0x%x)\n", pipeRef, handle, transferType);
            return retVal;
        }
        /* get the transfer direction of the pipe: either bulk in or bulk out */
        UInt8 direction = USBPIPE_DIR_ANY;
        retVal = CANUSB_GetInterfaceEndpointDirection(handle, pipeRef, &direction);
        if (retVal != CANUSB_SUCCESS) {
            MACCAN_DEBUG_ERROR("+++ Unable to read transfer type of pipe #%u from device #%i (%i)\n", pipeRef, handle, retVal);
            return retVal;
        }
        if (direction == USBPIPE_DIR_IN) {
            /* get max packet size of the bulk in pipe of the endpoint */
            retVal = CANUSB_GetInterfaceEndpointMaxPacketSize(handle, pipeRef, &endpoints->endpoint[PIPE2EP(pipeRef)].pipeIn.packetSize);
            if (retVal != CANUSB_SUCCESS) {
                MACCAN_DEBUG_ERROR("+++ Unable to read package size of pipe #%u from device #%i (%i)\n", pipeRef, handle, retVal);
                return retVal;
            }
            /* set the pipe number of the bulk in pipe of the endpoint */
            endpoints->endpoint[PIPE2EP(pipeRef)].pipeIn.pipeRef = pipeRef;

        } else if (direction == USBPIPE_DIR_OUT) {
            /* get max packet size of the bulk out pipe of the endpoint */
            retVal = CANUSB_GetInterfaceEndpointMaxPacketSize(handle, pipeRef, &endpoints->endpoint[PIPE2EP(pipeRef)].pipeOut.packetSize);
            if (retVal != CANUSB_SUCCESS) {
                MACCAN_DEBUG_ERROR("+++ Unable to read package size of pipe #%u from device #%i (%i)\n", pipeRef, handle, retVal);
                return retVal;
            }
            /* set the pipe number of the bulk out pipe of the endpoint */
            endpoints->endpoint[PIPE2EP(pipeRef)].pipeOut.pipeRef = pipeRef;

        } else {
            MACCAN_DEBUG_ERROR("+++ Sorry, wrong transfer direction of pipe #%u of device #%i (0x%x)\n", pipeRef, handle, direction);
            return retVal;
        }
    }
    return CANUSB_SUCCESS;
}

UInt8 CANEPE_GetNumEndpoints(const CANEPE_Endpoints_t *endpoints) {
    /* return the number for endpoints */
    if (endpoints)
        return endpoints->numEndpoints;
    else
        return 0U;
}
UInt8 CANEPE_GetPipeRefIn(UInt8 endpoint, const CANEPE_Endpoints_t *endpoints) {
    /* return the pipe number of the bulk in pipe of the endpoint */
    if (endpoints && (1U <= endpoint) && (endpoint <= CANEPE_MAX_ENDPOINTS))
        return endpoints->endpoint[endpoint - 1U].pipeIn.pipeRef;
    else
        return 0U;
}

UInt8 CANEPE_GetPipeRefOut(UInt8 endpoint, const CANEPE_Endpoints_t *endpoints) {
    /* return the pipe number of the bulk out pipe of the endpoint */
    if (endpoints && (1U <= endpoint) && (endpoint <= CANEPE_MAX_ENDPOINTS))
        return endpoints->endpoint[endpoint - 1U].pipeOut.pipeRef;
    else
        return 0U;
}

UInt16 CANEPE_GetPacketSizeIn(UInt8 endpoint, const CANEPE_Endpoints_t *endpoints) {
    /* return max. packet size of the bulk in pipe of the endpoint */
    if (endpoints && (1U <= endpoint) && (endpoint <= CANEPE_MAX_ENDPOINTS))
        return endpoints->endpoint[endpoint - 1U].pipeIn.packetSize;
    else
        return 0U;
}

UInt16 CANEPE_GetPacketSizeOut(UInt8 endpoint, const CANEPE_Endpoints_t *endpoints) {
    /* return max. packet size of the bulk out pipe of the endpoint */
    if (endpoints && (1U <= endpoint) && (endpoint <= CANEPE_MAX_ENDPOINTS))
        return endpoints->endpoint[endpoint - 1U].pipeOut.packetSize;
    else
        return 0U;
}

/* * $Id$ *** (c) UV Software, Berlin ***
 */
