#ifndef RING_H
#include <ds/ring.h>
#endif

META_FIELDS_DEF(ring) = {
    6, {
        {TYPE_ID_OF(uint64), sizeof(uint64), "cap"},
        {TYPE_ID_OF(uint64), sizeof(uint64), "r_counter"},
        {TYPE_ID_OF(uint64), sizeof(uint64), "w_counter"},
        {TYPE_ID_OF(uint64), sizeof(uint64), "a_counter"},
        {TYPE_ID_OF(ptr), sizeof(ptr), "items"},
        {TYPE_ID_OF(ptr), sizeof(ptr), "items_desc"},
    }
};

META_DEF(ring, & META_FIELDS_OF(ring), nil);

#ifndef MEM_H
#include <rt/mem.h>
#endif

#ifndef ATOMIC_H
#include <sched/atomic.h>
#endif

#ifndef ASSERT_H
#include <rt/assert.h>
#endif

#include <stdio.h>

ring* ring_create(const meta *type, uint64 cap) {
    assert(type != nil);
    ring *self = mem_get(sizeof(ring)+(type->size*cap));
    assert(self != nil);
    ring_init(self, type, cap);
    return self;
}

void ring_init(ring *self, const meta *type, uint64 cap) {
    assert(self != nil);
    assert(type != nil);

    // ring needs to be a power of 2, round up to make it so.
    cap--;
    cap |= cap >> 1;
    cap |= cap >> 2;
    cap |= cap >> 4;
    cap |= cap >> 8;
    cap |= cap >> 16;
    cap |= cap >> 32;
    cap++;

    self->cap = cap;
    self->r_counter = 0;
    self->w_counter = 0;
    self->a_counter = 0;
    self->items_desc = type;
    self->items = mem_get(type, cap);
    assert(self->items != nil);
    return;
}

void ring_deinit(ring *self) {
    assert(self != nil);
    mem_free(self->items);
}

void ring_destroy(ring *self) {
    assert(self != nil);
	ring_deinit(self);
	mem_free(self);
}

void ring_print(ring *self) {
    assert(self != nil);
    printf("<ring cap='%ld' r_counter='%ld' w_counter='%ld' a_counter='%ld' />", self->cap, self->r_counter, self->w_counter, self->a_counter);
}

bool ring_write(ring *self, const meta *type, ptr item) {
    assert(self != nil);
    assert(type != nil);
    assert(item != nil);
    TYPE_CHECK(self->items_desc, type);
    uint64 front;
    uint64 back;
    uint64 index;

    byte* bytes = self->items;
    uint32 bytes_size = self->items_desc->size;

    uint64 w_pending;
    uint64 w_pending_;
    uint64 w_pending__;

    // This loop either adds the item, or hits a full ring buffer.
    while (true) {
        front = self->a_counter; // These could increment after this but only the
        back = self->r_counter;  // a_counter is bad to have increment until the cas

        // Check for butting up against the back from other side.
        // If so, not necessarily full at this moment in time, but close enough.
        if (front - back == self->cap) { return false; }

        // Attempt to acquire a spot in the ring.
        // If not, start this whole attempt over with new spot to acquire.
        if (!atomic_cmp_and_set(&self->a_counter, front, front+1)) { continue; }

        index = front & (self->cap-1);

        // Guaranteed to have acquired a spot in the ring. Shunt item in.
        mem_copy(item, bytes + (index * bytes_size), bytes_size);

        // Add a tic to the pending write count. We'll update w_counter
        // if we are the writer any other writes "behind" us are waiting behind.
        // e.g. if we took longer to write than someone who acquired after we did
        // but finished writing before we did.
        w_pending = atomic_fetch_and_add(&self->w_pending, (uint64)1);

        // This increases r->w_counter only if we're responsible for it. (the first write request from w_counter.)
        // Otherwise we can just live with having set pending and letting the other slow writer handle for us here.
        if (!atomic_cmp_and_set(&self->w_counter, front, w_pending+1)) {
            return true;
        }
        // Since we could set the above, we must decrease w_pending before another writer updates w_counter.
        // TODO: I'm not sure if this is actually guaranteed...

        // Set r->w_pending to new low value, in case someone else has written
        // since we incremented r->w_pending
        // TODO: I don't like how this is unbounded.
        while (true) {
            w_pending_ = self->w_pending;
            w_pending__ = w_pending_ - (w_pending+1);

            if (atomic_cmp_and_set(&self->w_pending, w_pending_, front+1+w_pending__)) {
                break;
            }
        }
        return true;
    }
}

bool ring_read(ring *self, const meta *type, ptr item) {
    assert(self != nil);
    assert(type != nil);
    assert(item != nil);
    TYPE_CHECK(self->items_desc, type);
    uint64 front;
    uint64 back;
    uint64 index;
    byte* bytes = self->items;
    uint32 bytes_size = self->items_desc->size;

    while (true) {
        back = self->r_counter; // These could increment after this but only the
        front = self->w_counter;// r_counter is bad to have increment until the cas

        // Check for butting up against the front.
        // If so, not necessarily empty at this moment in time, but close enough.
        if (front - back == 0) {
            return false;
        }

        // Attempt to relinquish a spot in the ring.
        // If not, start this whole attempt over with new spot to relinquish.
        if (!atomic_cmp_and_set(&self->r_counter, back, back+1)) {
            continue;
        }

        // Guaranteed to be able to read this now.
        index = back & (self->cap-1);
        mem_copy(bytes + (index * bytes_size), item, bytes_size);
        return true;
    }
}

