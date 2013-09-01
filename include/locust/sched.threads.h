#define THREADS_H

#ifndef TYPES_H
#include <locust/type.types.h>
#endif

#ifndef TASK_H
#include <locust/sched.task.h>
#endif

#ifndef CONTEXT_H
#include <locust/sched.context.h>
#endif

enum {
	STACK = 8192
};

/* thread */
typedef struct {
	uint64 id;
	task *curr_task;
	context *sched_ctx;
	deque *tasks;
	byte thread_id[];
} thread;

#define thread__type_id ((type_id)22)
extern TYPE_ID_DECL(thread);

extern META_DECL(thread);

META_GETTER_FUNC(thread);

extern thread *__threads;
extern uint64 __threads_count;

void threads_init(uint64 thread_count);
void threads_deinit(void);
void threads_launch(void);
void threads_run(thread *context);
thread* threads_curr();
