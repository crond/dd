<<<<<<< HEAD
<<<<<<< HEAD
#include "../include/scull.h"
#include <semaphore.h>
=======
#include <linux/init.h>
#include <linux/module.h>
>>>>>>> a20fe76c1e590242bd186317d91f84a406698f8d
=======
#include <linux/init.h>
#include <linux/module.h>
>>>>>>> a20fe76c1e590242bd186317d91f84a406698f8d

MODULE_LICENSE("GPL");
static char *whom = "Linux WOrld";
static int howmany = 1;
static int hello_init(void)
{
<<<<<<< HEAD
<<<<<<< HEAD
	printk(KERN_NOTICE "HEllo world\n");
=======
	printk(KERN_ALERT "HEllo world\n");
>>>>>>> a20fe76c1e590242bd186317d91f84a406698f8d
=======
	printk(KERN_ALERT "HEllo world\n");
>>>>>>> a20fe76c1e590242bd186317d91f84a406698f8d
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
