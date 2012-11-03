#include<linux/module.h>
#include<linux/version.h>
#include<linux/kernel.h>
#include<linux/types.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>

static dev_t devNum_crond;
static struct cdev c_dev;
static struct class *cl;

static int my_open(struct inode *i,struct file *f)
{
    printk(KERN_INFO "Driver: open()\n");
    return 0;
}
static int my_close(struct inode *i,struct file *f)
{
    printk(KERN_INFO "Driver: close()\n");
    return 0;
}
static int my_read(struct file *f,char __user *buf,size_t len,loff_t *off)
{
    printk(KERN_INFO "Driver:read()\n");
    return 0;
}
static int my_write(struct file *f,const char __user *buf,size_t len,loff_t *off)
{
    printk(KERN_INFO "Driver:write()\n");
    return len;
}
static struct file_operations crond_fops=
{   
    .owner  =THIS_MODULE,
    .open   =my_open,
    .release = my_close,
    .read=my_read,
    .write=my_write
};
static int __init ofd_init(void)
{
    printk(KERN_INFO "WELCOME: OFD Registered");

    //Just getting a free NUmber and mapping that with a user readble name
    if(alloc_chrdev_region(&devNum_crond,0,3,"cronddev") < 0)
	{
	    return -1;
    }
    printk(KERN_INFO "<Major, Minor>: <%d, %d>\n",MAJOR(devNum_crond),MINOR(devNum_crond));
    
    //creating class. A entry will be made, with the 2nd arg name in /sys/class dir
    if((cl=class_create(THIS_MODULE,"ch_class_crond")) ==NULL)
    {
        unregister_chrdev_region(devNum_crond,1);
        return -1;
    } 
    //A entry will be made inside the class dir creaed using class_create
    if(device_create(cl,NULL,devNum_crond,NULL,"dev_crond") == NULL)
    {
        class_destroy(cl);
        unregister_chrdev_region(devNum_crond,1);
        return -1;
    }
   /*
    * Init the device with the required file ops. So, that the VFS, which will identify the file requests from user space and determine the file type as
    * 1. Normal file [ in this case, concern FS driver invoked
    * 2. Device file [ device driver operations are invoked
    */
    cdev_init(&c_dev,&crond_fops);

    //Add the device. So, that udev daemon will create device, instead of 
    //manual adding using mknod 
  if(cdev_add(&c_dev,devNum_crond,1) == -1)
  {
      device_destroy(cl,devNum_crond);  
      class_destroy(cl);
        unregister_chrdev_region(devNum_crond,1);
        return -1;
    
  }  
	return 0; 
}
static void __exit ofd_exit(void)
{
    cdev_del(&c_dev);  
    device_destroy(cl,devNum_crond);  
      class_destroy(cl);
        unregister_chrdev_region(devNum_crond,1);
    printk(KERN_INFO "EXIT: ofd unregisterwed");
}

module_init(ofd_init);
module_exit(ofd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("crond");
MODULE_DESCRIPTION("First LFY LDD");
