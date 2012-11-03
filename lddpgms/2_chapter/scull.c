#include "../include/scull.h"
MODULE_LICENSE("GPL");
static char *whom = "Linux WOrld";
static int howmany = 1;

struct file_operations scull_fops={
    .owner = THIS_MODULE,
    .llseek=scull_llseek,
    .read = scull_read,
    .write = scull_write,
    .ioctl = scull_ioctl,
    .open = scull_open,
    .release=scull_release,
};
int scull_open(struct inode *inode,struct file *filp)
{
    struct scull_dev *dev;
    dev =   container_of(inode->i_dev,struct scull_dev,cdev);
    filp->private_data  =   dev;
    if((filp->f_flags & O_ACCMODE) == O_WRONLY)
    {
        scull_trim(dev);
    }
}
int scull_release(struct inode *inode,struct file *filp)
{
    return 0;
}
int scull_trim(struct scull_dev *dev)
{
    struct scull_qset *next,*dptr;
    int qset = dev->qset;
    int i;

    for(dptr=dev->data;dptr;dptr=next)
    {
        if(dptr->data)
          {
              for(i=0;i<qset;i++)
                  kfree(dptr->data[i]);
              kfree(dptr->data);
              dptr->data=NULL;

          }
        next    =   dptr->next;
        kfree(dptr);
    }
    dev->size   =0;
    dev_quantum =scull_quantum;
    dev_qset    =scull_qset;
    dev_data    =NULL;
    return 0;

}
static void scull_setup_cdev(struct scull_dev *dev,int index)
{
    int err,devno=MKDEV(scull_major,scull_minor+index);
    cdev_init(&dev->cdev,&scull_fops);
    dev->cdev.owner = THIS_MODULE;
    dev->cdev.ops = &scull_fops;
    err = cdev_add(&dev->cdev,devno,1);
    if(err)
    {
        printk(KERN_NOTICE "Error %dadding scull:%d",err,index);
    }
}






static int hello_init(void)
{
	printk(KERN_NOTICE "HEllo world\n");
     
    




	return 0;
}

static void hello_exit(void)
{
	printk(KERN_ALERT "Goodbye\n");
}
module_param(howmany,int,S_IRUGO);
module_param(whom,charp,S_IRUGO);

module_init(hello_init);
module_exit(hello_exit);
