#define IFACE_H

#ifndef TYPES_H
#include <types/types.h>
#endif

// Like Golang interfaces, but able to specify required value fields too.
// Also, no runtime madness. Static checking isn't done at the C level.
// inspired by http://research.swtch.com/interfaces

// Meta information for ifaces
//   look in types.h for methods_list and fields_list
typedef struct {
    const fields_list *fields;
    const methods_list *methods;
    ptr name;
} iface_def;

typedef struct {
    ptr data;
    ptr meta; // meta* from types/meta.h
    ptr iface; // iface_def*
} iface;

// face helpers
#define IFACE_FIELDS_OF(iface_name) CONCAT(iface_name, __iface_fields)
#define IFACE_FIELDS_DEF(iface_name) static fields_list FACE_FIELDS_OF(iface_name)
#define IFACE_METHODS_OF(iface_name) CONCAT(iface_name, __iface_methods)
#define IFACE_METHODS_DEF(iface_name) static methods_list FACE_METHODS_OF(iface_name)
#define IFACE_INSTANCE(data, type, iface_name) { & data, & META_OF(type), & iface_name }
#define IFACE_DEF(iface_name) static iface iface_name

TYPE_ID_DEF(iface, 19);

extern META_DECL(iface);

