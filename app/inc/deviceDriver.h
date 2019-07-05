#ifndef DEVICE_DRIVER_H_
#define DEVICE_DRIVER_H_
#include <linux/compiler.h> /* __must_check */

__must_check int registerDevice(void);

void unregisterDevice(void);

#endif //DEVICE_DRIVER_H_
