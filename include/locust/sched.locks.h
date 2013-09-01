#define LOCKS_H

#ifndef TYPES_H
#include <locust/type.types.h>
#endif

#ifndef WAITS_H
#include <locust/sched.waits.h>
#endif

#ifndef ATOMIC_H
#include <locust/sched.atomic.h>
#endif

#ifndef TASK_H
#include <locust/sched.task.h>
#endif

#ifndef DEQUE_H
#include <locust/ds.deque.h>
#endif



/*
 * queuing locks
 */
typedef struct {
	task *owner;
	deque tasks_waiting; // task* deque
} qlock;

#define qlock__type_id ((type_id)3001)
extern TYPE_ID_DECL(qlock);
extern META_DECL(qlock);

void qlock_init(qlock *self);
void qlock_deinit(qlock *self);

void qlock_on(qlock *self);
bool qlock_can(qlock *self);
void qlock_off(qlock *self);

/*
 * reader-writer locks
 */
typedef struct {
	uint64 readers;
	task* writer;
	deque rwaiting; // task* deque
	deque wwaiting; // task* deque
} rwlock;

#define rwlock__type_id ((type_id)3002)
extern TYPE_ID_DECL(rwlock);
extern META_DECL(rwlock);

void rwlock_init(rwlock *self);
void rwlock_deinit(rwlock *self);

void rwlock_r_on(rwlock*);
bool rwlock_r_can(rwlock*);
void rwlock_r_off(rwlock*);

void rwlock_w_on(rwlock*);
bool rwlock_w_can(rwlock*);
void rwlock_w_off(rwlock*);

// simple lock
inline bool lock_try(bool* lock);
inline void lock_on(bool* lock);
inline void lock_off(bool* lock);

/*
// rendezvous (looks like a qlock, too)
typedef struct {
	task *owner;
	deque tasks_waiting; // task* deque
	bool locking;
} rendez;

TYPE_ID_DEF(rendez, 3003);

extern META_DECL(rendez);

META_GETTER_FUNC(rendez);

rendez* rendez_create(bool locking);
void rendez_init(rendez *r, bool locking);
void rendez_deinit();
void rendez_destroy();

void task_sleep(rendez*);
overloaded bool task_wake(rendez *r);
overloaded uint64 task_wake(rendez *r, uint64 count);
*/
