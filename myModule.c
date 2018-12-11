#include <linux/init.h>     // module init and exit
#include <linux/module.h>   //version info, liscence, author
#include "deviceDriver.h"   //our device registration

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Bryan Clark");
//need to have identical signature
//of int init(void)
static int myInit(void)
{
    int result = 0;
    printk(KERN_NOTICE "My Module initialization started!");

    result = registerDevice();
    return result;
}

//need to have identical signature
//of void exit(void)
static void myExit(void)
{
    printk(KERN_NOTICE "My Module is exiting!");
    unregisterDevice();
    return;
}

//notify the kernel about driver loading and unloading
module_init(myInit);
module_exit(myExit);
