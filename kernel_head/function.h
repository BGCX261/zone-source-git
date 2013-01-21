/* 从address_space中删除的页是怎么被回收的呢？
 对于要加一个东西就要注意是否分配，计数器，对其它数据结构的影响等问题 */

#define LIST_HEAD_INIT(name)
#define LIST_HEAD(name)
#define INIT_LIST_HEAD(ptr)
static inline void __list_add(struct list_head*,struct list_head* prev,struct list_head* next);
static inline void list_add(struct list_head *new, struct list_head *head);
static inline void list_add_tail(struct list_head *new, struct list_head *head);
static inline void __list_add_rcu(struct list_head * new,
								  struct list_head * prev, struct list_head * next);
static inline void list_add_rcu(struct list_head *new, struct list_head *head);
static inline void list_add_tail_rcu(struct list_head *new,
									 struct list_head *head);
static inline void __list_del(struct list_head * prev, struct list_head * next);
static inline void list_del(struct list_head *entry);
/* 与list_del的区别是next指针没设置 */
/**
 * list_del_rcu - deletes entry from list without re-initialization
 * @entry: the element to delete from the list.
 *
 * Note: list_empty on entry does not return true after this,
 * the entry is in an undefined state. It is useful for RCU based
 * lockfree traversal.
 *
 * In particular, it means that we can not poison the forward
 * pointers that may still be used for walking the list.
 *
 * The caller must take whatever precautions are necessary
 * (such as holding appropriate locks) to avoid racing
 * with another list-mutation primitive, such as list_del_rcu()
 * or list_add_rcu(), running on this same list.
 * However, it is perfectly legal to run concurrently with
 * the _rcu list-traversal primitives, such as
 * list_for_each_entry_rcu().
 *
 * Note that the caller is not permitted to immediately free
 * the newly deleted entry.  Instead, either synchronize_kernel()
 * or call_rcu() must be used to defer freeing until an RCU
 * grace period has elapsed.
 */
static inline void list_del_rcu(struct list_head *entry);
/*
 * list_replace_rcu - replace old entry by new one
 * @old : the element to be replaced
 * @new : the new element to insert
 *
 * The old entry will be replaced with the new entry atomically.
 */
static inline void list_replace_rcu(struct list_head *old, struct list_head *new);
/**
 * list_move - delete from one list and add as another's head
 * @list: the entry to move
 * @head: the head that will precede our entry
 */
static inline void list_move(struct list_head *list, struct list_head *head);
static inline void list_move_tail(struct list_head *list,
								  struct list_head *head);
static inline int list_empty(const struct list_head *head);
/* 如果函数返回1，那么一定是空的，所以不用加锁来判数是否为空 */
static inline int list_empty_careful(const struct list_head *head);
/* list指向一个链表的头，head又指向另一个链表的头
 把list中的节点放到head的链表中去，把list中的第一个节点放到head的第一个节点上去，
 而head的第一个节点接到list最后一个节点的后面，list的头没有在head这个链表中 */
static inline void __list_splice(struct list_head *list,
								 struct list_head *head);
static inline void list_splice(struct list_head *list,
							   struct list_head *head);
/* 与list_splice的区别是初始化了list */
static inline void list_splice_init(struct list_head *list,
									struct list_head *head);
/* list_del把删除的点prev,next指向一个指定的地址
 list_del_init把删除的点prev,next指向自已 */
static inline void list_del_init(struct list_head *entry);#define list_entry(ptr, type, member);
#define list_entry(ptr, type, member)
/* head的next为第一个 */
#define list_for_each(pos, head);
/* head的next为第一个 */
#define list_for_each_entry(pos, head, member);
/* 反方向 */
#define list_for_each_prev(pos, head);
/* 循环过程中可以把pos删除掉 */
#define list_for_each_safe(pos, n, head)
#define list_for_each_entry(pos, head, member)
/* 反方向 */
#define list_for_each_entry_reverse(pos, head, member)
#define list_prepare_entry(pos, head, member)
#define list_for_each_entry_continue(pos, head, member)
/**
 * list_for_each_rcu	-	iterate over an rcu-protected list
 * @pos:	the &struct list_head to use as a loop counter.
 * @head:	the head for your list.
 *
 * This list-traversal primitive may safely run concurrently with
 * the _rcu list-mutation primitives such as list_add_rcu()
 * as long as the traversal is guarded by rcu_read_lock().
 */
#define list_for_each_rcu(pos, head)
#define __list_for_each_rcu(pos, head)
/* 可用于list_add_rcu() */
#define list_for_each_safe_rcu(pos, n, head)
#define list_for_each_entry_rcu(pos, head, member)
#define list_for_each_continue_rcu(pos, head)

/* list_head里是两个list_head指针,
   hlist_head里是一个hlist_node指针,
   hlist_node里是两个hlist_node指针
*/
/* pprev是等于下一个结点的next成员的地址 */
/* 把n插入为表头即h->first指向n,
   n->pprev是等于&h->first */
static inline void hlist_add_head(struct hlist_node *n, struct hlist_head *h);
/* 如果n是头怎么办？没事，因为hlist_head中的first是一个指针,
   在这就可以看出为什么要pprev成员了,
   这个也初始化被删的结点到两个地址 */
static inline void hlist_del(struct hlist_node *n);
static inline void __hlist_del(struct hlist_node *n);
static inline void hlist_del_rcu(struct hlist_node *n);
/* next must be != NULL,
   n插在next之前,*/
static inline void hlist_add_before(struct hlist_node *n,
									struct hlist_node *next);
/* next插在n之后 */
static inline void hlist_add_after(struct hlist_node *n,
								   struct hlist_node *next);
#define hlist_for_each_safe(pos, n, head)
#define hlist_for_each_rcu(pos, head)
/* 只检查first是否为空 */
static inline int hlist_empty(const struct hlist_head *h);
#define hlist_entry(ptr, type, member) container_of(ptr,type,member)
#define hlist_for_each_entry(tpos, pos, head, member);
/* 可以接在上一个被遍历到的节点 */
#define hlist_for_each_entry_continue(tpos, pos, member)
/* 与continue的区别是开始时不用获取节点，pos已指向需要的第一个节点 */
#define hlist_for_each_entry_from(tpos, pos, member)
#define hlist_for_each_entry_safe(tpos, pos, n, head, member)
#define hlist_for_each_entry_rcu(tpos, pos, head, member)

/*
  process list and children list
  thread that is not the leader won't insert the precess list
*/
#define SET_LINKS(p);
#define thread_group_leader(p);	/* 判断p是否是thread group leader */
#define add_parent(p, parent);
#define REMOVE_LINKS(p);
/* remove from the list of parent's children */
#define remove_parent(p);
#define for_each_process(p);
#define next_task(p);
#define pid_hashfn(nr);
static inline unsigned long hash_long(unsigned long val, unsigned int bits);
struct pid * fastcall find_pid(enum pid_type type, int nr);
/* get the task through pid struct,
 list_entry可以把多层结构体中任意一层的结构体的地址找出 */
#define pid_task(elem, type);
/* get pid(nr) from type hash, the head of per-PID list of nr */
struct pid * fastcall find_pid(enum pid_type type, int nr);
fastcall void free_pidmap(int pid);
int alloc_pidmap(void);

/* find struct task_t by type and pid*/
task_t *find_task_by_pid_type(int type, int nr);
/* iterates over the per-PID list(the pid_list field of struct pid has the
   same nr) associated with the PID number nr of type type; */
#define do_each_task_pid(who, type, task);
#define while_each_task_pid(who, type, task);
/* if the hlist_node hasn't insert in the hash table ,return true */
static inline int hlist_unhashed(const struct hlist_node *h);
/*
 * This function switches the PIDs if a non-leader thread calls
 * sys_execve() - this must be done without releasing the PID.
 * (which a detach_pid() would eventually do.)
 * Returns the process descriptor address of the lightweight process that
 * follows task in the hash table list of type PIDTYPE_TGID. if it is
 * conventional process, return itself.
 */
void switch_exec_pids(task_t *leader, task_t *thread);
/* Inserts the process descriptor pointed to by task in the PID hash table of
   type type according to the PID number nr; if a process descriptor having
   PID nr is already in the hash table, the function simply inserts task in
   the per-PID list of the already present process. */
int fastcall attach_pid(task_t *task, enum pid_type type, int nr);
/* if the task's per-PID list is empty return nr */
static fastcall int __detach_pid(task_t *task, enum pid_type type);
/* Removes the process descriptor pointed to by task from the per-PID list
   of type type to which the descriptor belongs. If the per-PID list does not
   become empty, the function terminates. Otherwise, the function removes the
   process descriptor from the hash table of type type; finally, if the PID
   number does not occur in any other hash table, the function clears the
   corresponding bit in the PID bitmap, so that the number can be recycled.
   so it's chain_list, pid_list, pidmap.
*/
void fastcall detach_pid(task_t *task, enum pid_type type);
/* Same as find_task_by_pid_type(PIDTYPE_PID, nr) */
#define find_task_by_pid(nr);
#define fls(x) generic_fls(x)
/* 
 * fls: find last bit set.
*/
static __inline__ int generic_fls(int x);
void __init pidhash_init(void);
void __init pidmap_init(void);
/* scan the per-PID list */
task_t fastcall *next_thread(const task_t *p);
/* wait_queue_head_t is just a node of wait queue  */
/* add to the head of the queue,
   等待队列的头什么不包含内容，两个指针指向自己的list_head
 */
#define DECLARE_WAIT_QUEUE_HEAD(name)
static inline void init_waitqueue_head(wait_queue_head_t *q);
/* insert in the head of the queue */
static inline void __add_wait_queue(wait_queue_head_t *head, wait_queue_t *new);
/* try_to_wake_up */
int default_wake_function(wait_queue_t *curr, unsigned mode, int sync, void *key);
/* init the wait_queue_t struct,
 the func entry is default_wake_function */
static inline void init_waitqueue_entry(wait_queue_t *q, struct task_struct *p);

/* This function invokes default_wake_function( ) to awaken the sleeping process,
   and then removes the wait queue element from the wait queue list. */
int autoremove_wake_function(wait_queue_t *wait, unsigned mode, int sync, void *key);
#define DEFINE_WAIT(name)
/* flags - 0, task - null, func - func,
 compare with the init_waitqueue_entry*/
static inline void init_waitqueue_func_entry(wait_queue_t *q,
											 wait_queue_func_t func);
/* add to the head of queue */
void fastcall add_wait_queue(wait_queue_head_t *q, wait_queue_t *wait);
static inline void __add_wait_queue(wait_queue_head_t *head, wait_queue_t *new);
static inline void __add_wait_queue_tail(wait_queue_head_t *head,
										 wait_queue_t *new);
void fastcall add_wait_queue_exclusive(wait_queue_head_t *q, wait_queue_t *wait);

/* use default_wait_function */
void fastcall __sched sleep_on(wait_queue_head_t *q);
void fastcall __sched interruptible_sleep_on(wait_queue_head_t *q);
long fastcall __sched sleep_on_timeout(wait_queue_head_t *q, long timeout);
long fastcall __sched interruptible_sleep_on_timeout(wait_queue_head_t *q, long timeout);


/* DEFINE_WAIT(wait); */
/*     prepare_to_wait_exclusive(&wq, &wait, TASK_INTERRUPTIBLE); */
/*                                 /\* wq is the head of the wait queue *\/ */
/*     ... */
/*     if (!condition) */
/*         schedule(); */
/*     finish_wait(&wq, &wait);
	   比sleep_on加多了一个条件判断
*/
/*
 * don't alter the task state if this is just going to
 * queue an async wait queue callback.
 * if wait haven't inserted into the waitqueue, then insert, but is it
 * possible that the wait haven't insert into the waitqueue?
 */
void prepare_to_wait(wait_queue_head_t *q, wait_queue_t *wait, int state);
#define is_sync_wait(wait)	(!(wait) || ((wait)->task))
/* tests whether a list is
 * empty _and_ checks that no other CPU might be
 * in the process of still modifying either member,
 * doesn't need the lock.
 */
static inline int list_empty_careful(const struct list_head *head);

/* calling list_empty_careful, didn't  */
void fastcall finish_wait(wait_queue_head_t *q, wait_queue_t *wait);

/* max_scan是max_pid所占的页数，就是说offset一开始不是在一页的开始位置的，
   max_scan这个循环就要做max_pid+1次 */
int alloc_pidmap(void);

/* 	if (nr_threads >= max_threads)这句话说明什么呢？nr_threads只在
	copy_process上加只在exit上减. */
static task_t *copy_process(unsigned long clone_flags,
							unsigned long stack_start,
							struct pt_regs *regs,
							unsigned long stack_size,
							int __user *parent_tidptr,
							int __user *child_tidptr,
							int pid);

static inline int try_module_get(struct module *module);

/*
 * This gets called before we allocate a new thread and copy
 * the current task into it.
 */
/* 只有dup_task_struct调用 */
void prepare_to_copy(struct task_struct *tsk);
/* 为什么要设置usage为2 */
static struct task_struct *dup_task_struct(struct task_struct *orig);
static inline void copy_flags(unsigned long clone_flags, struct task_struct *p);
int copy_semundo(unsigned long clone_flags, struct task_struct *tsk);
static int copy_files(unsigned long clone_flags, struct task_struct * tsk);
static inline int copy_fs(unsigned long clone_flags, struct task_struct * tsk);
static inline int copy_sighand(unsigned long clone_flags, struct task_struct * tsk);
static inline int copy_signal(unsigned long clone_flags, struct task_struct * tsk);
static int copy_mm(unsigned long clone_flags, struct task_struct * tsk);
int copy_keys(unsigned long clone_flags, struct task_struct *tsk);
int copy_namespace(int flags, struct task_struct *tsk);
/* thread_info  */
int copy_thread(int nr, unsigned long clone_flags, unsigned long esp,
	unsigned long unused,
	struct task_struct * p, struct pt_regs * regs)









