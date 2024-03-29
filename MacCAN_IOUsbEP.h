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
#ifndef MACCAN_IOUSBEP_H_INCLUDED
#define MACCAN_IOUSBEP_H_INCLUDED

#include "MacCAN_Common.h"

#ifndef CANEPE_MAX_ENDPOINTS
#define CANEPE_MAX_ENDPOINTS  (UInt8)(8)
#endif

typedef int CANEPE_Handle_t;
typedef int CANEPE_Return_t;

typedef struct usb_endpoints_tag {      /* USB endpoint enumeration: */
    UInt8 numEndpoints;                 /*   number of endpoints (w/ EP0) */
    struct {                            /*   max. packet size: */
        struct {                        /*     pipe properties: */
            UInt8 pipeRef;              /*       pipe number */
            UInt16 packetSize;          /*       max. packet size */
        } pipeIn, pipeOut;              /*   note: only bulk in and bulk out */
    } endpoint[CANEPE_MAX_ENDPOINTS];   /*   array of endpoints (w/o EP0) */
} CANEPE_Endpoints_t;

#ifdef __cplusplus
extern "C" {
#endif

extern CANEPE_Return_t CANEPE_QueryEndpoints(CANEPE_Handle_t handle, CANEPE_Endpoints_t *endpoints);
extern UInt8 CANEPE_GetNumEndpoints(const CANEPE_Endpoints_t *endpoints);

extern UInt8 CANEPE_GetPipeRefIn(UInt8 endpoint, const CANEPE_Endpoints_t *endpoints);
extern UInt8 CANEPE_GetPipeRefOut(UInt8 endpoint, const CANEPE_Endpoints_t *endpoints);

extern UInt16 CANEPE_GetPacketSizeIn(UInt8 endpoint, const CANEPE_Endpoints_t *endpoints);
extern UInt16 CANEPE_GetPacketSizeOut(UInt8 endpoint, const CANEPE_Endpoints_t *endpoints);

#ifdef __cplusplus
}
#endif
#endif /* MACCAN_IOUSBEP_H_INCLUDED */

/* * $Id$ *** (c) UV Software, Berlin ***
 */

