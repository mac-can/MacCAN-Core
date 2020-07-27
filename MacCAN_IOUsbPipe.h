/*
 *  MacCAN - macOS User-Space Driver for CAN to USB Interfaces
 *
 *  Copyright (C) 2012-2020  Uwe Vogt, UV Software, Berlin (info@mac-can.com)
 *
 *  This file is part of MacCAN-Core.
 *
 *  MacCAN-Core is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  MacCAN-Core is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with MacCAN-Core.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef MACCAN_IOUSBPIPE_H_INCLUDED
#define MACCAN_IOUSBPIPE_H_INCLUDED

#include "MacCAN_IOUsbKit.h"

#define CANUSB_OPTION_RX_QUEUE  0x0000U
#define CANUSB_OPTION_TX_QUEUE  0x0001U
#define CANUSB_OPTION_MSG_PIPE  0x0010U
#define CANUSB_OPTION_DEFAULT   CANUSB_OPTION_RX_QUEUE

/* CAN API V3 compatible error codes */
#define CANUSB_ERROR_FULL  (-20)
#define CANUSB_ERROR_EMPTY  (-30)

/* CAN API V3 compatible time-out value */
#define CANUSB_INFINITE  (65535U)

/* macro to set up a MacCAN pipe context */
#define CANUSB_PIPE_CONTEXT(ptr,hnd,ref,cbk,para) \
                            do{ if (ptr) { \
                                    (ptr)->handle = hnd; \
                                    (ptr)->pipeRef = ref; \
                                    (ptr)->callback = cbk; \
                                    (ptr)->ptrParam = para; \
                            } } while(0)
#define CANUSB_PIPE_RUNNING(pipe)  pipe.running
#define CANUSB_QUEUE_OVERFLOW(pipe)  pipe.msgQueue.ovfl.flag

#ifdef __cplusplus
extern "C" {
#endif

extern CANUSB_Return_t CANUSB_CreatePipe(CANUSB_UsbPipe_t *usbPipe, size_t bufferSize, size_t numElem, size_t elemSize, UInt16 options);

extern CANUSB_Return_t CANUSB_DestroyPipe(CANUSB_UsbPipe_t *usbPipe);

extern CANUSB_Return_t CANUSB_Enqueue(CANUSB_UsbPipe_t *usbPipe, void const *message/*, UInt16 timeout*/);

extern CANUSB_Return_t CANUSB_Dequeue(CANUSB_UsbPipe_t *usbPipe, void *message, UInt16 timeout);

extern CANUSB_Return_t CANUSB_ResetQueue(CANUSB_UsbPipe_t *usbPipe);

extern CANUSB_Return_t CANUSB_WritePacket(CANUSB_UsbPipe_t *usbPipe, void const *buffer, size_t nbytes);

extern CANUSB_Return_t CANUSB_ReadPacket(CANUSB_UsbPipe_t *usbPipe, void *buffer, size_t maxbytes, UInt16 timeout);

#ifdef __cplusplus
}
#endif
#endif /* MACCAN_IOUSBPIPE_H_INCLUDED */

/* * $Id$ *** (C) UV Software, Berlin ***
 */
