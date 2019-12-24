/*load, write and read - 3-step way to assure your semester performance
 *
 */

#define __KERNEL__
#define MODULE

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

MODULE_AUTHOR("Nicolas R. Howe");
MODULE_DESCRIPTION("Linux module: first & tough");
MODULE_LICENSE("GPL");

static int __init buffer_init_module(void);
static void __exit buffer_exit_module(void);
static int device_open(struct inode *, struct file *); // driver file opening
static int device_release(struct inode *, struct file *); // return of system resource control
static ssize_t device_read(struct file *, char *, size_t, loff_t *); // reading from driver file
static ssize_t device_write(struct file *, const char *, size_t, loff_t *); // writing into driver file

#define DEVICE_NAME "buffer"
#define BUF_LEN 80

static int Major;
static int Device_Open = 0;
static int total_open = 1;

static char Buf[BUF_LEN + 1] = "Buffer is empty, add some input\n";
static char *Msg_ptr;
static int Buf_Char = 50;
static int Bytes_Read = 0;

static struct file_operations fops = {
        .read = device_read,
        .write = device_write,
        .open = device_open,
        .release = device_release
};

int buffer_init_module(void)
{
    printk(KERN_INFO
    "Device initializing in progress...");
    Major = register_chrdev(0, DEVICE_NAME, &fops);

    if(Major < 0) {
        printk("Major number hasn't been assigned - Driver registration failed\n");
        return Major;
    }

    printk(KERN_INFO "Registration success - device major number: %d\n", Major);

    return 0;
}

void buffer_exit_module(void)
{
    unregister_chrdev(Major, DEVICE_NAME);
    printk(KERN_INFO "Outside the module - exit successfully completed\n");
}

static int device_open(struct inode *inode, struct file *file)
{
    if(Device_Open)
        return -EBUSY;
    Device_Open++;
    printk(KERN_INFO "Device file has been accessed %d time(s)\n", total_open++);
    Msg_ptr = Buf;
    try_module_get(THIS_MODULE);
    Bytes_Read = 0;

    return 0;
}

static int device_release(struct inode * node, struct file * filep)
{
    Device_Open--;
    module_put(THIS_MODULE);
    printk(KERN_INFO "Device file gets close\n");
    return 0;
}

static ssize_t device_read(struct file * filep, char * buffer, size_t len, loff_t * offset)
{
    int got_read = Bytes_Read;

    if(Bytes_Read >= Buf_Char)
        return 0;

    while(len && (Bytes_Read < Buf_Char)) {
        put_user(Msg_ptr[Bytes_Read], buffer+Bytes_Read);
        len--;
        Bytes_Read++;
    }
    return Bytes_Read-got_read;
}

static ssize_t device_write(struct file * filep, const char * buffer, size_t len, loff_t * offset)
{
    Buf_Char = 0;
    if(Buf_Char >= BUF_LEN) {
        return 0;
    }

    while(len && (Buf_Char < BUF_LEN))
    {
        get_user(Msg_ptr[Buf_Char], buffer+Buf_Char);
        len--;
        Buf_Char++;
    }

    return Buf_Char;
}

module_init(buffer_init_module);
module_exit(buffer_exit_module);

