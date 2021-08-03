#ifndef __FDS_USB_API_H
#define __FDS_USB_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void usb_reset(struct usb_device *dev);

void cmdWrite(struct usb_device *dev, int *pIntBuff, int length);
void dataWrite(struct usb_device *dev, int *pIntBuff, int length);
void dataRead(struct usb_device *dev, int *pIntBuff, int length);

void bfmWrite(struct usb_device *dev, int addr, int *pIntBuff, int length);
void bfmRead(struct usb_device *dev, int addr, int *pIntBuff, int length);

#ifdef __cplusplus
}
#endif
#endif /* __FDS_USB_API_H */
