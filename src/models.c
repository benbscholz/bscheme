#include "scheme.h"
#include "environment.h"
#include "util.h"
#include "io.h"

object *alloc_object(void) {
	object *obj;
	obj = malloc(sizeof(object));
	if (obj == NULL) {
		crash_error("out of memory");
	}
	return obj;
}

object *make_fixnum(long value) {
	object *obj;
	obj = alloc_object();
	obj->type = FIXNUM;
	obj->data.fixnum.value = value;
	return obj;
}

char is_fixnum(object *obj) {
	return obj->type == FIXNUM;
}

char is_boolean(object *obj) {
	return obj->type == BOOLEAN;
}

char is_false(object *obj) {
	return obj == false;
}

char is_true(object *obj) {
	return !is_false(obj);
}

object *make_character(char value) {
	object *obj;
	obj = alloc_object();
	obj->type = CHARACTER;
	obj->data.character.value = value;
	return obj;
}

char is_character(object *obj) {
	return obj->type == CHARACTER;
}

object *make_string(char *value) {
	object *obj;
	
	obj = alloc_object();
	obj->type = STRING;
	obj->data.string.value = malloc(strlen(value) + 1);
	if (obj->data.string.value == NULL) {
		crash_error("out of memory");
	}
	strcpy(obj->data.string.value, value);
	return obj;
}

char is_string(object *obj) {
	return obj->type == STRING;
}

char is_the_empty_list(object *obj) {
	return obj == the_empty_list;
}

object *cons(object *car, object *cdr) {
	object *obj;
	obj = alloc_object();
	obj->type = PAIR;
	obj->data.pair.car = car;
	obj->data.pair.cdr = cdr;
	return obj;
}

char is_pair(object *obj) {
	return obj->type == PAIR;
}

object *car(object *pair) {
	return pair->data.pair.car;	
}

object *cdr(object *pair) {
	return pair->data.pair.cdr;
}

void set_car(object *obj, object *value) {
	obj->data.pair.car = value;
}

void set_cdr(object *obj, object *value) {
	obj->data.pair.cdr = value;
}

object *make_symbol(char *value) {
	object *obj;
	object *element;
	
	element = symbol_table;
	while (!is_the_empty_list(element)) {
		if (strcmp(car(element)->data.symbol.value, value) == 0) {
			return car(element);
		}
		element = cdr(element);
	}
	
	obj = alloc_object();
	obj->type = SYMBOL;
	obj->data.symbol.value = malloc(strlen(value) + 1);
	if (obj->data.symbol.value == NULL) {
		crash_error("out of memory");
	}
	strcpy(obj->data.symbol.value, value);
	symbol_table = cons(obj, symbol_table);
	return obj;
}

char is_symbol(object *obj) {
	return obj->type == SYMBOL;
}

void init(void) {
	the_empty_list = alloc_object();
	the_empty_list->type = THE_EMPTY_LIST;
	
	false = alloc_object();
	false->type = BOOLEAN;
	false->data.boolean.value = 0;
	
	true = alloc_object();
	true->type = BOOLEAN;
	true->data.boolean.value = 1;
	
	symbol_table = the_empty_list;
	quote_symbol = make_symbol("quote");
	define_symbol = make_symbol("define");
	set_symbol = make_symbol("set!");
	ok_symbol = make_symbol("ok");
	if_symbol = make_symbol("if");
	begin_symbol = make_symbol("begin");
	cond_symbol = make_symbol("cond");
	else_symbol = make_symbol("else");
	let_symbol = make_symbol("let");
	lambda_symbol = make_symbol("lambda");
	and_symbol = make_symbol("and");
	or_symbol = make_symbol("or");
	
	eof_object = alloc_object();
    eof_object->type = EOF_OBJECT;
	
	the_empty_environment = the_empty_list;
	the_global_environment = make_environment();
    
}




















