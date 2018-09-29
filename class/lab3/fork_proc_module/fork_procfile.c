/****************************************************************************

FILE   : fork_procfile.c
SUBJECT: Skeleton for a module that uses the seq file API.
AUTHOR : Chris Burnham - Modified from hello_procfile by Peter C. Chapin

This module creates a file in the /proc file system that uses the seq file API to display
"records" of information. This file is part of Vermont Technical College's CIS-4020 (Operating
Systems) class.

****************************************************************************/

#include <linux/module.h>   /* Needed for any module.         */
#include <linux/kernel.h>   /* Needed for printk, etc.        */
#include <linux/init.h>     /* Needed for init/exit macros.   */
#include <linux/proc_fs.h>  /* Needed for /proc stuff.        */
#include <linux/seq_file.h> /* Needed for the seq_file stuff. */

static Fork_info fork_record;

static const char module_name[20] = "fork_info";

/* Declare the sequence handling functions that we define (below). */
static void *fork_seq_start( struct seq_file *s, loff_t *pos );
static void *fork_seq_next ( struct seq_file *s, void *v, loff_t *pos );
static void  fork_seq_stop ( struct seq_file *s, void *v );
static int   fork_seq_show ( struct seq_file *s, void *v );

/* Gather our sequence handling functions into a seq_operations structure. */
static struct seq_operations fork_seq_ops = {
    .start = fork_seq_start,
    .next  = fork_seq_next,
    .stop  = fork_seq_stop,
    .show  = fork_seq_show
};

///////////////////////////////////////////////////////////////////////

/*
 * fork_seq_start
 *
 * This function is called each time the application calls read(). It starts the process of
 * accumulating data to fill the application buffer. Return a pointer representing the current
 * item. Return NULL if there are no more items.
 */
static void *fork_seq_start( struct seq_file *s, loff_t *record_number )
{
    // TODO: First Fork record

    return (get_last_fork_record(&fork_record) == -1) ? NULL : &fork_record;

//    extern unsigned long call_counters[NUM_COUNTERS];
//
//    if( *record_number >= NUM_COUNTERS ) 
//        return NULL;
//
//    return &call_counters[*record_number];
}

///////////////////////////////////////////////////////////////////////

/*
 * fork_seq_next
 *
 * This function is called to compute the next record in the sequence given a pointer to the
 * current record (in bookmark). It returns a pointer to the new record (essentially, an updated
 * bookmark) and updates *record_number appropriately. Return NULL if there are no more items.
 */
static void *fork_seq_next( struct seq_file *s, void *bookmark, loff_t *record_number )
{
  // TODO: Next fork record

//    extern unsigned long call_counters[NUM_COUNTERS];
//
//    (*record_number)++;
//
//    /* In this simple example record_number is sufficient to find the next item. */
//    if( *record_number >= NUM_COUNTERS ) 
//        return NULL;
//
//    return &call_counters[*record_number];
}

///////////////////////////////////////////////////////////////////////

/*
 * fork_seq_stop
 *
 * This function is called whenever an application buffer is filled (or when start or next
 * returns NULL. It can be used to undo any special preparations done in start (such as
 * deallocating auxillary memory that was allocated in start. In simple cases, you often do not
 * need to do anything in this function.
 */
static void fork_seq_stop( struct seq_file *s, void *bookmark )
{
    /* Nothing needed. */
}

///////////////////////////////////////////////////////////////////////

/*
 * fork_seq_show
 *
 * This function is called after next to actually compute the output. It can use various seq_...
 * printing functions (such as seq_printf) to format the output. It returns 0 if successful or a
 * negative value if it fails.
 */
static int fork_seq_show( struct seq_file *s, void *bookmark )
{
    // TODO: Print out a fork record

    //const unsigned long *count_ptr = (unsigned long *)bookmark;
    //extern unsigned long call_counters[NUM_COUNTERS];

    //return seq_printf( s, "%d : %lu\n", 
    //    (int)(count_ptr - &call_counters[0]), 
    //    *count_ptr );

    return seq_printf(s, "flags = 0x%08lX, user = %5d, ppid = %5d, 
                          cpid = %5d, comm = %8s, return = %ld\n",
                          fork_record.clone_flags, fork_record.parent_uid,
                          fork_record.parent_pid, fork_record.child_pid,
                          fork_record.command_name, fork_record.child_return);
}

///////////////////////////////////////////////////////////////////////

/* Define the only file handling function we need. */
static int fork_open( struct inode *inode, struct file *file )
{
    return seq_open( file, &fork_seq_ops );
}

///////////////////////////////////////////////////////////////////////

/* Use the seq_file file handling functions for some of the file operations. */
static struct file_operations fork_file_ops = {
    .owner   = THIS_MODULE,
    .open    = fork_open,
    .read    = seq_read,  /* Look at source of seq_read to understand protocol. */
    .llseek  = seq_lseek,
    .release = seq_release
};

///////////////////////////////////////////////////////////////////////

/*
 * system_fork_init  Called to load the module
 */
static int __init system_fork_init( void )
{
    struct proc_dir_entry *entry;
    kuid_t user_id;
    kgid_t group_id;

    /* Create a new entry in the /proc file system. */
    if( ( entry = proc_create( module_name, (S_IFREG | S_IRUGO), 
                               NULL, &fork_file_ops ) ) == NULL ) 
    {
      printk( KERN_ERR "fork_procfile NOT loaded. Error encountered creating /proc file.\n" );
      return -ENOMEM;
    }

    /* Fill in some fields of the proc_dir_entry */
    user_id.val = 0;
    group_id.val = 0;
    proc_set_user( entry, user_id, group_id );
    proc_set_size( entry, 0 ); /* A more appropriate size might be nice. */

    printk( KERN_INFO "fork_procfile loaded\n" );
    return 0;
}

///////////////////////////////////////////////////////////////////////

/*
 * system_fork_exit  Called to unload the module
 */
static void __exit system_fork_exit( void )
{
    /* Locate the named proc entry and unregister it. */
    remove_proc_entry( module_name, NULL );
    printk( KERN_INFO "fork_procfile unloaded\n" );
}

///////////////////////////////////////////////////////////////////////

/* Specify which functions do initialization and cleanup. */
module_init( system_fork_init );
module_exit( system_fork_exit );


/* Take care of some documentation tasks. */
MODULE_LICENSE( "GPL" );
MODULE_AUTHOR( "Chris Burnham <cburnham15@gmail.com>" );
MODULE_DESCRIPTION( "Module to display system call counters." );


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
