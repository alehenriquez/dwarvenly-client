#define STRING_H

#ifndef TYPES_H
#include <locust/type.types.h>
#endif

typedef struct {
    uint64 info;
    ptr bytes;
} string;

#define string__type_id ((type_id)17)
extern TYPE_ID_DECL(string);
extern META_DECL(string);


#define S(char_array) { sizeof(char_array), sizeof(char_array), (byteptr)(char_array) }

string string_from_cstr(const char *s);

void string_init(string *self, uint64 len);
void string_deinit(string *self);

void string_push(string *v, byte item);
void string_pop(string *v, ptr item);
void string_set(string *v, uint64 index, byte item);
void string_get(string *v, uint64 index, ptr item);

// This function trims trim_chars from the side of a string specified.
// With no trim characters specified, whitespace is trimmed.
string overloaded string_trim(string s, string trim_chars);
string overloaded string_trim(string s);
string overloaded string_triml(string s, string trim_chars);
string overloaded string_triml(string s);
string overloaded string_trimr(string s, string trim_chars);
string overloaded string_trimr(string s);

uint64 string_count_of(string s, string sub);

#define bits_set(mem, to, mask) (mem ^= (-to ^ mem) & mask)
#define bits_merge(a, b, mask) (a ^ ((a ^ b) & mask))
#define INFO_OFFSET_BITMASK 7UL

static inline uint64 string_get_offset(string *self) {
    return self->info & INFO_OFFSET_BITMASK;
}

static inline void string_set_offset(string *self, uint64 offset) {
	bits_set(self->info, (offset & INFO_OFFSET_BITMASK), INFO_OFFSET_BITMASK);
}

static inline uint64 string_get_capacity(string *self) {
    return self->info & (~INFO_OFFSET_BITMASK);
}

static inline void string_set_capacity(string *self, uint64 cap) {
	bits_set(self->info, (cap & ~(INFO_OFFSET_BITMASK)), (~INFO_OFFSET_BITMASK));
}

static inline uint64 string_get_len(string *self) {
	return string_get_capacity(self) - string_get_offset(self);
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

