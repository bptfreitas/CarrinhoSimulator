

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/printk.h>
#include <linux/fs.h>

#include <linux/types.h>

#include <linux/cdev.h>

#include <linux/slab.h> // For kmalloc/kfree


#include <virtualbot.h>


struct virtualbot_dev *virtualbot_devices;


MODULE_LICENSE("GPL v2");

#define BUF_LEN 10
#define SUCCESS 0

// Simulador do Arduino
// crw-rw---- root dialout 166 0 -


// #define VIRTUALBOT_MAJOR 166

/*  
 *  Prototypes - this would normally go in a .h file
 */
int virtualbot_init(void);
void virtualbot_exit(void);

static int virtualbot_open(struct inode *, struct file *);
static int virtualbot_release(struct inode *, struct file *);
static ssize_t virtualbot_read(struct file *, char *, size_t, loff_t *);
static ssize_t virtualbot_write(struct file *, const char *, size_t, loff_t *);

/* 
 * Global variables are declared as static, so are global within the file. 
 */

static dev_t device;

//  static int Device_Open = 0;	/* Is device open?  
// * Used to prevent multiple access to device */
// static char msg[BUF_LEN];	/* The msg the device will give when asked */
// static char *msg_Ptr;


#define JAVINO_READ_TEST_STRING "fffe02OK"


static struct uart_driver virtualbot_uart = {
	.owner		= THIS_MODULE,
	.driver_name	= "virtualbot",
	.dev_name	= "ttyVirtualBot",
	.major		= TTY_MAJOR,
	.minor		= 64,
	.nr		= 1,
	//.cons		= ALTERA_JTAGUART_CONSOLE,
};

static struct platform_driver virtualbot_serial_driver = {
	.probe = virtualbot_serial_probe,
	.remove = virtualbot_serial_remove,
	.suspend = virtualbot_serial_suspend,
	.resume = virtualbot_serial_resume,
	.driver = {
		.name = "virtualbot_usart",
		.owner = THIS_MODULE,
	},
};



int __init virtualbot_init(void){

	int rc;

	rc = uart_register_driver(&virtualbot_uart);

	//if (rc)
	//	return rc;

	platform_driver_register( &virtualbot_serial_driver );

	return 0; 
};


void __exit virtualbot_exit(void){
	/* 
	 * Unregister the device 
	 */
	platform_driver_unregister( &virtualbot_serial_driver );

	uart_unregister_driver( &virtualbot_uart );


}


module_init(virtualbot_init);
module_exit(virtualbot_exit);
