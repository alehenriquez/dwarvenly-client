#ifndef STRING_H
#include <types/string.h>
#endif

#ifndef MEM_H
#include <rt/mem.h>
#endif

#ifndef ASSERT_H
#include <rt/assert.h>
#endif

META_FIELDS_DEF(string) = {
    3, {
        {TYPE_ID_OF(uint64), sizeof(uint64), "info"},
        {TYPE_ID_OF(ptr), sizeof(ptr), "bytes"},
    }
};

META_DEF(string, & META_FIELDS_OF(string), nil);


/*
#define bits_set(mem, to, mask) (mem ^= (-to ^ mem) & mask)
#define bits_merge(a, b, mask) (a ^ ((a ^ b) & mask))
#define INFO_OFFSET_BITMASK 7UL
static inline uint64 string_get_offset(string *self) {
    return self->info & INFO_OFFSET_BITMASK;
}

static inline uint64 string_get_capacity(string *self) {
    return self->info & (~INFO_OFFSET_BITMASK);
}

static inline void string_set_len(string *self, uint64 len) {
    // offset: down from capacity to get real len
    uint64 extra = INFO_OFFSET_BITMASK - (len & INFO_OFFSET_BITMASK);
    len = (len + INFO_OFFSET_BITMASK) & (~INFO_OFFSET_BITMASK);
    self->info = bits_merge(len, extra, INFO_OFFSET_BITMASK); // now it's in capacity and offset from capacity form
}
#undef INFO_OFFSET_BITMASK
#undef bits_merge
#undef bits_set
*/

/*
    bigint *self = mem_get(& META_OF(bigint), 1);
    assert(self != nil);
    bigint_init(self, len);
    return self;
}

void bigint_init(bigint *self, uint64 len) {
    assert(self != nil);
    uint64 *bits = mem_get(& META_OF(uint64), len);
    assert(bits != nil);
    self->info = 0;
    bigint_set_len(self, len);
    self->bits = bits;
}

void bigint_deinit(bigint *self) {
    assert(self != nil);
    mem_free(self->bits);
}

void bigint_destroy(bigint *self) {
    assert(self != nil);
    bigint_deinit(self);
    mem_free(self);
*/

string* string_create(uint64 len) {
    string *self = mem_get(& META_OF(string), 1);
    assert(self != nil);
    string_init(self, len);
    return self;
}

void string_init(string *self, uint64 len) {
    assert(self != nil);
    string_set_len(self, len);
    self->bytes = mem_get(& META_OF(byte), string_get_capacity(self));
    assert(self->bytes != nil);
}

void string_deinit(string *self) {
    assert(self != nil);
    mem_free(self->bytes);
}

void string_destroy(string *self) {
    assert(self != nil);
    string_deinit(self);
    mem_free(self);
}


/*
char *trim(const char *input) {
  const char *b, *e;
  char *o;
  b = input;
  e = input + strlen(input);
  while (isspace(*b))     ++b;
  while (isspace(*(e-1))) --e;
  o = (char*)calloc(e-b+1, sizeof(char));
  memcpy(o, b, e-b);
  return o;
}

int slice(const char *input, const char *delims, char** *output) {
  int size = 0;
  char **list = (char**)calloc(size+1, sizeof(char*));

  const char *p = input;
  const char *b = input;
  do {
    if (strchr(delims, *p) != NULL)
    {
      ++size;
      char *s = (char*)calloc(p-b+1, sizeof(char));
      memcpy(s, b, p-b);
      list = (char**)realloc(list, (size+1)*sizeof(char*));
      list[size-1] = s;
      list[size] = NULL;
      b = p+1;
    }
  } while (*(p++));
  *output = list;
  return size;
}
*/
