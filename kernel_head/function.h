static inline void list_add(struct list_head *new, struct list_head *head);
static inline void list_add_tail(struct list_head *new, struct list_head *head);
static inline void list_del(struct list_head *entry);
static inline int list_empty(const struct list_head *head);
#define list_entry(ptr, type, member)
#define list_for_each(pos, head)
static inline void hlist_add_head(struct hlist_node *n, struct hlist_head *h)
static inline void hlist_del(struct hlist_node *n)
static inline int hlist_empty(const struct hlist_head *h)
#define hlist_entry(ptr, type, member) container_of(ptr,type,member)
#define hlist_for_each_entry(tpos, pos, head, member)
#define list_for_each_entry(pos, head, member)
/* thread that is not the leader won't insert the precess list */
#define SET_LINKS(p)
#define thread_group_leader(p)
#define add_parent(p, parent);
#define REMOVE_LINKS(p)
static inline void list_del_init(struct list_head *entry);
/* remove from the list of parent's children */
#define remove_parent(p)
static inline void hlist_add_head(struct hlist_node *n, struct hlist_head *h);
#define for_each_process(p) 
#define next_task(p)
#define pid_hashfn(nr)
static inline unsigned long hash_long(unsigned long val, unsigned int bits);
struct pid * fastcall find_pid(enum pid_type type, int nr)
#define hlist_for_each_entry(tpos, pos, head, member)
/* get the task through pid */
#define pid_task(elem, type)
/* get pid(nr) from type hash */
struct pid * fastcall find_pid(enum pid_type type, int nr)
/* find struct task_t by type and pid*/
task_t *find_task_by_pid_type(int type, int nr)
/* iterates over the per-PID list(the pid_list field of struct pid has the
   same nr) associated with the PID number nr of type type; */
#define do_each_task_pid(who, type, task)
#define while_each_task_pid(who, type, task)
/* if the hlist_node hasn't insert in the hash table ,return true */
static inline int hlist_unhashed(const struct hlist_node *h)
/* Same as find_task_by_pid_type(PIDTYPE_PID, nr) */
#define find_task_by_pid(nr)
/* Inserts the process descriptor pointed to by task in the PID hash table of
   type type according to the PID number nr; if a process descriptor having
   PID nr is already in the hash table, the function simply inserts task in
   the per-PID list of the already present process. */
int fastcall attach_pid(task_t *task, enum pid_type type, int nr)
/* if the task's per-PID list is empty return nr */
static fastcall int __detach_pid(task_t *task, enum pid_type type)
/* Removes the process descriptor pointed to by task from the per-PID list
   of type type to which the descriptor belongs. If the per-PID list does not
   become empty, the function terminates. Otherwise, the function removes the
   process descriptor from the hash table of type type; finally, if the PID
   number does not occur in any other hash table, the function clears the
   corresponding bit in the PID bitmap, so that the number can be recycled. */
void fastcall detach_pid(task_t *task, enum pid_type type)
/* Returns the process descriptor address of the lightweight process that
   follows task in the hash table list of type PIDTYPE_TGID. if it is
   conventional process, return itself. */
task_t fastcall *next_thread(const task_t *p)
/*
 * don't alter the task state if this is just going to
 * queue an async wait queue callback.
 * if wait haven't inserted into the waitqueue, then insert, but is it
 * possible that the wait haven't insert into the waitqueue?

 */
prepare_to_wait(wait_queue_head_t *q, wait_queue_t *wait, int state)
/* tests whether a list is
 * empty _and_ checks that no other CPU might be
 * in the process of still modifying either member,
 * doesn't need the lock.
 */
static inline int list_empty_careful(const struct list_head *head)

/* calling list_empty_careful, didn't  */
void fastcall finish_wait(wait_queue_head_t *q, wait_queue_t *wait)

