#ifndef _LIST_H_
#define _LIST_H_
typedef struct Type_* Type;
typedef struct FieldList_* FieldList;
typedef struct FuncList_* FuncList;
typedef struct StructList_* StructList;
struct Type_
{
	enum{BASIC,ARRAY,STRUCTURE} kind;
	union
	{
		int basic;
		struct {
			Type elem;
			int size;
		} array;
		FieldList structure;
	} u;
};
struct FieldList_{
	char name[32];
	Type type;
	Type stack[20];
	int nowstack;
	int var_no;
	int isparam;
	FieldList tail;
};
struct FuncList_{
	char name[32];
	char returnname[32];
	int isdefined;
	int paramnumber;
	int line;
	FieldList params;
	FuncList tail;
};
struct StructList_{
	char name[32];
	Type type;
	StructList tail;
};
Type newtype();
Type newint();
Type newfloat();
FieldList newfield();
FuncList newfunc();
StructList newstruct();
void addstructfield(StructList other);
void addvarfield(FieldList other);
void addfuncfield(FuncList other);
int findname(const char* name);
int findstruct(const char* name);
int findfunc(const char* name);
int findvar(const char* name);
FieldList returnvar(const char* name);
FieldList returnvarno(const int var_no);
FuncList returnfun(const char* name);
StructList returnstruct(const char* name);
void pushstack();
void popstack();
#include <assert.h>
#include <stdlib.h>
struct ListHead {
	struct ListHead *prev, *next;
};
typedef struct ListHead ListHead;

#define list_entry(ptr, type, member) \
	((type*)((char*)(ptr) - (int)(&((type*)0)->member)))

static inline void
list_add(ListHead *prev, ListHead *next, ListHead *data) {
	data->prev = prev;
	data->next = next;
	if (prev != NULL) prev->next = data;
	if (next != NULL) next->prev = data;
}

static inline void
list_add_before(ListHead *list, ListHead *data) {
	assert(list != NULL);
	list_add(list->prev, list, data);
}

static inline void
list_add_after(ListHead *list, ListHead *data) {
	list_add(list, list->next, data);
}

static inline void
list_del(ListHead *data) {
	assert(data != NULL);
	ListHead *prev = data->prev;
	ListHead *next = data->next;
	if (prev != NULL) prev->next = next;
	if (next != NULL) next->prev = prev;
}

static inline void
list_init(ListHead *list) {
	list->prev = list->next = list;
}

static inline int
list_empty(ListHead *list) {
	return list == list->next;
}

#define list_foreach(ptr, head) \
	for ((ptr) = (head)->next; (ptr) != (head); (ptr) = (ptr)->next)
#endif