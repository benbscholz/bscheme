#ifndef SCHEME_H
#define SCHEME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

typedef enum {THE_EMPTY_LIST, BOOLEAN, SYMBOL, FIXNUM,	
			  FLOATNUM, CHARACTER, STRING, PAIR, PRIMITIVE_PROC,
			  COMPOUND_PROC, INPUT_PORT, OUTPUT_PORT,
              EOF_OBJECT} object_type;

typedef struct object {
    object_type type;
    union {
	    struct {
		    char value;
	    } boolean;
	    struct {
		    char *value;
	    } symbol;
        struct {
            long value;
        } fixnum;
        struct {
	        float value;
        } floatnum;
        struct {
	        char value;
        } character;
        struct {
	        char *value;
        } string;
        struct {
	        struct object *car;
	        struct object *cdr;
        } pair;
        struct {
	        struct object *(*fn)(struct object *arguments);
        } primitive_proc;
        struct {
	        struct object *parameters;
	        struct object *body;
	        struct object *env;
        } compound_proc;
        struct {
            FILE *stream;
        } input_port;
        struct {
            FILE *stream;
        } output_port;
    } data;
} object;

object *the_empty_list;
object *symbol_table;

object *define_symbol;
object *quote_symbol;
object *set_symbol;
object *ok_symbol;
object *if_symbol;
object *begin_symbol;
object *cond_symbol;
object *else_symbol;
object *let_symbol;
object *lambda_symbol;
object *and_symbol;
object *or_symbol;
object *eof_object;

object *the_empty_environment;
object *the_global_environment;

object *false;
object *true;


#define caar(obj) car(car(obj))
#define cadr(obj) car(cdr(obj))
#define cdar(obj) cdr(car(obj))
#define cddr(obj) cdr(cdr(obj))
#define caaar(obj) car(car(car(obj)))
#define caadr(obj) car(car(cdr(obj)))
#define cadar(obj) car(cdr(car(obj)))
#define caddr(obj) car(cdr(cdr(obj)))
#define cdaar(obj) cdr(car(car(obj)))
#define cdadr(obj) cdr(car(cdr(obj)))
#define cddar(obj) cdr(cdr(car(obj)))
#define cdddr(obj) cdr(cdr(cdr(obj)))
#define caaaar(obj) car(car(car(car(obj))))
#define caaadr(obj) car(car(car(cdr(obj))))
#define caadar(obj) car(car(cdr(car(obj))))
#define caaddr(obj) car(car(cdr(cdr(obj))))
#define cadaar(obj) car(cdr(car(car(obj))))
#define cadadr(obj) car(cdr(car(cdr(obj))))
#define caddar(obj) car(cdr(cdr(car(obj))))
#define cadddr(obj) car(cdr(cdr(cdr(obj))))
#define cdaaar(obj) cdr(car(car(car(obj))))
#define cdaadr(obj) cdr(car(car(cdr(obj))))
#define cdadar(obj) cdr(car(cdr(car(obj))))
#define cdaddr(obj) cdr(car(cdr(cdr(obj))))
#define cddaar(obj) cdr(cdr(car(car(obj))))
#define cddadr(obj) cdr(cdr(car(cdr(obj))))
#define cdddar(obj) cdr(cdr(cdr(car(obj))))
#define cddddr(obj) cdr(cdr(cdr(cdr(obj))))



#endif