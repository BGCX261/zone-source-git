digraph wait_h{
        size="10,18";
        ratio=filled;
        
		node[style=filled, shape = box, margin="0.05,0.005",height="0.1",width="0.1"];

		subgraph cluster_init_waitqueue_head{
			label="static inline void init_waitqueue_head(wait_queue_head_t *q)";
			
			spin_lock_init[label="spin_lock_init(&q->lock);"];
			INIT_LIST_HEAD[label="INIT_LIST_HEAD(&q->task_list);"];

			spin_lock_init -> INIT_LIST_HEAD;
		}

		subgraph cluster_init_waitqueue_entry{
			label="init_waitqueue_entry(wait_queue_t *q, struct task_struct *p)";

			flags_0[label="flags_0"];
			task_p[label="task_p"];
			func_default_wake_function[label="func_default_wake_function"];

			flags_0 -> task_p;
			task_p -> func_default_wake_function;
		}

		subgraph cluster_autoremove_wake_function{
			label="int autoremove_wake_function(wait_queue_t *wait, unsigned mode, int sync, void *key)";

			default_wake_function[label="default_wake_function(wait, mode, sync, key)"];
			list_del_init[label="list_del_init(&wait->task_list);"];

			default_wake_function -> list_del_init;
		}

		        
		subgraph cluster__add_wait_queue{
				label="__add_wait_queue(wait_queue_head_t *head, wait_queue_t *new)";

				list_add[label="list_add(&new->task_list, &head->task_list);"];
		}

		subgraph cluster__remove_wait_queue{
			label="static inline void __remove_wait_queue(wait_queue_head_t *head,wait_queue_t *old)";

			list_del[label="list_del(&old->task_list);"];
		}

		digraph cluster_waitqueue_active{
			label="int waitqueue_active(wait_queue_head_t *q)";

			not_list_empty[label="!list_empty(&q->task_list);"];
		}

}
