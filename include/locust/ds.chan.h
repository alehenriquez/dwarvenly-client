#define CHAN_H

#ifndef TYPES_H
#include <locust/type.types.h>
#endif

#ifndef DEQUE_H
#include <locust/ds.deque.h>
#endif

typedef struct {
	deque	q1;
	deque	q2;
} chan;

#define chan__type_id ((type_id)35)
extern TYPE_ID_DECL(chan);
extern META_DECL(chan);

void chan_init(chan *self, const meta *type);
void chan_deinit(chan *self);

void chan_front_send(chan *self, const meta *type, ptr item);
bool chan_front_recv(chan *self, const meta *type, ptr ret);

void chan_back_send(chan *self, const meta *type, ptr item);
bool chan_back_recv(chan *self, const meta *type, ptr ret);
