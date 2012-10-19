#define CHAN_H

#ifndef TYPES_H
#include <types/types.h>
#endif

#ifndef DEQUE_H
#include <ds/deque.h>
#endif

typedef struct {
	deque	q1;
	deque	q2;
} chan;

TYPE_ID_DEF(chan, 35);

extern META_DECL(chan);

META_GETTER_FUNC(chan);

chan* chan_create(const meta *type);
void chan_init(chan *self, const meta *type);
void chan_deinit(chan *self);
void chan_destroy(chan *self);

void chan_front_send(chan *self, const meta *type, ptr item);
bool chan_front_recv(chan *self, const meta *type, ptr ret);

void chan_back_send(chan *self, const meta *type, ptr item);
bool chan_back_recv(chan *self, const meta *type, ptr ret);
