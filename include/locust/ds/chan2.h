#define CHAN2_H

#ifndef TYPES_H
#include <types/types.h>
#endif

#ifndef LOCKS_H
#include <sched/locks.h>
#endif

typedef struct {
	const meta *type;
	int64 direction;
	deque rpending;
	deque spending;
} chan2;

TYPE_ID_DEF(chan2, 3000);

extern META_DECL(chan2);

META_GETTER_FUNC(chan2);

chan2* chan2_create(const meta *type);
void chan2_init(chan2 *self, const meta *type);
void chan2_deinit(chan2 *self);
void chan2_destroy(chan2 *self);


bool chan2_send(chan2 *self, const meta *type, ptr item);
bool chan2_recv(chan2 *self, const meta *type, ptr ret);

