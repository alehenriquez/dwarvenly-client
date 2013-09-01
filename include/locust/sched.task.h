#define TASK_H

#ifndef TYPES_H
#include <locust/type.types.h>
#endif

#ifndef CONTEXT_H
#include <locust/sched.context.h>
#endif

#ifndef DEQUE_H
#include <locust/ds.deque.h>
#endif

/*
 * Tasks
 * Copyright (c) 2005 Russ Cox, MIT; see LIBTASK_COPYRIGHT
 * Altered by AndrewBC - 2012
 */
typedef struct {
	uint64 id;
	const byte* name;
    context ctx;
	void (*start_fn)(ptr);
	ptr start_arg;
	ptr data;
	uint64 stack_len;
	ptr stack;
	uint32 state;
} task;

#define task__type_id ((type_id)23)
extern TYPE_ID_DECL(task);
extern META_DECL(task);

void task_init(task *t, const byte *name, void (*fn)(ptr), ptr arg, uint64 stacksize);
void task_deinit(task *t);

enum {
    __task_mask_exiting = 1<<0,
    __task_mask_ready   = 1<<1,
    __task_mask_daemon  = 1<<2,
    __task_mask_yield   = 1<<3,
};

#ifndef DEBUG_H
#include <locust/rt.debug.h>
#endif

bool static inline nonnullfunc task_is_exiting(task *t) {
    return ((t->state & __task_mask_exiting) != 0);
}

void static inline nonnullfunc task_set_exiting_on(task *t) {
    t->state |= __task_mask_exiting;
}

void static inline nonnullfunc task_set_exiting_off(task *t) {
    t->state &= ~__task_mask_exiting;
}

bool static inline nonnullfunc task_is_ready(task *t) {
    return ((t->state & __task_mask_ready) != 0);
}

void static inline nonnullfunc task_set_ready_on(task *t) {
    t->state |= __task_mask_ready;
}

void static inline nonnullfunc task_set_ready_off(task *t) {
    t->state &= ~__task_mask_ready;
}

bool static inline nonnullfunc task_is_daemon(task *t) {
    return ((t->state & __task_mask_daemon) != 0);
}

void static inline nonnullfunc task_set_daemon_on(task *t) {
    t->state |= __task_mask_daemon;
}

void static inline nonnullfunc task_set_daemon_off(task *t) {
    t->state &= ~__task_mask_daemon;
}

bool static inline nonnullfunc task_is_yielding(task *t) {
    return ((t->state & __task_mask_yield) != 0);
}

void static inline nonnullfunc task_set_yielding_on(task *t) {
    t->state |= __task_mask_yield;
}

void static inline nonnullfunc task_set_yielding_off(task *t) {
    t->state &= ~__task_mask_yield;
}

task *task_curr(void);
void task_schedule(task *t);
void task_schedule_next(task *t);
void task_daemonize(task *t);
void task_switch_to(task *t);
void overloaded task_yield(task *t, bool reschedule);
static inline void overloaded task_yield(task *t) {
	task_yield(t, 1);
}
static inline void task_sleep(task *t) {
	task_yield(t, 0);
}
void task_exit_all();
void task_exit(task *t);
void task_needstack(task *t, int n);


/*
 * basic procs and threads
 */

void		taskmain(int argc, char *argv[]);
unsigned int	taskdelay(unsigned int);

/*
 * Threaded I/O.
 */
int		fdread(int, void*, int);
int		fdread1(int, void*, int);	/* always uses fdwait */
int		fdwrite(int, void*, int);
void		fdwait(int, int);
int		fdnoblock(int);

void		fdtask(void*);

/*
 * Network dialing - sets non-blocking automatically
 */
enum {
	UDP = 0,
	TCP = 1,
};

int		netannounce(int, char*, int);
int		netaccept(int, char*, int*);
int		netdial(int, char*, int);
int		netlookup(char*, uint32_t*);	/* blocks entire program! */
int		netdial(int, char*, int);
