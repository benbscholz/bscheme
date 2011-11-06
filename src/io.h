#ifndef IO_H
#define IO_H

#include "scheme.h"

object *load_proc(object *arguments);
object *open_input_port_proc(object *arguments);
object *close_input_port_proc(object *arguments);
object *is_input_port_proc(object *arguments);
object *read_proc(object *arguments);
object *read_char_proc(object *arguments);
object *peek_char_proc(object *arguments);
object *is_eof_object_proc(object *arguments);
object *open_output_port_proc(object *arguments);
object *close_output_port_proc(object *arguments);
object *is_output_port_proc(object *arguments);
object *write_char_proc(object *arguments);
object *write_proc(object *arguments);
object *error_proc(object *arguments);
object *make_input_port(FILE *stream);
char is_input_port(object *obj);
object *make_output_port(FILE *stream);
char is_output_port(object *obj);
char is_eof_object(object *obj);

#endif