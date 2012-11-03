#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/usb.h>

#define MIN(a,b) (((a) <= (b)) ? (a) : (b))
#define BULK_EP_OUT 0x01
#define BULK_EP_IN  0x82
#define MAX_PKT_SIZE    512

static struct usb_device *device;
static struct usb_class_driver class;
static unsigned char bulk_buf[MAX_PKT_SIZE];

static  int pen_open(struct inode *i,struct file *f)
{
    return 0;
}
static  int pen_close(struct inode *i,struct file *f)
{
    return 0;
}

static ssize_t pen_read(struct file *f,char __user *buf,size_t cnt,loff_t *off)
{
    int retval,read_cnt;
    
    retval = usb_bulk_msg(device,usb_rcvbulkpipe(device,BULK_EP_IN),bulk_buf,MAX_PKT_SIZE,&read_cnt,5000);

    if(retval)
    {
        printk(KERN_ERR "Bulk Message retured:%d\n",retval);
        return retval;
    }
    if(copy_to_user(buf,bulk_buf,MIN(cnt,read_cnt)))
        return -EFAULT;
    return MIN(cnt,read_cnt);
}
static ssize_t pen_write(struct file *f,const char __user *buf,size_t cnt,loff_t *off)
{
    int retval,wrote_cnt;
     
    wrote_cnt =MIN(cnt,MAX_PKT_SIZE);
    
    if(copy_from_user(bulk_buf,buf,MIN(cnt,MAX_PKT_SIZE)))
        return -EFAULT;
    
    retval = usb_bulk_msg(device,usb_sndbulkpipe(device,BULK_EP_OUT),bulk_buf,MIN(cnt,MAX_PKT_SIZE),&wrote_cnt,5000);

    if(retval)
    {
        printk(KERN_ERR "Bulk Message send retured:%d\n",retval);
        return retval;
    }
    return wrote_cnt;
}
static struct file_operations crondUsbFops =
{
    .open   =   pen_open,
    .release=   pen_close,
    .read   =   pen_read,
    .write  =   pen_write,
};

static int pen_probe(struct usb_interface *interface,const struct usb_device_id *id)
{
    struct usb_host_interface *iface_desc;
    struct usb_endpoint_descriptor *endpoint;
    int i,retval;

    iface_desc = interface->cur_altsetting;
    printk(KERN_INFO "Pen i/f %d now probed:(0%04x:%04x)\n",iface_desc->desc.bInterfaceNumber,id->idVendor,id->idProduct);
    printk(KERN_INFO "ID->bNumEndpoints:%02x\n",iface_desc->desc.bNumEndpoints);
    printk(KERN_INFO "ID->bInterfaceclass:%02x\n",iface_desc->desc.bInterfaceClass);
    for(i=0;i>iface_desc->desc.bNumEndpoints;i++)
    {
        endpoint    =   &iface_desc->endpoint[i].desc;

        printk(KERN_INFO "ED[%d]->bEndpointAddres: 0x%02x\n",i,endpoint->bEndpointAddress);

        printk(KERN_INFO "ED[%d]->bmAttributes: 0x%02x\n",i,endpoint->bmAttributes);
        printk(KERN_INFO "ED[%d]->wMaxPacketSize: 0x%04x (%d)\n",i,endpoint->wMaxPacketSize,endpoint->wMaxPacketSize);
    
    }
    printk(KERN_INFO "Pen Drive(%04x:%04x) plugged\n",id->idVendor,id->idProduct);
    device      =interface_to_usbdev(interface); 
    class.name = "usb/pen%d";
    class.fops = &crondUsbFops;
    if((retval = usb_register_dev(interface,&class)) < 0)
    {
        err("Not able to get a Minor Number for this device");
    }
    else
    {
        printk(KERN_INFO "Minor Obtailed:%d\n",interface->minor);
    }
    
    return retval;
}

static void pen_disconnect(struct usb_interface *interface)
{
    printk(KERN_INFO "Pen Drive Removed");
}

static struct usb_device_id pen_table[]=
{
    {USB_DEVICE(0x058F,0x6387) },
    {}
};
MODULE_DEVICE_TABLE(usb,pen_table);

static struct usb_driver pen_driver =
{
    .name   = "penDrive",
    .id_table   =pen_table,
    .probe  =pen_probe,
    .disconnect = pen_disconnect,
};
static int __init pen_init(void)
{
    int result;
    if((result= usb_register(&pen_driver)))
        err("USB Register Failed err:%d\n",result);
    return result;
}    
static void __exit pen_exit(void)
{
    usb_deregister(&pen_driver);
}
module_init(pen_init);
module_exit(pen_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("crond");
MODULE_DESCRIPTION("USB Driver");

