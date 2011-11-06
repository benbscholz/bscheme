#include "scheme.h"
#include "models.h"
#include "eval.h"
#include "write.h"
#include "read.h"

object *make_input_port(FILE *in);
char is_input_port(object *obj);
char is_eof_object(object *obj);
char is_output_port(object *obj);

object *load_proc(object *arguments) {
    char *filename;
    FILE *in;
    object *exp;
    object *result;
    
    filename = car(arguments)->data.string.value;
    in = fopen(filename, "r");
    if (in == NULL) {
        fprintf(stderr, "could not load file \"%s\"", filename);
        exit(1);
    }
    while ((exp = read(in)) != NULL) {
        result = eval(exp, the_global_environment);
    }
    fclose(in);
    return result;
}

object *open_input_port_proc(object *arguments) {
    char *filename;
    FILE *in;

    filename = car(arguments)->data.string.value;
    in = fopen(filename, "r");
    if (in == NULL) {
        fprintf(stderr, "could not open file \"%s\"\n", filename);
        exit(1);
    }
    return make_input_port(in);
}

object *close_input_port_proc(object *arguments) {
    int result;
    
    result = fclose(car(arguments)->data.input_port.stream);
    if (result == EOF) {
        fprintf(stderr, "could not close input port\n");
        exit(1);
    }
    return ok_symbol;
}

object *is_input_port_proc(object *arguments) {
    return is_input_port(car(arguments)) ? true : false;
}

object *read_proc(object *arguments) {
    FILE *in;
    object *result;
    
    in = is_the_empty_list(arguments) ?
             stdin :
             car(arguments)->data.input_port.stream;
    result = read(in);
    return (result == NULL) ? eof_object : result;
}

object *read_char_proc(object *arguments) {
    FILE *in;
    int result;
    
    in = is_the_empty_list(arguments) ?
             stdin :
             car(arguments)->data.input_port.stream;
    result = getc(in);
    return (result == EOF) ? eof_object : make_character(result);
}

object *peek_char_proc(object *arguments) {
    FILE *in;
    int result;
    
    in = is_the_empty_list(arguments) ?
             stdin :
             car(arguments)->data.input_port.stream;
    result = peek(in);
    return (result == EOF) ? eof_object : make_character(result);
}

object *is_eof_object_proc(object *arguments) {
    return is_eof_object(car(arguments)) ? true : false;
}

object *make_output_port(FILE *in);

object *open_output_port_proc(object *arguments) {
    char *filename;
    FILE *out;

    filename = car(arguments)->data.string.value;
    out = fopen(filename, "w");
    if (out == NULL) {
        fprintf(stderr, "could not open file \"%s\"\n", filename);
        exit(1);
    }
    return make_output_port(out);
}

object *close_output_port_proc(object *arguments) {
    int result;
    
    result = fclose(car(arguments)->data.output_port.stream);
    if (result == EOF) {
        fprintf(stderr, "could not close output port\n");
        exit(1);
    }
    return ok_symbol;
}

object *is_output_port_proc(object *arguments) {
    return is_output_port(car(arguments)) ? true : false;
}

object *write_char_proc(object *arguments) {
    object *character;
    FILE *out;
    
    character = car(arguments);
    arguments = cdr(arguments);
    out = is_the_empty_list(arguments) ?
             stdout :
             car(arguments)->data.output_port.stream;
    putc(character->data.character.value, out);
    fflush(out);
    return ok_symbol;
}

object *write_proc(object *arguments) {
    object *exp;
    FILE *out;
    
    exp = car(arguments);
    arguments = cdr(arguments);
    out = is_the_empty_list(arguments) ?
             stdout :
             car(arguments)->data.output_port.stream;
    write(out, exp);
    fflush(out);
    return ok_symbol;
}

object *error_proc(object *arguments) {
    while (!is_the_empty_list(arguments)) {
        write(stderr, car(arguments));
        fprintf(stderr, " ");
        arguments = cdr(arguments);
    };
    printf("\nexiting\n");
    exit(1);
}

object *make_input_port(FILE *stream) {
    object *obj;
    
    obj = alloc_object();
    obj->type = INPUT_PORT;
    obj->data.input_port.stream = stream;
    return obj;
}

char is_input_port(object *obj) {
    return obj->type == INPUT_PORT;
}

object *make_output_port(FILE *stream) {
    object *obj;
    
    obj = alloc_object();
    obj->type = OUTPUT_PORT;
    obj->data.output_port.stream = stream;
    return obj;
}

char is_output_port(object *obj) {
    return obj->type == OUTPUT_PORT;
}

char is_eof_object(object *obj) {
    return obj == eof_object;
}