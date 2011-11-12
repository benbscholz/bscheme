#include "scheme.h"
#include "models.h"
#include "eval.h"
#include "util.h"
#include "io.h"

void populate_environment(object *env);
object *make_environment(void);

object *enclosing_environment(object *env) {
	return cdr(env);
}

object *first_frame(object *env) {
	return car(env);
}

object *make_frame(object *vars, object *vals) {
	return cons(vars, vals);
}

object *frame_variables(object *frame) {
	return car(frame);
}

object *frame_values(object *frame) {
	return cdr(frame);
}

void add_binding_to_frame(object *var, object *val, object *frame) {
    set_car(frame, cons(var, car(frame)));
    set_cdr(frame, cons(val, cdr(frame)));
}

object *extend_environment(object *vars, object *vals, object *base_env) {
	return cons(make_frame(vars, vals), base_env);
}

object *lookup_variable_value(object *var, object *env) {
	object *frame;
	object *vars;
	object *vals;
	
	while (!is_the_empty_list(env)) {
		frame = first_frame(env);
		vars = frame_variables(frame);
		vals = frame_values(frame);
		
		while (!is_the_empty_list(vars)) {
			if (var == car(vars)) {
				return car(vals);
			}
			vars = cdr(vars);
			vals = cdr(vals);
		}
		env = enclosing_environment(env);
	}
	fprintf(stderr, "lookup: unbound variable -> %s\n", var->data.string.value);
	exit(1);
}

void set_variable_value(object *var, object *val, object *env) {
	object *frame;
	object *vars;
	object *vals;
	
	while (!is_the_empty_list(env)) {
		frame = first_frame(env);
		vars = frame_variables(frame);
		vals = frame_values(frame);
		
		while (!is_the_empty_list(vars)) {
			if (var == car(vars)) {
				set_car(vals, val);
				return;
			}
			vars = cdr(vars);
			vals = cdr(vals);
		}
		env = enclosing_environment(env);
	}
	fprintf(stderr, "set: unbound variable -> %s\n", var->data.string.value);
	exit(1);
}

void define_variable(object *var, object *val, object *env) {
	object *frame;
	object *vars;
	object *vals;
	
	frame = first_frame(env);
	vars = frame_variables(frame);
	vals = frame_values(frame);
	
	while (!is_the_empty_list(vars)) {
		if (var == car(vars)) {
			set_car(vals, val);
			return;
		}
		vars = cdr(vars);
		vals = cdr(vals);
	}
	add_binding_to_frame(var, val, frame);
}

object *setup_environment(void) {
	object *initial_env;
	
	initial_env = extend_environment(the_empty_list, 
								     the_empty_list, 
								     the_empty_environment);
	return initial_env;
}

object *interaction_environment_proc(object *arguments) {
    return the_global_environment;
}

object *null_environment_proc(object *arguments) {
    return setup_environment();
}

object *environment_proc(object *arguments) {
    return make_environment();
}

object *make_environment(void) {
    object *env;
    env = setup_environment();
    populate_environment(env);
    return env;
}

void populate_environment(object *env) {
	
	#define add_procedure(scheme_name, c_name) define_variable(make_symbol(scheme_name), make_primitive_proc(c_name), env);

    add_procedure("null?" , is_null_proc);
    add_procedure("boolean?" , is_boolean_proc);
    add_procedure("symbol?" , is_symbol_proc);
    add_procedure("integer?" , is_integer_proc);
    add_procedure("float?", is_float_proc);
    add_procedure("char?" , is_char_proc);
    add_procedure("string?" , is_string_proc);
    add_procedure("pair?" , is_pair_proc);
    add_procedure("procedure?" , is_procedure_proc);
    
    add_procedure("char->integer" , char_to_integer_proc);
    add_procedure("integer->char" , integer_to_char_proc);
    add_procedure("number->string", number_to_string_proc);
    add_procedure("string->number", string_to_number_proc);
    add_procedure("symbol->string", symbol_to_string_proc);
    add_procedure("string->symbol", string_to_symbol_proc);
    add_procedure("string->list", string_to_list_proc);
    add_procedure("list->string", list_to_string_proc);
      
    add_procedure("+" , add_proc);
    add_procedure("-" , sub_proc);
    add_procedure("*" , mul_proc);
    add_procedure("/" , quotient_proc);
    add_procedure("%", remainder_proc);
    add_procedure("=" , is_number_equal_proc);
    add_procedure("<" , is_less_than_proc);
    add_procedure(">" , is_greater_than_proc);
    add_procedure("sqrt", sqrt_proc);
    add_procedure("max", max_proc);
    add_procedure("min", min_proc);

    add_procedure("cons" , cons_proc);
    add_procedure("car" , car_proc);
    add_procedure("cdr" , cdr_proc);
    add_procedure("set-car!", set_car_proc);
    add_procedure("set-cdr!", set_cdr_proc);
    add_procedure("list" , list_proc);

    add_procedure("eq?", is_eq_proc);

    add_procedure("apply", apply_proc);
    
    add_procedure("interaction-environment", interaction_environment_proc);
    add_procedure("null-environment", null_environment_proc);
    add_procedure("environment", environment_proc);
    add_procedure("eval", eval_proc);
	
	
    add_procedure("load" , load_proc);
    add_procedure("open-input-port" , open_input_port_proc);
    add_procedure("close-input-port" , close_input_port_proc);
    add_procedure("input-port?" , is_input_port_proc);
    add_procedure("read" , read_proc);
    add_procedure("read-char" , read_char_proc);
    add_procedure("peek-char" , peek_char_proc);
    add_procedure("eof-object?" , is_eof_object_proc);
    add_procedure("open-output-port" , open_output_port_proc);
    add_procedure("close-output-port", close_output_port_proc);
    add_procedure("output-port?" , is_output_port_proc);
    add_procedure("write-char" , write_char_proc);
    add_procedure("write" , write_proc);

    add_procedure("error", error_proc);
    add_procedure("exit", exit_proc);
    add_procedure("reload", reload_proc);
    add_procedure("time", time_proc);
}






