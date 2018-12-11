#include <linux/fs.h>       //our file system
#include <linux/kernel.h>   //printk
#include <linux/errno.h>    //error codes
#include <linux/module.h>   //THIS_MODULE
#include <linux/cdev.h>     //char device
#include <asm/uaccess.h>    //copy to user
#include "deviceDriver.h"

static const char _helloWorldString[] = "Hello world from the kernel mode!\n\0";
static const ssize_t _helloWorldSize = sizeof(_helloWorldString);

static ssize_t deviceFileRead(struct file* filePtr, char __user* userBuffer,
                                size_t count, loff_t* position);

//our file operation structure
static struct file_operations _bkcDriverFops =
{
   .owner   = THIS_MODULE,
   .read    = deviceFileRead,
};
//the major number this device will be registered toos
static int _deviceFileMajorNums = 0;
//signature
static const char deviceName[] = "BKC-Driver";

static ssize_t deviceFileRead(struct file* filePtr, char __user* userBuffer,
                                size_t count, loff_t* position)
{
    printk(KERN_NOTICE "BKC Driver: Device file is read at offset = %i,"\
                        "read bytes count = %u", (int)* position, (unsigned int) count);

    if(*position > _helloWorldSize)
    {
        return 0;
    }

    //check that we don't exceed the buffer size,
    //i.e. onl allow reading up to buffer size
    if( *position + count > _helloWorldSize )
      count = _helloWorldSize - *position;

    if(raw_copy_to_user(userBuffer, _helloWorldString + *position, count) != 0 )
      return -EFAULT;

    //update our buffer start position
    *position += count;
    return count;
}

int registerDevice(void)
{
      int result = 0;

      printk( KERN_NOTICE "BKC Driver: %s is called.", __FUNCTION__);

      result = register_chrdev( 0, deviceName, &_bkcDriverFops);
      if( result < 0 )
      {
         printk( KERN_WARNING "BKC Driver:  can\'t register character device with errorcode = %i", result );
         return result;
      }

      _deviceFileMajorNums = result;
      printk( KERN_NOTICE "BKC Driver: registered character device with major number = %i and minor numbers 0...255"
                  , _deviceFileMajorNums);

      return 0;
}

void unregisterDevice(void)
{
   printk( KERN_NOTICE "BKC Driver: unregisterDevice() is called" );
   if(_deviceFileMajorNums != 0)
   {
      unregister_chrdev(_deviceFileMajorNums, deviceName);
   }
}
