/**
PREEMPT_ACTIVE这个标志有什么用呢？
好像这个标志在preempt_count被设置就说明可以抢占，
不对，被设置说明不可以抢占.

atomic_clear_mask里有~mask这样的东西是不是不原子了，因为这样再加上addl指令就不止一条指令了。那么怎么原子呢？

多处理器的spinlock的底层处理是在特定架构的spinlock.h里定义的，而上层的spinlock函数在通用
spinlock.h文件里声明在spinlock.c文件定义实现调用底层处理函数；而单处理器spinlock底层函数
是在通用的spinlock.h里声明和实现。关于spinlock的DEBUG多单处理器也是不一样的,单的复杂一点。
spin_lock()函数都是调用_spin_lock()无论多单,_spin_lock()分多单。
像_raw_spin_lock()这些就是底层函数。

call_rcu里实现了一种用指针的指针实现了方便操作的插入

像up(), down(), down_trylock(), down_interrupt()这些函数与特定架构有关，
asm("	.section .sched.text,\"ax\"		\n\
	.align	5				\n\
	.globl	__down_failed			\n\
__down_failed:					\n\
	stmfd	sp!, {r0 - r3, lr}		\n\
	mov	r0, ip				\n\
	bl	__down				\n\
	ldmfd	sp!, {r0 - r3, pc}		\n\
						\n\
	.align	5				\n\
	.globl	__down_interruptible_failed	\n\
__down_interruptible_failed:			\n\
	stmfd	sp!, {r0 - r3, lr}		\n\
	mov	r0, ip				\n\
	bl	__down_interruptible		\n\
	mov	ip, r0				\n\
	ldmfd	sp!, {r0 - r3, pc}		\n\
						\n\
	.align	5				\n\
	.globl	__down_trylock_failed		\n\
__down_trylock_failed:				\n\
	stmfd	sp!, {r0 - r3, lr}		\n\
	mov	r0, ip				\n\
	bl	__down_trylock			\n\
	mov	ip, r0				\n\
	ldmfd	sp!, {r0 - r3, pc}		\n\
						\n\
	.align	5				\n\
	.globl	__up_wakeup			\n\
__up_wakeup:					\n\
	stmfd	sp!, {r0 - r3, lr}		\n\
	mov	r0, ip				\n\
	bl	__up				\n\
	ldmfd	sp!, {r0 - r3, pc}		\n\
	");
这几个函数是这样子定义的，用C-c s g和C-c s s是找不到的，只能用C-c s t来找.

* _down_trylock () 里为什么会把sleepers设为0呢？

* _down_interruptible()函数里为什么一会用current一会又用tsk呢？

* 怎么include/asm-i386/rwsem.h和include/linux/rwsem-spinlock.h都有struct rw_semaphore的定义呢？
在include/linux/rwsem.h里有一段这样的代码：
#ifdef CONFIG_RWSEM_GENERIC_SPINLOCK
#include <linux/rwsem-spinlock.h> \/* use a generic implementation *\/
#else
#include <asm/rwsem.h> \/* use an arch-specific implementation *\/
#endif
所以像init_rwsem()这样的函数在两个地方出现。
而down_read()只在include/linux/rwsem.h里出现，但是里面调用的__down_read()又分别在asm/rwsem.h和
lib/rwsem-spinlock.c里出现

* 有local_bh_enable()和__local_bh_enable()，前都调用do_softirq()

* asmlinkage void __sched preempt_schedule_irq(void)里面的__sched是不是把这个函数放到__sched段里呢？

* update_process_times()上有段这样的注释.
  Called from the timer interrupt handler to charge one tick to the current 
  process.  user_tick is 1 if the tick is user time, 0 for system.
  
* account_user_time()定义在sched.c里但是只在timer.c里被调用，真的有点不明白为什么这样安排。

* 找不到arm的__delay()函数

* arm好像是不调用calibrate_delay()的

* 还有__devinit之类的修辞符号的
 **/
/* page.h */
/* PAGE_SHIFT determines the page size */
#define PAGE_SHIFT		12
#define PAGE_SIZE		(1UL << PAGE_SHIFT)
#define PAGE_MASK		(~(PAGE_SIZE-1))

/* include/asm-i386/pgtable-3level-defs.h: */
#define PMD_SHIFT 21
#define PTRS_PER_PMD	512 	/* 2^(32-PMD_SHIFT) */
#define PGDIR_SHIFT	30
#define PTRS_PER_PGD	4 		/*  2^(32-PGDIR_SHIFT) */

/* include/asm-i386/pgtable-2level-defs.h: */
#define PGDIR_SHIFT 22

/* include/asm-i386/pgtable.h: */
#define _PAGE_BIT_PRESENT	0
#define _PAGE_BIT_RW		1
#define _PAGE_BIT_USER		2
#define _PAGE_BIT_PWT		3
#define _PAGE_BIT_PCD		4
#define _PAGE_BIT_ACCESSED	5
#define _PAGE_BIT_DIRTY		6
#define _PAGE_BIT_PSE		7	/* 4 MB (or 2MB) page, Pentium+, if present.. */
#define _PAGE_BIT_GLOBAL	8	/* Global TLB entry PPro+ */
#define _PAGE_BIT_UNUSED1	9	/* available for programmer */
#define _PAGE_BIT_UNUSED2	10
#define _PAGE_BIT_UNUSED3	11
#define _PAGE_BIT_NX		63

#define _PAGE_PRESENT	0x001
#define _PAGE_RW	0x002
#define _PAGE_USER	0x004
#define _PAGE_PWT	0x008
#define _PAGE_PCD	0x010
#define _PAGE_ACCESSED	0x020
#define _PAGE_DIRTY	0x040
#define _PAGE_PSE	0x080	/* 4 MB (or 2MB) page, Pentium+, if present.. */
#define _PAGE_GLOBAL	0x100	/* Global TLB entry PPro+ */
#define _PAGE_UNUSED1	0x200	/* available for programmer */
#define _PAGE_UNUSED2	0x400
#define _PAGE_UNUSED3	0x800

/* 以下的是arm的定义 */
/* /include/asm-arm/pgtable.h */
#define L_PTE_PRESENT		(1 << 0)
#define L_PTE_FILE		(1 << 1)	/* only when !PRESENT */
#define L_PTE_YOUNG		(1 << 1)
#define L_PTE_BUFFERABLE	(1 << 2)	/* matches PTE */
#define L_PTE_CACHEABLE		(1 << 3)	/* matches PTE */
#define L_PTE_USER		(1 << 4)
#define L_PTE_WRITE		(1 << 5)
#define L_PTE_EXEC		(1 << 6)
#define L_PTE_DIRTY		(1 << 7)

/* include/asm-i386/page.h: */
#ifdef CONFIG_X86_PAE
extern unsigned long long __supported_pte_mask;
typedef struct { unsigned long pte_low, pte_high; } pte_t;
typedef struct { unsigned long long pmd; } pmd_t;
typedef struct { unsigned long long pgd; } pgd_t;
typedef struct { unsigned long long pgprot; } pgprot_t;
#define pmd_val(x)	((x).pmd)
#define pte_val(x)	((x).pte_low | ((unsigned long long)(x).pte_high << 32))

/* include/asm-i386/page.h: */
#define __pte(x) ((pte_t) { (x) } )
#define __pgd(x) ((pgd_t) { (x) } )
#define __pgprot(x)	((pgprot_t) { (x) } )


/* sched.h */
struct task_struct {
	volatile long state;	/* -1 unrunnable, 0 runnable, >0 stopped */
	struct thread_info *thread_info;
	atomic_t usage;
	unsigned long flags;	/* per process flags, defined below */
	unsigned long ptrace;

	int lock_depth;		/* BKL lock depth */

	int prio, static_prio;
	struct list_head run_list;	/* where the process is in the run list. */
	prio_array_t *array;

	unsigned long sleep_avg;
	unsigned long long timestamp, last_ran;
	unsigned long long sched_time; /* sched_clock time spent running */
	int activated;

	unsigned long policy;
	cpumask_t cpus_allowed;
	unsigned int time_slice, first_time_slice;

#ifdef CONFIG_SCHEDSTATS
	struct sched_info sched_info;
#endif

	struct list_head tasks;		/**
								   process list
								*/
	/*
	 * ptrace_list/ptrace_children forms the list of my children
	 * that were stolen by a ptracer.
	 */
	struct list_head ptrace_children; /**
									   The head of a list containing all
									   children of P being traced by a debugger
									  */
	struct list_head ptrace_list; /**
								   The pointers to the next and previous
								   elements in the real parent's list of
								   traced processes (used when P is being
								   traced)
								  */

	struct mm_struct *mm, *active_mm;

/* task state */
	struct linux_binfmt *binfmt;
	long exit_state;
	int exit_code, exit_signal;
	int pdeath_signal;  /*  The signal sent when the parent dies  */
	/* ??? */
	unsigned long personality;
	unsigned did_exec:1;		/* 按POSIX要求设计的布尔量，
								   区分进程是正在执行老程序代码，
								   还是在执行execve装入的新代码 */
	pid_t pid;
	pid_t tgid;					/**
								   PID of the thread group leader of P
								 */
	/* 
	 * pointers to (original) parent process, youngest child, younger sibling,
	 * older sibling, respectively.  (p->father can be replaced with 
	 * p->parent->pid)
	 */
	struct task_struct *real_parent; /* real parent process (when being debugged) */
	struct task_struct *parent;	/* parent process */
	/*
	 * children/sibling forms the list of my children plus the
	 * tasks I'm ptracing.
	 */
	struct list_head children;	/* list of my children */
	struct list_head sibling;	/* linkage in my parent's children list */
	struct task_struct *group_leader;	/* threadgroup(processgroup) leader */

	/* PID/PID hash table linkage. */
	struct pid pids[PIDTYPE_MAX];

	struct completion *vfork_done;		/* for vfork(),
										   在do_fork中置1 
										 */
	int __user *set_child_tid;		/* CLONE_CHILD_SETTID */
	int __user *clear_child_tid;		/* CLONE_CHILD_CLEARTID */

	unsigned long rt_priority;
	cputime_t utime, stime;/**
							  utime = user time,
							  stime = system time,
							  cutime = cumulative user time (process + its children),
							  cstime = cumulative system time (process + its children) 
						   **/
	unsigned long nvcsw, nivcsw; /* context switch counts */
	struct timespec start_time;/**
								  value of the jiffies when the task was created
								**/
/* mm fault and swap info: this can arguably be seen as either mm-specific or thread-specific */
	unsigned long min_flt, maj_flt;/**
									  min_flt: minor fault, 
									  maj_flt: major fault (means that it had access to the disk),
									  cmin_flt: cumulative minor fault (process + its children),
									  cmaj_flt: cumulative major fault (process + its children)
									**/

  	cputime_t it_prof_expires, it_virt_expires;
	unsigned long long it_sched_expires;
	struct list_head cpu_timers[3];

/* process credentials */
	uid_t uid,euid,suid,fsuid;/**
								 uid: user identifier,
								 euid: effective UID used for privilege checks,
								 suid: saved UID used to support switching permission,
								 fsuid: UID used for filesystem access checks (used by NFS for 
								 example)
							   **/
	gid_t gid,egid,sgid,fsgid;/**
								 gid: group identifier, 
								 egid: effective GID used for privilege checks,
								 sgid: saved GID used to support switching permission,
								 fgid: GID used for filesystem access checks
							   **/
	struct group_info *group_info;
	kernel_cap_t   cap_effective, cap_inheritable, cap_permitted;/**
																	POSIX capability information. It's sets of bits that permit splitting of 
																	the privileges typically held by root into a larger set of more specific 
																	privileges. 
																  **/
	unsigned keep_capabilities:1;/**
									used to forbid the drop of all privileges.
								  **/
	struct user_struct *user;/**
								information about user who owns the process. 
							  **/
#ifdef CONFIG_KEYS
	struct key *thread_keyring;	/* keyring private to this thread */
#endif
	int oomkilladj; /* OOM kill score adjustment (bit shift). */
	char comm[TASK_COMM_LEN]; /* executable name excluding path
				     - access with [gs]et_task_comm (which lock
				       it with task_lock())
				     - initialized normally by flush_old_exec */
/* file system info */
	int link_count, total_link_count;
/* ipc stuff */
	struct sysv_sem sysvsem;
/* CPU-specific state of this task */
	struct thread_struct thread;
/* filesystem information */
	struct fs_struct *fs;
/* open file information */
	struct files_struct *files;
/* namespace */
	struct namespace *namespace;
/* signal handlers */
	struct signal_struct *signal;/**
									signal associated to the process
								  **/
	struct sighand_struct *sighand;/**
									  signal handler associated to the process
									**/

	sigset_t blocked, real_blocked;/**
									  signals that are blocked by the process
									**/
	struct sigpending pending;/**
								 signals generated but not yet delivered 
							   **/

	unsigned long sas_ss_sp;
	size_t sas_ss_size;
	int (*notifier)(void *priv);
	void *notifier_data;
	sigset_t *notifier_mask;
	
	void *security;
	struct audit_context *audit_context;
	seccomp_t seccomp;

/* Thread group tracking */
   	u32 parent_exec_id;
   	u32 self_exec_id;/**
						used to distinguish if we have changed execution domain by comparing the 
						two value. When changing execution domain, self_exec_id is incremented and
						then is different from parent_exec_id.
					  **/
/* Protection of (de-)allocation: mm, files, fs, tty, keyrings */
	spinlock_t alloc_lock;
/* Protection of proc_dentry: nesting proc_lock, dcache_lock, write_lock_irq(&tasklist_lock); */
	spinlock_t proc_lock;
/* context-switch lock */
	spinlock_t switch_lock;

/* journalling filesystem info */
	void *journal_info;

/* VM state */
	struct reclaim_state *reclaim_state;
	struct dentry *proc_dentry;
	struct backing_dev_info *backing_dev_info;

	struct io_context *io_context;

	unsigned long ptrace_message;
	siginfo_t *last_siginfo; /* For ptrace use.  */
/*
 * current io wait handle: wait queue entry to use for io waits
 * If this thread is processing aio, this points at the waitqueue
 * inside the currently handled kiocb. It may be NULL (i.e. default
 * to a stack based synchronous wait) if its doing sync IO.
 */
	wait_queue_t *io_wait;
/* i/o counters(bytes read/written, #syscalls */
	u64 rchar, wchar, syscr, syscw;
#if defined(CONFIG_BSD_PROCESS_ACCT)
	u64 acct_rss_mem1;	/* accumulated rss usage */
	u64 acct_vm_mem1;	/* accumulated virtual memory usage */
	clock_t acct_stimexpd;	/* clock_t-converted stime since last update */
#endif
#ifdef CONFIG_NUMA
  	struct mempolicy *mempolicy;
	short il_next;
#endif
#ifdef CONFIG_CPUSETS
	struct cpuset *cpuset;
	nodemask_t mems_allowed;
	int cpuset_mems_generation;
#endif
}

/* sched.h */
union thread_union {
	struct thread_info thread_info;                /* smaller 4k,but doesn't matter */
	unsigned long stack[THREAD_SIZE/sizeof(long)]; /* THREAD_SIZE is 4k or 8k */
};

/* thread_info.h */
struct thread_info {
	struct task_struct	*task;		/* main task structure */
	struct exec_domain	*exec_domain;	/* execution domain */
	unsigned long		flags;		/* low level flags */
	unsigned long		status;		/* thread-synchronous flags */
	__u32			cpu;		/* current CPU */
	__s32			preempt_count; /* 0 => preemptable, <0 => BUG */


	mm_segment_t		addr_limit;	/* thread address space:
					 	   0-0xBFFFFFFF for user-thead
						   0-0xFFFFFFFF for kernel-thread
						*/
	struct restart_block    restart_block;

	unsigned long           previous_esp;   /* ESP of the previous stack in case
						   of nested (IRQ) stacks
						*/
	__u8			supervisor_stack[0];
};

/* pid.c */
static pidmap_t pidmap_array[PIDMAP_ENTRIES] =
	 { [ 0 ... PIDMAP_ENTRIES-1 ] = { ATOMIC_INIT(BITS_PER_PAGE), NULL } };

/* include/linux/list.h */
struct list_head {
	struct list_head *next, *prev;
};

/* include/linux/list.h */
/*
 * Double linked lists with a single pointer list head.
 * Mostly useful for hash tables where the two pointer list head is
 * too wasteful.
 * You lose the ability to access the tail in O(1).
 */

struct hlist_head {
	struct hlist_node *first;
};

struct hlist_node {
	struct hlist_node *next, **pprev;
};

struct prio_array {
	unsigned int nr_active;
	unsigned long bitmap[BITMAP_SIZE];
	struct list_head queue[MAX_PRIO];
};


/*
 * NOTE! "signal_struct" does not have it's own
 * locking, because a shared signal_struct always
 * implies a shared sighand_struct, so locking
 * sighand_struct is always a proper superset of
 * the locking of signal_struct.
 */
struct signal_struct {
	atomic_t		count;
	atomic_t		live;

	wait_queue_head_t	wait_chldexit;	/* for wait4() */

	/* current thread group signal load-balancing target: */
	task_t			*curr_target;

	/* shared signal handling: */
	struct sigpending	shared_pending;

	/* thread group exit support */
	int			group_exit_code;
	/* overloaded:
	 * - notify group_exit_task when ->count is equal to notify_count
	 * - everyone except group_exit_task is stopped during signal delivery
	 *   of fatal signals, group_exit_task processes the signal.
	 */
	struct task_struct	*group_exit_task;
	int			notify_count;

	/* thread group stop support, overloads group_exit_code too */
	int			group_stop_count;
	unsigned int		flags; /* see SIGNAL_* flags below */

	/* POSIX.1b Interval Timers */
	struct list_head posix_timers;

	/**
	   - unsigned long it_real_value, it_prof_value, it_virt_value;
	   holds the current timer value. It's used to implement the specific 
	   interval timer (itmer). 
	   
	   - unsigned long it_real_incr, it_prof_incr, it_virt_incr;
	   holds the duration of the interval. It's used to implement the specific 
	   interval timer (itmer). 
	 **/
	/* ITIMER_REAL timer for the process */
	struct timer_list real_timer;
	unsigned long it_real_value, it_real_incr;

	/* ITIMER_PROF and ITIMER_VIRTUAL timers for the process */
	cputime_t it_prof_expires, it_virt_expires;
	cputime_t it_prof_incr, it_virt_incr;

	/* job control IDs */
	pid_t pgrp;					/**
								 PID of the group leader of P
								 */
	pid_t tty_old_pgrp;
	pid_t session;				/**
								 PID of the login session leader of P
								*/
	/* boolean value for session group leader */
	int leader;

	struct tty_struct *tty; /* NULL if no tty */

	/*
	 * Cumulative resource counters for dead threads in the group,
	 * and for reaped dead child processes forked by this group.
	 * Live threads maintain their own counters and add to these
	 * in __exit_signal, except for the group leader.
	 */
	cputime_t utime, stime, cutime, cstime;/**
											  utime = user time,
											  stime = system time,
											  cutime = cumulative user time (process + its children),
											  cstime = cumulative system time (process + its children) 
											**/
	unsigned long nvcsw, nivcsw, cnvcsw, cnivcsw;/**
													context switch counts
												  **/
	unsigned long min_flt, maj_flt, cmin_flt, cmaj_flt;/**
														  min_flt: minor fault, 
														  maj_flt: major fault (means that it had access to the disk),
														  cmin_flt: cumulative minor fault (process + its children),
														  cmaj_flt: cumulative major fault (process + its children)
													   **/

	/*
	 * Cumulative ns of scheduled CPU time for dead threads in the
	 * group, not including a zombie group leader.  (This only differs
	 * from jiffies_to_ns(utime + stime) if sched_clock uses something
	 * other than jiffies.)
	 */
	unsigned long long sched_time;

	/*
	 * We don't bother to synchronize most readers of this at all,
	 * because there is no reader checking a limit that actually needs
	 * to get both rlim_cur and rlim_max atomically, and either one
	 * alone is a single word that can safely be read normally.
	 * getrlimit/setrlimit use task_lock(current->group_leader) to
	 * protect this instead of the siglock, because they really
	 * have no need to disable irqs.
	 */
	struct rlimit rlim[RLIM_NLIMITS];

	struct list_head cpu_timers[3];

	/* keep the process-shared keyrings here so that they do the right
	 * thing in threads created with CLONE_THREAD */
#ifdef CONFIG_KEYS
	struct key *session_keyring;	/* keyring inherited over fork */
	struct key *process_keyring;	/* keyring private to this process */
#endif
};

/* 这样定义MAX的 */
enum pid_type
{
	PIDTYPE_PID,
	PIDTYPE_TGID,
	PIDTYPE_PGID,
	PIDTYPE_SID,
	PIDTYPE_MAX
};

/* task_struct->pids */
struct pid
{
	/* Try to keep pid_chain in the same cacheline as nr for find_pid */
	int nr;						/**
								 The PID number
								*/
	struct hlist_node pid_chain;
	/* list of pids with the same nr, only one of them is in the hash */
	struct list_head pid_list;
};

/* include/linux/list.h:
   第二维在开机时根据内存大小来初始化
*/
static struct hlist_head *pid_hash[PIDTYPE_MAX];

/* linux/wait.h: */
struct __wait_queue_head {
	spinlock_t lock;
	struct list_head task_list;
};
typedef struct __wait_queue_head wait_queue_head_t;


struct __wait_queue {
	unsigned int flags;			/* 1 - exclusive; 0 - nonexclusive */
#define WQ_FLAG_EXCLUSIVE	0x01
	struct task_struct * task;
	wait_queue_func_t func;		/* used to specify how the processes sleeping
								   in the wait queue should be woken up. */
	struct list_head task_list;
};


/*
 * cloning flags:
 */
#define CSIGNAL		0x000000ff	/* signal mask to be sent at exit */
#define CLONE_VM	0x00000100	/* set if VM shared between processes,
								 Shares the memory descriptor and all Page
								 Tables*/
#define CLONE_FS	0x00000200	/* set if fs info shared between processes */
#define CLONE_FILES	0x00000400	/* set if open files shared between processes */
#define CLONE_SIGHAND	0x00000800	/* set if signal handlers and blocked signals shared */
#define CLONE_PTRACE	0x00002000	/* set if we want to let tracing continue on the child too */
#define CLONE_VFORK	0x00004000	/* set if the parent wants the child to wake it up on mm_release */
#define CLONE_PARENT	0x00008000	/* set if we want to have the same parent as the cloner */
#define CLONE_THREAD	0x00010000	/* Same thread group? */
#define CLONE_NEWNS	0x00020000	/* New namespace group?
								 Set if the clone needs its own namespace,
								 that is, its own view of the mounted
								 filesystems (see Chapter 12); it
								 is not possible to specify both CLONE_NEWNS
								 and CLONE_FS.*/
#define CLONE_SYSVSEM	0x00040000	/* share system V SEM_UNDO semantics */
#define CLONE_SETTLS	0x00080000	/* create a new TLS for the child */
#define CLONE_PARENT_SETTID	0x00100000	/* set the TID in the parent */
#define CLONE_CHILD_CLEARTID	0x00200000	/* clear the TID in the child */
#define CLONE_DETACHED		0x00400000	/* Unused, ignored */
#define CLONE_UNTRACED		0x00800000	/* set if the tracing process can't force CLONE_PTRACE on this clone */
#define CLONE_CHILD_SETTID	0x01000000	/* set the TID in the child */
#define CLONE_STOPPED		0x02000000	/* Start in stopped state */



/*
 * PID-map pages start out as NULL, they get allocated upon
 * first use and are never deallocated. This way a low pid_max
 * value does not cause lots of bitmaps to be allocated, but
 * the scheme scales to up to 4 million PIDs, runtime.
 */
typedef struct pidmap {
	atomic_t nr_free;
	void *page;
} pidmap_t;

int pid_max = PID_MAX_DEFAULT;  /**
                                   最大的PID号/数，至少一页
                                 */

/*
 * cloning flags:
 */
#define CSIGNAL		0x000000ff	/* signal mask to be sent at exit */
#define CLONE_VM	0x00000100	/* set if VM shared between processes */
#define CLONE_FS	0x00000200	/* set if fs info shared between processes */
#define CLONE_FILES	0x00000400	/* set if open files shared between processes */
#define CLONE_SIGHAND	0x00000800	/* set if signal handlers and blocked signals shared */
#define CLONE_PTRACE	0x00002000	/* set if we want to let tracing continue on the child too */
#define CLONE_VFORK	0x00004000	/* set if the parent wants the child to wake it up on mm_release */
#define CLONE_PARENT	0x00008000	/* set if we want to have the same parent as the cloner */
#define CLONE_THREAD	0x00010000	/* Same thread group? */
#define CLONE_NEWNS	0x00020000	/* New namespace group? */
#define CLONE_SYSVSEM	0x00040000	/* share system V SEM_UNDO semantics */
#define CLONE_SETTLS	0x00080000	/* create a new TLS for the child */
#define CLONE_PARENT_SETTID	0x00100000	/* set the TID in the parent */
#define CLONE_CHILD_CLEARTID	0x00200000	/* clear the TID in the child */
#define CLONE_DETACHED		0x00400000	/* Unused, ignored */
#define CLONE_UNTRACED		0x00800000	/* set if the tracing process can't force CLONE_PTRACE on this clone */
#define CLONE_CHILD_SETTID	0x01000000	/* set the TID in the child */
#define CLONE_STOPPED		0x02000000	/* Start in stopped state */

struct user_struct {
	atomic_t __count;	/* reference count */
	atomic_t processes;	/* How many processes does this user have? */
	atomic_t files;		/* How many open files does this user have? */
	atomic_t sigpending;	/* How many pending signals does this user have? */
	/* protected by mq_lock	*/
	unsigned long mq_bytes;	/* How many bytes can be allocated to mqueue? */
	unsigned long locked_shm; /* How many pages of mlocked shm ? */

#ifdef CONFIG_KEYS
	struct key *uid_keyring;	/* UID specific keyring */
	struct key *session_keyring;	/* UID's default session keyring */
#endif

	/* Hash table maintenance information */
	struct list_head uidhash_list;
	uid_t uid;
};

struct group_info {
	int ngroups;
	atomic_t usage;
	gid_t small_block[NGROUPS_SMALL];
	int nblocks;
	gid_t *blocks[0];
/* personality.h */
struct exec_domain {
	const char		*name;		/* name of the execdomain */
	handler_t		handler;	/* handler for syscalls */
	unsigned char		pers_low;	/* lowest personality */
	unsigned char		pers_high;	/* highest personality */
	unsigned long		*signal_map;	/* signal mapping */
	unsigned long		*signal_invmap;	/* reverse signal mapping */
	struct map_segment	*err_map;	/* error mapping */
	struct map_segment	*socktype_map;	/* socket type mapping */
	struct map_segment	*sockopt_map;	/* socket option mapping */
	struct map_segment	*af_map;	/* address family mapping */
	struct module		*module;	/* module context of the ed. */
	struct exec_domain	*next;		/* linked list (internal) */
};


/*
 * Per process flags
 */
#define PF_ALIGNWARN	0x00000001	/* Print alignment warning msgs */
					/* Not implemented yet, only for 486 */
#define PF_STARTING	0x00000002	/* being created */
#define PF_EXITING	0x00000004	/* getting shut down */
#define PF_DEAD		0x00000008	/* Dead */
#define PF_FORKNOEXEC	0x00000040	/* forked but didn't exec */
#define PF_SUPERPRIV	0x00000100	/* used super-user privileges */
#define PF_DUMPCORE	0x00000200	/* dumped core */
#define PF_SIGNALED	0x00000400	/* killed by a signal */
#define PF_MEMALLOC	0x00000800	/* Allocating memory */
#define PF_FLUSHER	0x00001000	/* responsible for disk writeback */
#define PF_USED_MATH	0x00002000	/* if unset the fpu must be initialized before use */
#define PF_FREEZE	0x00004000	/* this task is being frozen for suspend now */
#define PF_NOFREEZE	0x00008000	/* this thread should not be frozen */
#define PF_FROZEN	0x00010000	/* frozen for system suspend */
#define PF_FSTRANS	0x00020000	/* inside a filesystem transaction */
#define PF_KSWAPD	0x00040000	/* I am kswapd */
#define PF_SWAPOFF	0x00080000	/* I am in swapoff */
#define PF_LESS_THROTTLE 0x00100000	/* Throttle me less: I clean memory */
#define PF_SYNCWRITE	0x00200000	/* I am doing a sync write */
#define PF_BORROWED_MM	0x00400000	/* I am a kthread doing use_mm */
#define PF_RANDOMIZE	0x00800000	/* randomize virtual address space */

/* include/asm-i386/ptrace.h: */
struct pt_regs {
	long ebx;
	long ecx;
	long edx;
	long esi;
	long edi;
	long ebp;
	long eax;
	int  xds;
	int  xes;
	long orig_eax;
	long eip;
	int  xcs;
	long eflags;
	long esp;
	int  xss;
};

/* each process descriptor includes a field called thread of type thread_struct, in which the kernel saves the hardware context whenever the process is being switched out. */
	struct thread_struct {
/* cached TLS descriptors. */
	struct desc_struct tls_array[GDT_ENTRY_TLS_ENTRIES];
	unsigned long	esp0;
	unsigned long	sysenter_cs;
	unsigned long	eip;
	unsigned long	esp;
	unsigned long	fs;
	unsigned long	gs;
/* Hardware debugging registers */
	unsigned long	debugreg[8];  /* %%db0-7 debug registers */
/* fault info */
	unsigned long	cr2, trap_no, error_code;
/* floating point info */
	union i387_union	i387;
/* virtual 86 mode info */
	struct vm86_struct __user * vm86_info;
	unsigned long		screen_bitmap;
	unsigned long		v86flags, v86mask, saved_esp0;
	unsigned int		saved_fs, saved_gs;
/* IO permissions */
	unsigned long	*io_bitmap_ptr;
/* max allowed port in the bitmap, in bytes: */
	unsigned long	io_bitmap_max;
};


	////////////////////////////////3-21//////////////////////////////////
	/******************************sched.h******************************/
/*
 * Bits in flags field of signal_struct.
 */
#define SIGNAL_STOP_STOPPED	0x00000001 /* job control stop in effect */
#define SIGNAL_STOP_DEQUEUED	0x00000002 /* stop signal dequeued */
#define SIGNAL_STOP_CONTINUED	0x00000004 /* SIGCONT since WCONTINUED reap */
#define SIGNAL_GROUP_EXIT	0x00000008 /* group exit in progress *//**
																	  ulk:If the thread group is being killed
																	  (SIGNAL_GROUP_EXIT flag in the flags field of
																	  the signal descriptor set)
																	**/

#define TASK_RUNNING		0
#define TASK_INTERRUPTIBLE	1
#define TASK_UNINTERRUPTIBLE	2
#define TASK_STOPPED		4
#define TASK_TRACED		8
#define EXIT_ZOMBIE		16
#define EXIT_DEAD		32

	/* http://www.spinics.net/lists/newbies/msg11186.html 有很多成员的注释 */
struct task_struct {
	volatile long state;	/* -1 unrunnable, 0 runnable, >0 stopped */
	struct thread_info *thread_info;
	atomic_t usage;				/* used by get_task_struct(). It's also set in kernel/fork.c. This value 
								   acts like a reference count on the task structure of a process. It can be
								   used if we don't want to hold the tasklist_lock. */
	/**
      process flag can be, for example, PF_DEAD when exit_notify() is called.
      List is of possible values is in include/linux/sched.h 
	 **/

	/*
	 * Per process flags
	 */
#define PF_ALIGNWARN	0x00000001	/* Print alignment warning msgs */
	/* Not implemented yet, only for 486*/
#define PF_STARTING	0x00000002	/* being created */
#define PF_EXITING	0x00000004	/* getting shut down */
#define PF_DEAD		0x00000008	/* Dead */
#define PF_FORKNOEXEC	0x00000040	/* forked but didn't exec */
#define PF_SUPERPRIV	0x00000100	/* used super-user privileges */
#define PF_DUMPCORE	0x00000200	/* dumped core */
#define PF_SIGNALED	0x00000400	/* killed by a signal */
#define PF_MEMALLOC	0x00000800	/* Allocating memory */
#define PF_FLUSHER	0x00001000	/* responsible for disk writeback */
#define PF_USED_MATH	0x00002000	/* if unset the fpu must be initialized before use */
#define PF_FREEZE	0x00004000	/* this task is being frozen for suspend now */
#define PF_NOFREEZE	0x00008000	/* this thread should not be frozen */
#define PF_FROZEN	0x00010000	/* frozen for system suspend */
#define PF_FSTRANS	0x00020000	/* inside a filesystem transaction */
#define PF_KSWAPD	0x00040000	/* I am kswapd */
#define PF_SWAPOFF	0x00080000	/* I am in swapoff */
#define PF_LESS_THROTTLE 0x00100000	/* Throttle me less: I clean memory */
#define PF_SYNCWRITE	0x00200000	/* I am doing a sync write */
#define PF_BORROWED_MM	0x00400000	/* I am a kthread doing use_mm */
#define PF_RANDOMIZE	0x00800000	/* randomize virtual address space */
	unsigned long flags;	/* per process flags, defined below */
	/**
	   used by ptrace a system call that provides the ability to a parent 
	   process to observe and control the execution of another process.  
	 **/
	/**
	   ulk:if it is not zero, the parent process (current) is being traced by another process, 
	 **/
	/**
	   标志在ptrace.h里
	 **/
/*
 * Ptrace flags
 */
/**
   这些是task_struct.ptrace里设置的标志
 **/
#define PT_PTRACED	0x00000001
#define PT_DTRACE	0x00000002	/* delayed trace (used on m68k, i386) */
#define PT_TRACESYSGOOD	0x00000004
#define PT_PTRACE_CAP	0x00000008	/* ptracer can follow suid-exec */
#define PT_TRACE_FORK	0x00000010
#define PT_TRACE_VFORK	0x00000020
#define PT_TRACE_CLONE	0x00000040
#define PT_TRACE_EXEC	0x00000080
#define PT_TRACE_VFORK_DONE	0x00000100
#define PT_TRACE_EXIT	0x00000200
#define PT_ATTACHED	0x00000400	/* parent != real_parent */
	unsigned long ptrace;
	int lock_depth;		/* BKL lock depth */

	int prio, static_prio;/**
							 priority of a process used when scheduled. Variable prio, which is the 
							 user-nice values, can be converted to static priority to better scale 
							 various scheduler parameters.
						   **/

	/**
	    a list of runnable task.
	 **/
	/**
	   If the process priority is equal to k (a value ranging between 0 and 139),
	   the run_list field links the process descriptor into the list of runnable processes having priority k.
	 **/
	struct list_head run_list;	/* where the process is in the run list. */
	
	/**
struct prio_array {
	unsigned int nr_active;
	unsigned long bitmap[BITMAP_SIZE];
	struct list_head queue[MAX_PRIO];
};
	**/
	/**
	   因为每一个CPU有一个prio_array_t，array成员只是指向了其中的一个。
	 **/
	prio_array_t *array;/**
						   a pointer to a priority array. 
						 **/

	unsigned long sleep_avg;/**
							   average sleep time of the process
							 **/
	unsigned long long timestamp, last_ran;/**
											  timestamp:keep the time when the process has been activating. It is used, for 
											  example, to recalculate the task's priority.
											**/
	unsigned long long sched_time; /* sched_clock time spent running */
	int activated;

	unsigned long policy;/**
							the scheduling policy used for this process. It can be SCHED_NORMAL, 
							SCHED_FIFO or SCHED_RR
						  **/
	cpumask_t cpus_allowed;/**
							  mask that indicates on what CPUs the process can run.
							**/
	unsigned int time_slice, first_time_slice;/**
												 time during when the process can run.
											   **/

#ifdef CONFIG_SCHEDSTATS
	struct sched_info sched_info;
#endif

	struct list_head tasks;		/**
								   process list
								*/
	/*
	 * ptrace_list/ptrace_children forms the list of my children
	 * that were stolen by a ptracer.
	 */
	struct list_head ptrace_children; /**
									   The head of a list containing all
									   children of P being traced by a debugger
									  */
	struct list_head ptrace_list; /**
								   The pointers to the next and previous
								   elements in the real parent's list of
								   traced processes (used when P is being
								   traced)
								  */

	struct mm_struct *mm, *active_mm;/**
										process address space describes by mm_struct. Field active_mm points to 
										the active address space if the process doesn't have real one (eg kernel 
										threads).
									  **/

/* task state */
	struct linux_binfmt *binfmt;/**
								   allows to define functions that are used to load the binary formats that 
								   linux accepts.
								 **/
	long exit_state;			/* tsk->exit_state can only be 0, EXIT_ZOMBIE, or EXIT_DEAD */
	/**
      holds code or signal when a process exited. 
      code: SIGHUP, SIGINT, SIGQUIT, ...
      signal: generally used with SIGCHLD to signal init on exit
	**/
	/* blog.csdn.net/dog250/article/details/5303673 */
	int exit_code, exit_signal;	/* 把task_struct.exit_code(任务退出代码)置为exit()提供的代码code(退出代码存放在task_struct.exit_code中以供父进程随时检索) */
	int pdeath_signal;  /*  The signal sent when the parent dies  */
	/* ??? */
	unsigned long personality;/**
								 relates to the personality of the task, i.e. to the way certain system 
								 calls behave in order to emulate the "personality" of foreign flavors of
								 UNIX.
							   **/
	/**
	   set to 1 when executing a new program using sys_execve() and searching 
	   the correct binary formats handler
	 **/
	unsigned did_exec:1;		/* 按POSIX要求设计的布尔量，
								   区分进程是正在执行老程序代码，
								   还是在执行execve装入的新代码 */
	pid_t pid;					/* 是一个int类型，指明在哪个位 */
	pid_t tgid;					/**
								   PID of the thread group leader of P
								 */
	/* 
	 * pointers to (original) parent process, youngest child, younger sibling,
	 * older sibling, respectively.  (p->father can be replaced with 
	 * p->parent->pid)
	 */
	struct task_struct *real_parent; /* real parent process (when being debugged) */
	struct task_struct *parent;	/* parent process */
	/*
	 * children/sibling forms the list of my children plus the
	 * tasks I'm ptracing.
	 */
	struct list_head children;	/* list of my children */
	struct list_head sibling;	/* linkage in my parent's children list */
	struct task_struct *group_leader;	/* threadgroup leader */

	/* PID/PID hash table linkage. */
	struct pid pids[PIDTYPE_MAX];

	struct completion *vfork_done;		/* for vfork(),
										   在do_fork中置1 
										 */
	int __user *set_child_tid;		/* CLONE_CHILD_SETTID */
	int __user *clear_child_tid;		/* CLONE_CHILD_CLEARTID */

	unsigned long rt_priority;	/**
								   real time priority
								 **/
	cputime_t utime, stime;/**
							  utime = user time,
							  stime = system time,
							  cutime = cumulative user time (process + its children),
							  cstime = cumulative system time (process + its children)
						   **/
	unsigned long nvcsw, nivcsw; /* context switch counts */
	struct timespec start_time;/**
								  value of the jiffies when the task was created
								**/
	/* mm fault and swap info: this can arguably be seen as either mm-specific or thread-specific */
	unsigned long min_flt, maj_flt;/**
									  min_flt: minor fault, 
									  maj_flt: major fault (means that it had access to the disk),
									  cmin_flt: cumulative minor fault (process + its children),
									  cmaj_flt: cumulative major fault (process + its children)
									**/

  	cputime_t it_prof_expires, it_virt_expires;
	unsigned long long it_sched_expires;
	struct list_head cpu_timers[3];

/* process credentials */
	uid_t uid,euid,suid,fsuid;
	gid_t gid,egid,sgid,fsgid;
	struct group_info *group_info;
	kernel_cap_t   cap_effective, cap_inheritable, cap_permitted;
	unsigned keep_capabilities:1;
	struct user_struct *user;
#ifdef CONFIG_KEYS
	struct key *thread_keyring;	/* keyring private to this thread */
#endif
	int oomkilladj; /* OOM kill score adjustment (bit shift). */
	char comm[TASK_COMM_LEN]; /* executable name excluding path
				     - access with [gs]et_task_comm (which lock
				       it with task_lock())
				     - initialized normally by flush_old_exec */
/* file system info */
	int link_count, total_link_count;
/* ipc stuff */
	struct sysv_sem sysvsem;
/* CPU-specific state of this task */
	struct thread_struct thread;
/* filesystem information */
	struct fs_struct *fs;
/* open file information */
	struct files_struct *files;
/* namespace */
	struct namespace *namespace;
/* signal handlers */
	struct signal_struct *signal;
	struct sighand_struct *sighand;

	sigset_t blocked, real_blocked;
	struct sigpending pending;

	unsigned long sas_ss_sp;
	size_t sas_ss_size;
	int (*notifier)(void *priv);
	void *notifier_data;
	sigset_t *notifier_mask;
	
	void *security;
	struct audit_context *audit_context;
	seccomp_t seccomp;

/* Thread group tracking */
   	u32 parent_exec_id;
   	u32 self_exec_id;
/* Protection of (de-)allocation: mm, files, fs, tty, keyrings */
	spinlock_t alloc_lock;
/* Protection of proc_dentry: nesting proc_lock, dcache_lock, write_lock_irq(&tasklist_lock); */
	spinlock_t proc_lock;
/* context-switch lock */
	spinlock_t switch_lock;

/* journalling filesystem info */
	void *journal_info;

/* VM state */
	struct reclaim_state *reclaim_state;/**
										   pointer to structure reclaim_state when a task is running system's page 
										   release (kmem_freepages).
										 **/
	struct dentry *proc_dentry;
	struct backing_dev_info *backing_dev_info;

	struct io_context *io_context;

	unsigned long ptrace_message;/**
									若创建的子进程被跟踪,用在调用do_fork里ptrace_message被存放子进程的pid
								  **/
	siginfo_t *last_siginfo; /* For ptrace use.  */
/*
 * current io wait handle: wait queue entry to use for io waits
 * If this thread is processing aio, this points at the waitqueue
 * inside the currently handled kiocb. It may be NULL (i.e. default
 * to a stack based synchronous wait) if its doing sync IO.
 */
	wait_queue_t *io_wait;
/* i/o counters(bytes read/written, #syscalls */
	u64 rchar, wchar, syscr, syscw;
#if defined(CONFIG_BSD_PROCESS_ACCT)
	u64 acct_rss_mem1;	/* accumulated rss usage */
	u64 acct_vm_mem1;	/* accumulated virtual memory usage */
	clock_t acct_stimexpd;	/* clock_t-converted stime since last update */
#endif
#ifdef CONFIG_NUMA
  	struct mempolicy *mempolicy;
	short il_next;
#endif
#ifdef CONFIG_CPUSETS
	struct cpuset *cpuset;
	nodemask_t mems_allowed;
	int cpuset_mems_generation;
#endif
};

union thread_union {
	struct thread_info thread_info;
	unsigned long stack[THREAD_SIZE/sizeof(long)];
};

/*
 * Per process flags
 */
#define PF_ALIGNWARN	0x00000001	/* Print alignment warning msgs */
					/* Not implemented yet, only for 486*/
#define PF_STARTING	0x00000002	/* being created */
#define PF_EXITING	0x00000004	/* getting shut down */
#define PF_DEAD		0x00000008	/* Dead */
#define PF_FORKNOEXEC	0x00000040	/* forked but didn't exec */
#define PF_SUPERPRIV	0x00000100	/* used super-user privileges */
#define PF_DUMPCORE	0x00000200	/* dumped core */
#define PF_SIGNALED	0x00000400	/* killed by a signal */
#define PF_MEMALLOC	0x00000800	/* Allocating memory */
#define PF_FLUSHER	0x00001000	/* responsible for disk writeback */
#define PF_USED_MATH	0x00002000	/* if unset the fpu must be initialized before use */
#define PF_FREEZE	0x00004000	/* this task is being frozen for suspend now */
#define PF_NOFREEZE	0x00008000	/* this thread should not be frozen */
#define PF_FROZEN	0x00010000	/* frozen for system suspend */
#define PF_FSTRANS	0x00020000	/* inside a filesystem transaction */
#define PF_KSWAPD	0x00040000	/* I am kswapd */
#define PF_SWAPOFF	0x00080000	/* I am in swapoff */
#define PF_LESS_THROTTLE 0x00100000	/* Throttle me less: I clean memory */
#define PF_SYNCWRITE	0x00200000	/* I am doing a sync write */
#define PF_BORROWED_MM	0x00400000	/* I am a kthread doing use_mm */
#define PF_RANDOMIZE	0x00800000	/* randomize virtual address space */

	/******************************kernel/pid.c******************************/
#define PIDMAP_ENTRIES		((PID_MAX_LIMIT + 8*PAGE_SIZE - 1)/PAGE_SIZE/8)
	static pidmap_t pidmap_array[PIDMAP_ENTRIES] =
	 { [ 0 ... PIDMAP_ENTRIES-1 ] = { ATOMIC_INIT(BITS_PER_PAGE), NULL } };

	/******************************include/asm-i386/thread_info.h************/
#ifdef CONFIG_4KSTACKS
#define THREAD_SIZE            (4096)
#else
#define THREAD_SIZE		(8192)
#endif

struct thread_info {
	struct task_struct	*task;		/* main task structure */
	struct exec_domain	*exec_domain;	/* execution domain */
	unsigned long		flags;		/* low level flags */
	unsigned long		status;		/* thread-synchronous flags */
	__u32			cpu;		/* current CPU */
	__s32			preempt_count; /* 0 => preemptable, <0 => BUG */


	mm_segment_t		addr_limit;	/* thread address space:
					 	   0-0xBFFFFFFF for user-thead
						   0-0xFFFFFFFF for kernel-thread
						*/
	struct restart_block    restart_block;

	unsigned long           previous_esp;   /* ESP of the previous stack in case
						   of nested (IRQ) stacks
						*/
	__u8			supervisor_stack[0];
};

/*
 * thread information flags
 * - these are process state flags that various assembly files may need to access
 * - pending work-to-be-done flags are in LSW
 * - other flags in MSW
 */
#define TIF_SYSCALL_TRACE	0	/* syscall trace active */
#define TIF_NOTIFY_RESUME	1	/* resumption notification requested */
#define TIF_SIGPENDING		2	/* signal pending */
#define TIF_NEED_RESCHED	3	/* rescheduling necessary */
#define TIF_SINGLESTEP		4	/* restore singlestep on return to user mode */
#define TIF_IRET		5	/* return with iret */
#define TIF_SYSCALL_AUDIT	7	/* syscall auditing active */
#define TIF_SECCOMP		8	/* secure computing */
#define TIF_POLLING_NRFLAG	16	/* true if poll_idle() is polling TIF_NEED_RESCHED */
#define TIF_MEMDIE		17

    /******************************sched.c******************************/

	struct prio_array {
		unsigned int nr_active;
		unsigned long bitmap[BITMAP_SIZE];
		struct list_head queue[MAX_PRIO];
	};

/*
 * This is the main, per-CPU runqueue data structure.
 *
 * Locking rule: those places that want to lock multiple runqueues
 * (such as the load balancing or the thread migration code), lock
 * acquire operations must be ordered by ascending &runqueue.
 */
struct runqueue {
	spinlock_t lock;

	/*
	 * nr_running and cpu_load should be in the same cacheline because
	 * remote CPUs use both these fields when doing load calculation.
	 */
	unsigned long nr_running;
#ifdef CONFIG_SMP
	unsigned long cpu_load;
#endif
	unsigned long long nr_switches;

	/*
	 * This is part of a global counter where only the total sum
	 * over all CPUs matters. A task can increase this counter on
	 * one CPU and if it got migrated afterwards it may decrease
	 * it on another CPU. Always updated under the runqueue lock:
	 */
	unsigned long nr_uninterruptible;

	unsigned long expired_timestamp;
	unsigned long long timestamp_last_tick; /**
											  Timestamp value of the last
											  timer interrupt
											*/
	task_t *curr, *idle;		/**
								   Process descriptor pointer of the swapper
								   process for this CPU
								 */
	struct mm_struct *prev_mm;
	prio_array_t *active, *expired, arrays[2];
	int best_expired_prio;
	atomic_t nr_iowait;

#ifdef CONFIG_SMP
	struct sched_domain *sd;

	/* For active balancing */
	int active_balance;
	int push_cpu;

	task_t *migration_thread;
	struct list_head migration_queue;
#endif

#ifdef CONFIG_SCHEDSTATS
	/* latency stats */
	struct sched_info rq_sched_info;

	/* sys_sched_yield() stats */
	unsigned long yld_exp_empty;
	unsigned long yld_act_empty;
	unsigned long yld_both_empty;
	unsigned long yld_cnt;

	/* schedule() stats */
	unsigned long sched_switch;
	unsigned long sched_cnt;
	unsigned long sched_goidle;

	/* try_to_wake_up() stats */
	unsigned long ttwu_cnt;
	unsigned long ttwu_local;
#endif
};


    /******************************pid.c******************************/
	static struct hlist_head *pid_hash[PIDTYPE_MAX];

    /******************************pid.h******************************/
	struct pid
	{
		/* Try to keep pid_chain in the same cacheline as nr for find_pid */
		int nr;
		struct hlist_node pid_chain;
		/* list of pids with the same nr, only one of them is in the hash */
		struct list_head pid_list;
	};

	/******************************include/wait.h******************************/
    struct __wait_queue_head {
        spinlock_t lock;
        struct list_head task_list;
    };
	typedef struct __wait_queue_head wait_queue_head_t;
	
	struct __wait_queue {
		unsigned int flags;
#define WQ_FLAG_EXCLUSIVE	0x01
		struct task_struct * task;
		wait_queue_func_t func;
		struct list_head task_list;
	};

#define __WAIT_QUEUE_HEAD_INITIALIZER(name) {				\
	.lock		= SPIN_LOCK_UNLOCKED,				\
	.task_list	= { &(name).task_list, &(name).task_list } }

#define DECLARE_WAIT_QUEUE_HEAD(name) \
	wait_queue_head_t name = __WAIT_QUEUE_HEAD_INITIALIZER(name)


/******************************include/asm-i386/processor.h******************************/
struct thread_struct {
/* cached TLS descriptors. */
	struct desc_struct tls_array[GDT_ENTRY_TLS_ENTRIES];
	unsigned long	esp0;
	unsigned long	sysenter_cs;
	unsigned long	eip;
	unsigned long	esp;
	unsigned long	fs;
	unsigned long	gs;
/* Hardware debugging registers */
	unsigned long	debugreg[8];  /* %%db0-7 debug registers */
/* fault info */
	unsigned long	cr2, trap_no, error_code;
/* floating point info */
	union i387_union	i387;
/* virtual 86 mode info */
	struct vm86_struct __user * vm86_info;
	unsigned long		screen_bitmap;
	unsigned long		v86flags, v86mask, saved_esp0;
	unsigned int		saved_fs, saved_gs;
/* IO permissions */
	unsigned long	*io_bitmap_ptr;
/* max allowed port in the bitmap, in bytes: */
	unsigned long	io_bitmap_max;
};
	/******************************include/linux/ptrace.h******************************/
/*
 * Ptrace flags
 */
/**
   这些是task_struct.ptrace里设置的标志
 **/
#define PT_PTRACED	0x00000001
#define PT_DTRACE	0x00000002	/* delayed trace (used on m68k, i386) */
#define PT_TRACESYSGOOD	0x00000004
#define PT_PTRACE_CAP	0x00000008	/* ptracer can follow suid-exec */
#define PT_TRACE_FORK	0x00000010
#define PT_TRACE_VFORK	0x00000020
#define PT_TRACE_CLONE	0x00000040
#define PT_TRACE_EXEC	0x00000080
#define PT_TRACE_VFORK_DONE	0x00000100
#define PT_TRACE_EXIT	0x00000200
#define PT_ATTACHED	0x00000400	/* parent != real_parent */

	/******************************include/linux/irq.h******************************/
	
/*
 * IRQ line status.
 */
#define IRQ_INPROGRESS	1	/* IRQ handler active - do not enter! */
#define IRQ_DISABLED	2	/* IRQ disabled - do not enter! */
#define IRQ_PENDING	4	/* IRQ pending - replay on enable */
#define IRQ_REPLAY	8	/* IRQ has been replayed but not acked yet */
#define IRQ_AUTODETECT	16	/* IRQ is being autodetected */
#define IRQ_WAITING	32	/* IRQ not yet seen - for autodetection */
#define IRQ_LEVEL	64	/* IRQ level triggered */
#define IRQ_MASKED	128	/* IRQ masked - shouldn't be seen again */
#define IRQ_PER_CPU	256	/* IRQ is per CPU */

/*
 * Interrupt controller descriptor. This is all we need
 * to describe about the low-level hardware. 
 */
struct hw_interrupt_type {
	const char * typename;
	unsigned int (*startup)(unsigned int irq);
	void (*shutdown)(unsigned int irq);
	void (*enable)(unsigned int irq);
	void (*disable)(unsigned int irq);
	void (*ack)(unsigned int irq);
	void (*end)(unsigned int irq);
	void (*set_affinity)(unsigned int irq, cpumask_t dest);
};

typedef struct hw_interrupt_type  hw_irq_controller;

/*
 * This is the "IRQ descriptor", which contains various information
 * about the irq, including what kind of hardware handling it has,
 * whether it is disabled etc etc.
 *
 * Pad this out to 32 bytes for cache and indexing reasons.
 */
typedef struct irq_desc {
	hw_irq_controller *handler;
	void *handler_data;
	struct irqaction *action;	/* IRQ action list */
	unsigned int status;		/* IRQ status */
	unsigned int depth;		/* nested irq disables */
	unsigned int irq_count;		/* For detecting broken interrupts */
	unsigned int irqs_unhandled;
	spinlock_t lock;
} ____cacheline_aligned irq_desc_t;
extern irq_desc_t irq_desc [NR_IRQS];

	/******************************include/linux/interrupt.h******************************/
struct irqaction {
	irqreturn_t (*handler)(int, void *, struct pt_regs *);
	unsigned long flags;
	cpumask_t mask;
	const char *name;
	void *dev_id;
	struct irqaction *next;
	int irq;
	struct proc_dir_entry *dir;
};

/* PLEASE, avoid to allocate new softirqs, if you need not _really_ high
   frequency threaded job For almost all the purposes
   tasklets are more than enough. F.e. all serial device BHs et
   al. should be converted to tasklets, not to softirqs.
 */

enum
{
	HI_SOFTIRQ=0,
	TIMER_SOFTIRQ,
	NET_TX_SOFTIRQ,
	NET_RX_SOFTIRQ,
	SCSI_SOFTIRQ,
	TASKLET_SOFTIRQ
};


/* softirq mask and active fields moved to irq_cpustat_t in
 * asm/hardirq.h to get better cache usage.  KAO
 */

struct softirq_action
{
	void	(*action)(struct softirq_action *);
	void	*data;
};
 

/* Tasklets --- multithreaded analogue of BHs.

   Main feature differing them of generic softirqs: tasklet
   is running only on one CPU simultaneously.

   Main feature differing them of BHs: different tasklets
   may be run simultaneously on different CPUs.

   Properties:
   * If tasklet_schedule() is called, then tasklet is guaranteed
     to be executed on some cpu at least once after this.
   * If the tasklet is already scheduled, but its excecution is still not
     started, it will be executed only once.
   * If this tasklet is already running on another CPU (or schedule is called
     from tasklet itself), it is rescheduled for later.
   * Tasklet is strictly serialized wrt itself, but not
     wrt another tasklets. If client needs some intertask synchronization,
     he makes it with spinlocks.
 */

struct tasklet_struct
{
	struct tasklet_struct *next;
	unsigned long state;
	atomic_t count;
	void (*func)(unsigned long);
	unsigned long data;
};

    /******************************include/linux/signal.h******************************/
#define SA_PROBE		SA_ONESHOT
#define SA_SAMPLE_RANDOM	SA_RESTART
#define SA_SHIRQ		0x04000000

    /******************************include/asm-i386/hardirq.h******************************/
typedef struct {
	unsigned int __softirq_pending;
	unsigned long idle_timestamp;
	unsigned int __nmi_count;	/* arch dependent */
	unsigned int apic_timer_irqs;	/* arch dependent */
} ____cacheline_aligned irq_cpustat_t;

    /******************************kernel/softirq.c******************************/
irq_cpustat_t irq_stat[NR_CPUS] ____cacheline_aligned;
static struct softirq_action softirq_vec[32] __cacheline_aligned_in_smp;

/* Tasklets */
struct tasklet_head
{
	struct tasklet_struct *list;
};

/* Some compilers disobey section attribute on statics when not
   initialized -- RR */
static DEFINE_PER_CPU(struct tasklet_head, tasklet_vec) = { NULL };
static DEFINE_PER_CPU(struct tasklet_head, tasklet_hi_vec) = { NULL };

    /******************************kernel/irq.c******************************/
static char softirq_stack[NR_CPUS * THREAD_SIZE]
		__attribute__((__aligned__(THREAD_SIZE)));

static char hardirq_stack[NR_CPUS * THREAD_SIZE]
		__attribute__((__aligned__(THREAD_SIZE)));

    /******************************include/linux/kernel_stat.h******************************/
struct kernel_stat {
	struct cpu_usage_stat	cpustat;
	unsigned int irqs[NR_IRQS];
};

DECLARE_PER_CPU(struct kernel_stat, kstat);

        /******************************kernel/irq/handle.h******************************/
struct hw_interrupt_type no_irq_type = {
	.typename = 	"none",
	.startup = 	startup_none,
	.shutdown = 	shutdown_none,
	.enable = 	enable_none,
	.disable = 	disable_none,
	.ack = 		ack_none,
	.end = 		end_none,
	.set_affinity = NULL
};

/******************************kernel/workqueue.c******************************/
/*
 * The per-CPU workqueue (if single thread, we always use cpu 0's).
 *
 * The sequence counters are for flush_scheduled_work().  It wants to wait
 * until until all currently-scheduled works are completed, but it doesn't
 * want to be livelocked by new, incoming ones.  So it waits until
 * remove_sequence is >= the insert_sequence which pertained when
 * flush_scheduled_work() was called.
 */
struct cpu_workqueue_struct {

	spinlock_t lock;

	long remove_sequence;	/* Least-recently added (next to run) */
	long insert_sequence;	/* Next to add */

	struct list_head worklist;
	wait_queue_head_t more_work;
	wait_queue_head_t work_done;

	struct workqueue_struct *wq;
	task_t *thread;

	int run_depth;		/* Detect run_workqueue() recursion depth */
} ____cacheline_aligned;

/*
 * The externally visible workqueue abstraction is an array of
 * per-CPU workqueues:
 */
struct workqueue_struct {
	struct cpu_workqueue_struct cpu_wq[NR_CPUS];
	const char *name;
	struct list_head list; 	/* Empty if single thread */
};

static struct workqueue_struct *keventd_wq;

/******************************include/linux/workqueue.h******************************/
struct work_struct {
	unsigned long pending;
	struct list_head entry;
	void (*func)(void *);
	void *data;
	void *wq_data;
	struct timer_list timer;
};

/******************************include/linux/workqueue.h******************************/
#define create_singlethread_workqueue(name) __create_workqueue((name), 1)
#define create_workqueue(name) __create_workqueue((name), 0)

/******************************include/linux/percpu.h******************************/
#ifdef CONFIG_SMP
struct percpu_data {
	void *ptrs[NR_CPUS];
	void *blkp;
};

/******************************include/asm-i386/atomic.h******************************/
/*
 * Make sure gcc doesn't try to be clever and move things around
 * on us. We need to use _exactly_ the address the user gave us,
 * not some alias that contains the same information.
 */
typedef struct { volatile int counter; } atomic_t;

/******************************include/asm-i386/spinlock.h******************************/
typedef struct {
	volatile unsigned int slock;
#ifdef CONFIG_DEBUG_SPINLOCK
	unsigned magic;
#endif
#ifdef CONFIG_PREEMPT
	unsigned int break_lock;
#endif
} spinlock_t;

#define SPINLOCK_MAGIC	0xdead4ead

#ifdef CONFIG_DEBUG_SPINLOCK
#define SPINLOCK_MAGIC_INIT	, SPINLOCK_MAGIC
#else
#define SPINLOCK_MAGIC_INIT	/* */
#endif
#define SPIN_LOCK_UNLOCKED (spinlock_t) { 1 SPINLOCK_MAGIC_INIT }

/*
 * Read-write spinlocks, allowing multiple readers
 * but only one writer.
 *
 * NOTE! it is quite common to have readers in interrupts
 * but no interrupt writers. For those circumstances we
 * can "mix" irq-safe locks - any writer needs to get a
 * irq-safe write-lock, but readers can get non-irqsafe
 * read-locks.
 */
typedef struct {
	volatile unsigned int lock;
#ifdef CONFIG_DEBUG_SPINLOCK
	unsigned magic;
#endif
#ifdef CONFIG_PREEMPT
	unsigned int break_lock;
#endif
} rwlock_t;

/******************************include/linux/seqlock.h******************************/
typedef struct {
	unsigned sequence;
	spinlock_t lock;
} seqlock_t;

/******************************include/linux/rcupdate.h******************************/
/**
 * struct rcu_head - callback structure for use with RCU
 * @next: next update requests in a list
 * @func: actual update function to call after the grace period.
 */
struct rcu_head {
	struct rcu_head *next;
	void (*func)(struct rcu_head *head);
};


/*
 * Per-CPU data for Read-Copy UPdate.
 * nxtlist - new callbacks are added here
 * curlist - current batch for which quiescent cycle started if any
 */
struct rcu_data {
	/* 1) quiescent state handling : */
	long		quiescbatch;     /* Batch # for grace period */
	int		passed_quiesc;	 /* User-mode/idle loop etc. */
	int		qs_pending;	 /* core waits for quiesc state */

	/* 2) batch handling */
	long  	       	batch;           /* Batch # for current RCU batch */
	struct rcu_head *nxtlist;
	struct rcu_head **nxttail;
	struct rcu_head *curlist;
	struct rcu_head **curtail;
	struct rcu_head *donelist;
	struct rcu_head **donetail;
	int cpu;
};

/******************************include/asm-i386/semaphore.h******************************/

struct semaphore {
	atomic_t count;
	int sleepers;
	wait_queue_head_t wait;
};

/******************************include/asm-i386/timer.h******************************/
/**
 * struct timer_ops - used to define a timer source
 *
 * @name: name of the timer.
 * @init: Probes and initializes the timer. Takes clock= override 
 *        string as an argument. Returns 0 on success, anything else
 *        on failure.
 * @mark_offset: called by the timer interrupt.
 * @get_offset:  called by gettimeofday(). Returns the number of microseconds
 *               since the last timer interupt.
 * @monotonic_clock: returns the number of nanoseconds since the init of the
 *                   timer.
 * @delay: delays this many clock cycles.
 */
struct timer_opts {
	char* name;
	void (*mark_offset)(void);
	unsigned long (*get_offset)(void);
	unsigned long long (*monotonic_clock)(void);
	void (*delay)(unsigned long);
};

/******************************kernel/timer.c******************************/
/* 
 * The current time 
 * wall_to_monotonic is what we need to add to xtime (or xtime corrected 
 * for sub jiffie times) to get to monotonic time.  Monotonic is pegged
 * at zero at system boot time, so wall_to_monotonic will be negative,
 * however, we will ALWAYS keep the tv_nsec part positive so we can use
 * the usual normalization.
 */
struct timespec xtime __attribute__ ((aligned (16)));
struct timespec wall_to_monotonic __attribute__ ((aligned (16)));


/******************************include/asm-i386/rwsem.h******************************/

/*
 * the semaphore definition
 */
struct rw_semaphore {
	signed long		count;
#define RWSEM_UNLOCKED_VALUE		0x00000000
#define RWSEM_ACTIVE_BIAS		0x00000001
#define RWSEM_ACTIVE_MASK		0x0000ffff
#define RWSEM_WAITING_BIAS		(-0x00010000)
#define RWSEM_ACTIVE_READ_BIAS		RWSEM_ACTIVE_BIAS
#define RWSEM_ACTIVE_WRITE_BIAS		(RWSEM_WAITING_BIAS + RWSEM_ACTIVE_BIAS)
	spinlock_t		wait_lock;
	struct list_head	wait_list;
#if RWSEM_DEBUG
	int			debug;
#endif
};
/******************************include/linux/rwsem-spinlock.h******************************/

/*
 * the rw-semaphore definition
 * - if activity is 0 then there are no active readers or writers
 * - if activity is +ve then that is the number of active readers
 * - if activity is -1 then there is one active writer
 * - if wait_list is not empty, then there are processes waiting for the semaphore
 */
struct rw_semaphore {
	__s32			activity;
	spinlock_t		wait_lock;
	struct list_head	wait_list;
#if RWSEM_DEBUG
	int			debug;
#endif
};

/******************************include/linux/completion.h******************************/
struct completion {
	unsigned int done;
	wait_queue_head_t wait;
};

/******************************include/linux/kernel_stat.h******************************/

/*
 * 'kernel_stat.h' contains the definitions needed for doing
 * some kernel statistics (CPU usage, context switches ...),
 * used by rstatd/perfmeter
 */
/**
   里面的大部分成员都是在account_system_time(),account_user_time()里更新的。
 **/
struct cpu_usage_stat {
	cputime64_t user;
	cputime64_t nice;
	cputime64_t system;
	cputime64_t softirq;
	cputime64_t irq;
	cputime64_t idle;
	cputime64_t iowait;
	cputime64_t steal;
};

struct kernel_stat {
	struct cpu_usage_stat	cpustat;
	unsigned int irqs[NR_IRQS];
};

DECLARE_PER_CPU(struct kernel_stat, kstat);

#define kstat_cpu(cpu)	per_cpu(kstat, cpu)
/* Must have preemption disabled for this to be meaningful. */
#define kstat_this_cpu	__get_cpu_var(kstat)

/******************************include/linux/timer.h******************************/

struct timespec {
	time_t	tv_sec;		/* seconds */
	long	tv_nsec;	/* nanoseconds */
};

struct timer_list {
	struct list_head entry;
	unsigned long expires;

	spinlock_t lock;
	unsigned long magic;

	void (*function)(unsigned long);
	unsigned long data;

	struct tvec_t_base_s *base;
};

/******************************kernel/timer.c******************************/

/*
 * per-CPU timer vector definitions:
 */

#define TVN_BITS (CONFIG_BASE_SMALL ? 4 : 6)
#define TVR_BITS (CONFIG_BASE_SMALL ? 6 : 8)
#define TVN_SIZE (1 << TVN_BITS)
#define TVR_SIZE (1 << TVR_BITS)
#define TVN_MASK (TVN_SIZE - 1)
#define TVR_MASK (TVR_SIZE - 1)

typedef struct tvec_s {
	struct list_head vec[TVN_SIZE];
} tvec_t;

typedef struct tvec_root_s {
	struct list_head vec[TVR_SIZE];
} tvec_root_t;

struct tvec_t_base_s {
	spinlock_t lock;
	unsigned long timer_jiffies;
	struct timer_list *running_timer;
	tvec_root_t tv1;
	tvec_t tv2;
	tvec_t tv3;
	tvec_t tv4;
	tvec_t tv5;
} ____cacheline_aligned_in_smp;

typedef struct tvec_t_base_s tvec_base_t;

/******************************include/linux/thread_info.h******************************/
/*
 * System call restart block. 
 */
struct restart_block {
	long (*fn)(struct restart_block *);
	unsigned long arg0, arg1, arg2, arg3;
};

/******************************include/linux/time.h******************************/
struct timezone {
	int	tz_minuteswest;	/* minutes west of Greenwich */
	int	tz_dsttime;	/* type of dst correction */
};

/******************************mm/memory.c******************************/
struct page *mem_map;

/******************************include/asm-i386/page.h******************************/
#define virt_to_page(kaddr)	pfn_to_page(__pa(kaddr) >> PAGE_SHIFT)

#define pfn_to_page(pfn)	(mem_map + (pfn))

/******************************include/asm-i386/mmzone.h******************************/
#define pfn_to_page(pfn)						\
({									\
	unsigned long __pfn = pfn;					\
	int __node  = pfn_to_nid(__pfn);				\
	&node_mem_map(__node)[node_localnr(__pfn,__node)];		\
})

typedef struct pglist_data {
	struct zone node_zones[MAX_NR_ZONES];
	struct zonelist node_zonelists[GFP_ZONETYPES];
	int nr_zones;
	struct page *node_mem_map;
	struct bootmem_data *bdata;
	unsigned long node_start_pfn;
	unsigned long node_present_pages; /* total number of physical pages */
	unsigned long node_spanned_pages; /* total size of physical page
					     range, including holes */
	int node_id;
	struct pglist_data *pgdat_next;
	wait_queue_head_t kswapd_wait;
	struct task_struct *kswapd;
	int kswapd_max_order;
} pg_data_t;

/*
 * On machines where it is needed (eg PCs) we divide physical memory
 * into multiple physical zones. On a PC we have 3 zones:
 *
 * ZONE_DMA	  < 16 MB	ISA DMA capable memory
 * ZONE_NORMAL	16-896 MB	direct mapped by the kernel
 * ZONE_HIGHMEM	 > 896 MB	only page cache and user processes
 */

struct zone {
	/* Fields commonly accessed by the page allocator */
	unsigned long		free_pages;
	unsigned long		pages_min, pages_low, pages_high;
	/*
	 * We don't know if the memory that we're going to allocate will be freeable
	 * or/and it will be released eventually, so to avoid totally wasting several
	 * GB of ram we must reserve some of the lower zone memory (otherwise we risk
	 * to run OOM on the lower zones despite there's tons of freeable ram
	 * on the higher zones). This array is recalculated at runtime if the
	 * sysctl_lowmem_reserve_ratio sysctl changes.
	 */
	unsigned long		lowmem_reserve[MAX_NR_ZONES];

	struct per_cpu_pageset	pageset[NR_CPUS];

	/*
	 * free areas of different sizes
	 */
	spinlock_t		lock;
	struct free_area	free_area[MAX_ORDER];


	ZONE_PADDING(_pad1_)

	/* Fields commonly accessed by the page reclaim scanner */
	spinlock_t		lru_lock;	
	struct list_head	active_list;
	struct list_head	inactive_list;
	unsigned long		nr_scan_active;
	unsigned long		nr_scan_inactive;
	unsigned long		nr_active;
	unsigned long		nr_inactive;
	unsigned long		pages_scanned;	   /* since last reclaim */
	int			all_unreclaimable; /* All pages pinned */

	/*
	 * prev_priority holds the scanning priority for this zone.  It is
	 * defined as the scanning priority at which we achieved our reclaim
	 * target at the previous try_to_free_pages() or balance_pgdat()
	 * invokation.
	 *
	 * We use prev_priority as a measure of how much stress page reclaim is
	 * under - it drives the swappiness decision: whether to unmap mapped
	 * pages.
	 *
	 * temp_priority is used to remember the scanning priority at which
	 * this zone was successfully refilled to free_pages == pages_high.
	 *
	 * Access to both these fields is quite racy even on uniprocessor.  But
	 * it is expected to average out OK.
	 */
	int temp_priority;
	int prev_priority;


	ZONE_PADDING(_pad2_)
	/* Rarely used or read-mostly fields */

	/*
	 * wait_table		-- the array holding the hash table
	 * wait_table_size	-- the size of the hash table array
	 * wait_table_bits	-- wait_table_size == (1 << wait_table_bits)
	 *
	 * The purpose of all these is to keep track of the people
	 * waiting for a page to become available and make them
	 * runnable again when possible. The trouble is that this
	 * consumes a lot of space, especially when so few things
	 * wait on pages at a given time. So instead of using
	 * per-page waitqueues, we use a waitqueue hash table.
	 *
	 * The bucket discipline is to sleep on the same queue when
	 * colliding and wake all in that wait queue when removing.
	 * When something wakes, it must check to be sure its page is
	 * truly available, a la thundering herd. The cost of a
	 * collision is great, but given the expected load of the
	 * table, they should be so rare as to be outweighed by the
	 * benefits from the saved space.
	 *
	 * __wait_on_page_locked() and unlock_page() in mm/filemap.c, are the
	 * primary users of these fields, and in mm/page_alloc.c
	 * free_area_init_core() performs the initialization of them.
	 */
	wait_queue_head_t	* wait_table;
	unsigned long		wait_table_size;
	unsigned long		wait_table_bits;

	/*
	 * Discontig memory support fields.
	 */
	struct pglist_data	*zone_pgdat;
	struct page		*zone_mem_map;
	/* zone_start_pfn == zone_start_paddr >> PAGE_SHIFT */
	unsigned long		zone_start_pfn;

	unsigned long		spanned_pages;	/* total size, including holes */
	unsigned long		present_pages;	/* amount of memory (excluding holes) */

	/*
	 * rarely used fields:
	 */
	char			*name;
} ____cacheline_maxaligned_in_smp;


/******************************include/linux/mm.h******************************/

/*
 * Each physical page in the system has a struct page associated with
 * it to keep track of whatever it is we are using the page for at the
 * moment. Note that we have no way to track which tasks are using
 * a page.
 */
struct page {
	page_flags_t flags;		/* Atomic flags, some possibly
					 * updated asynchronously */
	atomic_t _count;		/* Usage count, see below. */
	atomic_t _mapcount;		/* Count of ptes mapped in mms,
					 * to show when page is mapped
					 * & limit reverse map searches.
					 */
	unsigned long private;		/* Mapping-private opaque data:
					 * usually used for buffer_heads
					 * if PagePrivate set; used for
					 * swp_entry_t if PageSwapCache
					 * When page is free, this indicates
					 * order in the buddy system.
					 */
	struct address_space *mapping;	/* If low bit clear, points to
					 * inode address_space, or NULL.
					 * If page mapped as anonymous
					 * memory, low bit is set, and
					 * it points to anon_vma object:
					 * see PAGE_MAPPING_ANON below.
					 */
	pgoff_t index;			/* Our offset within mapping. */
	struct list_head lru;		/* Pageout list, eg. active_list
					 * protected by zone->lru_lock !
					 */
	/*
	 * On machines where all RAM is mapped into kernel address space,
	 * we can simply calculate the virtual address. On machines with
	 * highmem some memory is mapped into kernel virtual memory
	 * dynamically, so we need a place to store that address.
	 * Note that this field could be 16 bits on x86 ... ;)
	 *
	 * Architectures with slow multiplication can define
	 * WANT_PAGE_VIRTUAL in asm/page.h
	 */
#if defined(WANT_PAGE_VIRTUAL)
	void *virtual;			/* Kernel virtual address (NULL if
					   not kmapped, ie. highmem) */
#endif /* WANT_PAGE_VIRTUAL */
};

/******************************include/linux/page-flags.h******************************/
/*
 * Various page->flags bits:
 *
 * PG_reserved is set for special pages, which can never be swapped out. Some
 * of them might not even exist (eg empty_bad_page)...
 *
 * The PG_private bitflag is set if page->private contains a valid value.
 *
 * During disk I/O, PG_locked is used. This bit is set before I/O and
 * reset when I/O completes. page_waitqueue(page) is a wait queue of all tasks
 * waiting for the I/O on this page to complete.
 *
 * PG_uptodate tells whether the page's contents is valid.  When a read
 * completes, the page becomes uptodate, unless a disk I/O error happened.
 *
 * For choosing which pages to swap out, inode pages carry a PG_referenced bit,
 * which is set any time the system accesses that page through the (mapping,
 * index) hash table.  This referenced bit, together with the referenced bit
 * in the page tables, is used to manipulate page->age and move the page across
 * the active, inactive_dirty and inactive_clean lists.
 *
 * Note that the referenced bit, the page->lru list_head and the active,
 * inactive_dirty and inactive_clean lists are protected by the
 * zone->lru_lock, and *NOT* by the usual PG_locked bit!
 *
 * PG_error is set to indicate that an I/O error occurred on this page.
 *
 * PG_arch_1 is an architecture specific page state bit.  The generic code
 * guarantees that this bit is cleared for a page when it first is entered into
 * the page cache.
 *
 * PG_highmem pages are not permanently mapped into the kernel virtual address
 * space, they need to be kmapped separately for doing IO on the pages.  The
 * struct page (these bits with information) are always mapped into kernel
 * address space...
 */

/*
 * Don't use the *_dontuse flags.  Use the macros.  Otherwise you'll break
 * locked- and dirty-page accounting.  The top eight bits of page->flags are
 * used for page->zone, so putting flag bits there doesn't work.
 */
#define PG_locked	 	 0	/* Page is locked. Don't touch. */
#define PG_error		 1
#define PG_referenced		 2
#define PG_uptodate		 3

#define PG_dirty	 	 4
#define PG_lru			 5
#define PG_active		 6
#define PG_slab			 7	/* slab debug (Suparna wants this) */

#define PG_highmem		 8
#define PG_checked		 9	/* kill me in 2.5.<early>. */
#define PG_arch_1		10
#define PG_reserved		11

#define PG_private		12	/* Has something at ->private */
#define PG_writeback		13	/* Page is under writeback */
#define PG_nosave		14	/* Used for system suspend/resume */
#define PG_compound		15	/* Part of a compound page */

#define PG_swapcache		16	/* Swap page: swp_entry_t in private */
#define PG_mappedtodisk		17	/* Has blocks allocated on-disk */
#define PG_reclaim		18	/* To be reclaimed asap */
#define PG_nosave_free		19	/* Free, should not be written */
#define PG_uncached		20	/* Page has been mapped as uncached */

/******************************mm/page_alloc.c******************************/
int min_free_kbytes = 1024;

