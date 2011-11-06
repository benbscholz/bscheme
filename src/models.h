#ifndef MODELS_H
#define MODELS_H

#include "scheme.h"

#define BUFFER_MAX 1000

void init(void);

object *alloc_object(void);
object *make_fixnum(long value);
char is_fixnum(object *obj);

char is_boolean(object *obj);
char is_false(object *obj);
char is_true(object *obj);

object *make_character(char value);
char is_character(object *obj);

object *make_string(char *value);
char is_string(object *obj);

char is_the_empty_list(object *obj);

object *cons(object *car, object *cdr);
char is_pair(object *obj);
object *car(object *pair);
object *cdr(object *pair);
void set_car(object *obj, object *value);
void set_cdr(object *obj, object *value);

object *make_symbol(char *value);
char is_symbol(object *obj);

#endif