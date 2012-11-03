#include<linux/semaphore.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include<linux/ipc.h>

struct scull_dev{
	struct scull_qset *data;
	int quantum;
	int qset;
	unsigned long size;
    unsigned int access_key;
    struct semaphore sem;
    struct cdev cdev;
};    
#include<linux/slab.h>
