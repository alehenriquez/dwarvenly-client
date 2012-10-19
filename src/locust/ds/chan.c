#ifndef CHAN_H
#include <ds/chan.h>
#endif

META_FIELDS_DEF(chan) = {
    2, {
        {TYPE_ID_OF(deque), sizeof(deque), "q1"},
		{TYPE_ID_OF(deque), sizeof(deque), "q2"},
    }
};

META_DEF(chan, & META_FIELDS_OF(chan), nil);

#ifndef MEM_H
#include <rt/mem.h>
#endif

#ifndef ASSERT_H
#include <rt/assert.h>
#endif

#ifndef TASK_H
#include <sched/task.h>
#endif

#include <stdio.h>

chan* chan_create(const meta *type) {
    assert(type != nil);
	chan *c = mem_get(& META_OF(chan), 1);
	assert(c != nil);
	mem_set(c, sizeof(chan), 0);
	chan_init(c, type);
	return c;
}

void chan_init(chan *self, const meta *type) {
    assert(self != nil);
    assert(type != nil);
	deque_init(&self->q1, type);
	deque_init(&self->q2, type);
}

void chan_deinit(chan *self) {
    assert(self != nil);
	deque_deinit(&self->q1);
	deque_deinit(&self->q2);
}

void chan_destroy(chan *self) {
    assert(self != nil);
	chan_deinit(self);
	mem_free(self);
}

void chan_front_send(chan *self, const meta *type, ptr item) {
    assert(self != nil);
    assert(type != nil);
    assert(item != nil);
	deque_back_push(&self->q1, type, item);
	task_yield(task_curr());
}

bool chan_front_recv(chan *self, const meta *type, ptr ret) {
    assert(self != nil);
    assert(type != nil);
    assert(ret != nil);
	bool popped = deque_front_pop(&self->q2, type, ret);
	task_yield(task_curr());
	return popped;
}

void chan_back_send(chan *self, const meta *type, ptr item) {
    assert(self != nil);
    assert(type != nil);
    assert(item != nil);
	deque_back_push(&self->q2, type, item);
	task_yield(task_curr());
}

bool chan_back_recv(chan *self, const meta *type, ptr ret) {
    assert(self != nil);
    assert(type != nil);
    assert(ret != nil);
	bool popped = deque_front_pop(&self->q1, type, ret);
	task_yield(task_curr());
	return popped;
}
