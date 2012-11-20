static inline void list_add(struct list_head *new, struct list_head *head);
static inline void __list_add(struct list_head*,struct list_head* prev,struct list_head* next);
static inline void list_add_tail(struct list_head *new, struct list_head *head);
static inline void list_del(struct list_head *entry);
static inline int list_empty(const struct list_head *head);
#define list_entry(ptr, type, member);
#define list_for_each(pos, head);
/* 把n插入为表头即h->first指向n */
static inline void hlist_add_head(struct hlist_node *n, struct hlist_head *h);
/* 如果n是头怎么办？ */
static inline void hlist_del(struct hlist_node *n);
static inline int hlist_empty(const struct hlist_head *h);
#define hlist_entry(ptr, type, member) container_of(ptr,type,member)
#define hlist_for_each_entry(tpos, pos, head, member);
#define list_for_each_entry(pos, head, member);
/*
  process list and children list
  thread that is not the leader won't insert the precess list
*/
#define SET_LINKS(p);
#define thread_group_leader(p);
#define add_parent(p, parent);
#define REMOVE_LINKS(p);
#define LIST_HEAD_INIT(name);
#define INIT_LIST_HEAD(ptr); 
/* list_del把删除的点prev,next指向一个指定的地址
 list_del_init把删除的点prev,next指向自已 */
static inline void list_del_init(struct list_head *entry);
/* remove from the list of parent's children */
#define remove_parent(p);
#define for_each_process(p);
#define next_task(p);
#define pid_hashfn(nr);
static inline unsigned long hash_long(unsigned long val, unsigned int bits);
struct pid * fastcall find_pid(enum pid_type type, int nr);
#define hlist_for_each_entry(tpos, pos, head, member);
/* get the task through pid struct,
 list_entry可以把多层结构体中任意一层的结构体的地址找出 */
#define pid_task(elem, type);
/* get pid(nr) from type hash, the head of per-PID list of nr */
struct pid * fastcall find_pid(enum pid_type type, int nr);
/* find struct task_t by type and pid*/
task_t *find_task_by_pid_type(int type, int nr);
/* iterates over the per-PID list(the pid_list field of struct pid has the
   same nr) associated with the PID number nr of type type; */
#define do_each_task_pid(who, type, task);
#define while_each_task_pid(who, type, task);
/* if the hlist_node hasn't insert in the hash table ,return true */
static inline int hlist_unhashed(const struct hlist_node *h);
/* Same as find_task_by_pid_type(PIDTYPE_PID, nr) */
#define find_task_by_pid(nr);
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
/* Returns the process descriptor address of the lightweight process that
   follows task in the hash table list of type PIDTYPE_TGID. if it is
   conventional process, return itself. */
task_t fastcall *next_thread(const task_t *p);
/* add to the head of the queue,
   等待队列的头什么不包含内容
 */
#define DECLARE_WAIT_QUEUE_HEAD(name)
static inline void init_waitqueue_head(wait_queue_head_t *q);
static inline void __add_wait_queue(wait_queue_head_t *head, wait_queue_t *new);
int default_wake_function(wait_queue_t *curr, unsigned mode, int sync, void *key);

static inline void init_waitqueue_entry(wait_queue_t *q, struct task_struct *p);
int autoremove_wake_function(wait_queue_t *wait, unsigned mode, int sync, void *key);
#define DEFINE_WAIT(name)
/* flags - 0, task - null, func - func */
static inline void init_waitqueue_func_entry(wait_queue_t *q,
											 wait_queue_func_t func);
/* add to the head of queue */
void fastcall add_wait_queue(wait_queue_head_t *q, wait_queue_t *wait);
static inline void __add_wait_queue_tail(wait_queue_head_t *head,
										 wait_queue_t *new);
/* add to the tail of queue */
void fastcall add_wait_queue_exclusive(wait_queue_head_t *q, wait_queue_t *wait);

void fastcall __sched sleep_on(wait_queue_head_t *q);
void fastcall __sched interruptible_sleep_on(wait_queue_head_t *q);
long fastcall __sched sleep_on_timeout(wait_queue_head_t *q, long timeout);
long fastcall __sched interruptible_sleep_on_timeout(wait_queue_head_t *q, long timeout);


/*
 * don't alter the task state if this is just going to
 * queue an async wait queue callback.
 * if wait haven't inserted into the waitqueue, then insert, but is it
 * possible that the wait haven't insert into the waitqueue?
 */
void prepare_to_wait(wait_queue_head_t *q, wait_queue_t *wait, int state);
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
