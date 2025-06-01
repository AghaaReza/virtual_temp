#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/random.h>

#define PROC_NAME "virtual_temp"

static struct proc_dir_entry *proc_file;

// Generates a random temperature between 20 and 40
static int generate_random_temperature(void) {
    u8 temp;
    get_random_bytes(&temp, sizeof(temp));
    return 20 + (temp % 21);  // 20 to 40 inclusive
}

static ssize_t proc_read(struct file *file, char __user *buffer, size_t count, loff_t *pos) {
    static char msg[32];
    static int finished = 0;
    int len;
    int temp;

    if (finished) {
        finished = 0;
        return 0;
    }

    temp = generate_random_temperature();
    len = snprintf(msg, sizeof(msg), "Temperature: %d°C\n", temp);
    finished = 1;

    if (copy_to_user(buffer, msg, len))
        return -EFAULT;

    return len;
}

static const struct proc_ops proc_file_ops = {
    .proc_read = proc_read,
};

static int __init virtual_temp_init(void) {
    proc_file = proc_create(PROC_NAME, 0, NULL, &proc_file_ops);
    if (!proc_file) {
        pr_alert("virtual_temp: Failed to create /proc entry\n");
        return -ENOMEM;
    }
    pr_info("virtual_temp: Module loaded. /proc/%s created\n", PROC_NAME);
    return 0;
}

static void __exit virtual_temp_exit(void) {
    proc_remove(proc_file);
    pr_info("virtual_temp: Module unloaded. /proc/%s removed\n", PROC_NAME);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Reza Esmaeilzadeh");
MODULE_DESCRIPTION("Virtual temperature sensor using /proc");

module_init(virtual_temp_init);
module_exit(virtual_temp_exit);
