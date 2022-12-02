#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <linux/kprobes.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/rcupdate.h>
#include <linux/uidgid.h>
#include <linux/signal.h>
#include <linux/security.h>
#include <linux/sched.h>

#define ONE_PAGE_SIZE   4096
#define FILE_NAME_SIZE  32
static char *int_str;
static int pid;
static int pgid;
static int add_cnt;
static int act_cnt;
static int shr_cnt;

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jinshu Liu");
MODULE_DESCRIPTION("LRU");
module_param(int_str, charp, S_IRUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(int_str, "A file");

static char symbol_read_iter[KSYM_NAME_LEN] = "generic_file_read_iter";
static char symbol_filemap_read[KSYM_NAME_LEN] = "filemap_read";
static char symbol_page_cache[KSYM_NAME_LEN] = "page_cache_sync_ra";
static char symbol_folio_add_lru[KSYM_NAME_LEN] = "folio_add_lru";
static char symbol_mark_accessed[KSYM_NAME_LEN] = "folio_mark_accessed";
static char symbol_active_fn[KSYM_NAME_LEN] = "folio_activate_fn";
static char symbol_lru_add_fn[KSYM_NAME_LEN] = "lru_add_fn";
static char symbol_shrink_lruvec[KSYM_NAME_LEN] = "shrink_lruvec";
module_param_string(symbol_read_iter, symbol_read_iter, KSYM_NAME_LEN, 0644);
module_param_string(symbol_filemap_read, symbol_filemap_read, KSYM_NAME_LEN, 0644);
module_param_string(symbol_page_cache, symbol_page_cache, KSYM_NAME_LEN, 0644);
module_param_string(symbol_folio_add_lru, symbol_folio_add_lru, KSYM_NAME_LEN, 0644);
module_param_string(symbol_mark_accessed, symbol_mark_accessed, KSYM_NAME_LEN, 0644);
module_param_string(symbol_active_fn, symbol_active_fn, KSYM_NAME_LEN, 0644);
module_param_string(symbol_lru_add_fn, symbol_lru_add_fn, KSYM_NAME_LEN, 0644);
module_param_string(symbol_shrink_lruvec, symbol_shrink_lruvec, KSYM_NAME_LEN, 0644);

static int get_curr_pgid(void) {
  struct pid *grp; 
  int retval;
  rcu_read_lock();
  grp = task_pgrp(current);
  retval = pid_vnr(grp);
  rcu_read_unlock();
  return retval;
}

static struct kprobe kp_read_iter = {
	.symbol_name	= symbol_read_iter,
};
static int __kprobes handler_pre_read_iter(struct kprobe *p, struct pt_regs *regs) {
	return 0;
}
static void __kprobes handler_post_read_iter(struct kprobe *p, struct pt_regs *regs, unsigned long flags) {
  if (current->pid != pid && get_curr_pgid() == pgid) {
    printk(KERN_INFO "POST generic_file_read_iter");
  }
  return;
}
static struct kprobe kp_filemap_read = {
  .symbol_name    = symbol_filemap_read,
};
static int __kprobes handler_pre_filemap_read(struct kprobe *p, struct pt_regs *regs) {
  return 0;
}
static void __kprobes handler_post_filemap_read(struct kprobe *p, struct pt_regs *regs, unsigned long flags) {
  if (current->pid != pid && get_curr_pgid() == pgid) {
    printk(KERN_INFO "POST filemap_read");
  }
  return;
}
static struct kprobe kp_page_cache = {
  .symbol_name    = symbol_page_cache,
};
static int __kprobes handler_pre_page_cache(struct kprobe *p, struct pt_regs *regs) {
  return 0;
}
static void __kprobes handler_post_page_cache(struct kprobe *p, struct pt_regs *regs, unsigned long flags) {
  if (current->pid != pid && get_curr_pgid() == pgid) {
    printk(KERN_INFO "POST page_cache_sync_ra");
  }
  return;
}
static struct kprobe kp_folio_add_lru = {
  .symbol_name    = symbol_folio_add_lru,
};
static int __kprobes handler_pre_folio_add_lru(struct kprobe *p, struct pt_regs *regs) {
  return 0;
}
static void __kprobes handler_post_folio_add_lru(struct kprobe *p, struct pt_regs *regs, unsigned long flags) {
  if (current->pid != pid && get_curr_pgid() == pgid) {
    // add_cnt += 1;
    printk(KERN_INFO "POST folio_add_lru");
  }
  return;
}
static struct kprobe kp_mark_accessed = {
  .symbol_name    = symbol_mark_accessed,
};
static int __kprobes handler_pre_mark_accessed(struct kprobe *p, struct pt_regs *regs) {
  return 0;
}
static void __kprobes handler_post_mark_accessed(struct kprobe *p, struct pt_regs *regs, unsigned long flags) {
  /*if (current->pid != pid && get_curr_pgid() == pgid) {
    printk(KERN_INFO "POST folio_mark_accessed");
  }*/
  return;
}
static struct kprobe kp_active_fn = {
  .symbol_name    = symbol_active_fn,
};
static int __kprobes handler_pre_active_fn(struct kprobe *p, struct pt_regs *regs) {
  return 0;
}
static void __kprobes handler_post_active_fn(struct kprobe *p, struct pt_regs *regs, unsigned long flags) {
  if (current->pid != pid && get_curr_pgid() == pgid) {
    act_cnt += 1;
    printk(KERN_INFO "POST folio_activate_fn");
  }
  return;
}
static struct kprobe kp_lru_add_fn = {
  .symbol_name = symbol_lru_add_fn,
};
static int __kprobes handler_pre_lru_add_fn(struct kprobe *p, struct pt_regs *regs) {
  return 0;
}
static void __kprobes handler_post_lru_add_fn(struct kprobe *p, struct pt_regs *regs, unsigned long flags) {
  if (current->pid != pid && get_curr_pgid() == pgid) {
    add_cnt += 1;
    printk(KERN_INFO "POST lru_add_fn");
  }
  return;
}
static struct kprobe kp_shrink_lruvec = {
  .symbol_name = symbol_shrink_lruvec,
};
static int __kprobes handler_pre_shrink_lruvec(struct kprobe *p, struct pt_regs *regs) {
  return 0;
}
static void __kprobes handler_post_shrink_lruvec(struct kprobe *p, struct pt_regs *regs, unsigned long flags) {
  if (current->pid != pid && get_curr_pgid() == pgid) {
    shr_cnt += 1;
    printk(KERN_INFO "POST shrink_lruvec");
  }
  return;
}

/*
static int get_pgid(pid_t pid) {
  struct task_struct *p;
	struct pid *grp;
	int retval;
  if (!pid) {
    return get_curr_pgid();
  } else {
    rcu_read_lock();
    retval = -ESRCH;
    p = find_task_by_vpid_def(pid);
    if (!p)
      goto out;
    grp = task_pgrp(p);
    if (!grp)
      goto out;
    retval = security_task_getpgid_def(p);
    if (retval)
      goto out;
  }
  retval = pid_vnr(grp);
out:
  rcu_read_unlock();
  return retval;
}*/

static int lru_show(struct seq_file *m, void *v) {
  seq_printf(m, "pid: %d, pgid: %d\n", pid, pgid);
  seq_printf(m, "addition times: %d\n activatation times: %d\n", add_cnt, act_cnt);
  seq_printf(m, "shrink_lruvec times: %d\n", shr_cnt);
  return 0;
}

static int __init lru_init(void) {
  int ret, err;
  char *p;
  p = kstrdup(int_str, GFP_KERNEL);
  if (!p)
    return -ENOMEM;
  err = kstrtoint(int_str, 0, &pid);
  kfree(p);
  if (err) {
    return err;
  }
  pgid = pid;
  add_cnt = 0;
  act_cnt = 0;
  shr_cnt = 0;
  printk(KERN_INFO "pid: %d, pgid: %d\n", pid, pgid);
  kp_read_iter.pre_handler = handler_pre_read_iter;
  kp_read_iter.post_handler = handler_post_read_iter;
  ret = register_kprobe(&kp_read_iter);
  if (ret < 0) {
    pr_err("register_kprobe read_iter failed, returned %d\n", ret);
    return ret;
  }
  kp_filemap_read.pre_handler = handler_pre_filemap_read;
  kp_filemap_read.post_handler = handler_post_filemap_read;
  ret = register_kprobe(&kp_filemap_read);
  if (ret < 0) {
    pr_err("register_kprobe batch_add failed, returned %d\n", ret);
    return ret;
  }
  kp_page_cache.pre_handler = handler_pre_page_cache;
  kp_page_cache.post_handler = handler_post_page_cache;
  ret = register_kprobe(&kp_page_cache);
  if (ret < 0) {
    pr_err("register_kprobe page_cache_sync_ra failed, returned %d\n", ret);
    return ret;
  }
  kp_folio_add_lru.pre_handler = handler_pre_folio_add_lru;
  kp_folio_add_lru.post_handler = handler_post_folio_add_lru;
  ret = register_kprobe(&kp_folio_add_lru);
  if (ret < 0) {
    pr_err("register_kprobe folio_add_lru failed, returned %d\n", ret);
    return ret;
  }
  kp_mark_accessed.pre_handler = handler_pre_mark_accessed;
  kp_mark_accessed.post_handler = handler_post_mark_accessed;
  ret = register_kprobe(&kp_mark_accessed);
  if (ret < 0) {
    pr_err("register_kprobe folio_mark_accessed, returned %d\n", ret);
    return ret;
  }
  kp_active_fn.pre_handler = handler_pre_active_fn;
  kp_active_fn.post_handler = handler_post_active_fn;
  ret = register_kprobe(&kp_active_fn);
  if (ret < 0) {
    pr_err("register_kprobe folio_activate_fn, returned %d\n", ret);
    return ret;
  }
  kp_lru_add_fn.pre_handler = handler_pre_lru_add_fn;
  kp_lru_add_fn.post_handler = handler_post_lru_add_fn;
  ret = register_kprobe(&kp_lru_add_fn);
  if (ret < 0) {
    pr_err("register_kprobe lru_add_fn, returned %d\n", ret);
    return ret;
  }
  kp_shrink_lruvec.pre_handler = handler_pre_shrink_lruvec;
  kp_shrink_lruvec.post_handler = handler_post_shrink_lruvec;
  ret = register_kprobe(&kp_shrink_lruvec);
  if (ret < 0) {
    pr_err("register_kprobe shrink_lruvec, returned %d\n", ret);
    return ret;
  }
  proc_create_single("lru", 0, NULL, lru_show);
  return 0;
}

static void __exit lru_exit(void) {
  unregister_kprobe(&kp_read_iter);
  unregister_kprobe(&kp_filemap_read);
  unregister_kprobe(&kp_page_cache);
  unregister_kprobe(&kp_folio_add_lru);
  unregister_kprobe(&kp_mark_accessed);
  unregister_kprobe(&kp_active_fn);
  unregister_kprobe(&kp_lru_add_fn);
  unregister_kprobe(&kp_shrink_lruvec);
  remove_proc_entry("lru", NULL);
}

module_init(lru_init);
module_exit(lru_exit);
