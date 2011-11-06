#include "scheme.h"
#include "models.h"
#include "eval.h"

#define MAX_OUTPUT 1000

char is_last_exp(object *seq);
object *first_exp(object *seq);
object *make_begin(object *seq);
object *make_application(object *operator, object *operands);

void crash_error(char *message) {
	fprintf(stderr, "%s\n", message);
	exit(1);
}

char is_if(object *exp) {
	return is_tagged_list(exp, if_symbol);
}

object *if_predicate(object *exp) {
	return cadr(exp);
}

object *if_consequent(object *exp) {
	return caddr(exp);
}

object *if_alternative(object *exp) {
	if (is_the_empty_list(cdddr(exp))) {
		return false;
	} else {
		return cadddr(exp);
	}
}

object *make_if(object *predicate, object *consequent, object *alternative) {
	return cons(if_symbol,
				  cons(predicate,
				  	   cons(consequent,
				  	        cons(alternative, the_empty_list))));
}

char is_cond(object *exp) {
    return is_tagged_list(exp, cond_symbol);
}

object *cond_clauses(object *exp) {
    return cdr(exp);
}

object *cond_predicate(object *clause) {
    return car(clause);
}

object *cond_actions(object *clause) {
    return cdr(clause);
}

char is_cond_else_clause(object *clause) {
    return cond_predicate(clause) == else_symbol;
}

object *sequence_to_exp(object *seq) {
    if (is_the_empty_list(seq)) {
        return seq;
    }
    else if (is_last_exp(seq)) {
        return first_exp(seq);
    }
    else {
        return make_begin(seq);
    }
}

object *expand_clauses(object *clauses) {
    object *first;
    object *rest;
    
    if (is_the_empty_list(clauses)) {
        return false;
    }
    else {
        first = car(clauses);
        rest = cdr(clauses);
        if (is_cond_else_clause(first)) {
            if (is_the_empty_list(rest)) {
                return sequence_to_exp(cond_actions(first));
            }
            else {
                fprintf(stderr, "else clause isn't last cond->if");
                exit(1);
            }
        }
        else {
            return make_if(cond_predicate(first),
                           sequence_to_exp(cond_actions(first)),
                           expand_clauses(rest));
        }
    }
}

object *cond_to_if(object *exp) {
    return expand_clauses(cond_clauses(exp));
}

object *make_primitive_proc(object *(*fn)(struct object *arguments)) {
	object *obj;
	obj = alloc_object();
	obj->type = PRIMITIVE_PROC;
	obj->data.primitive_proc.fn = fn;
	return obj;
}

object *make_compound_proc(object *parameters, object *body, object *env) {
	object *obj;
	
	obj = alloc_object();
	obj->type = COMPOUND_PROC;
	obj->data.compound_proc.parameters = parameters;
	obj->data.compound_proc.body = body;
	obj->data.compound_proc.env = env;
	return obj;
}

char is_compound_proc(object *obj) {
	return obj->type == COMPOUND_PROC;
}

object *make_lambda(object *parameters, object *body) {
	return cons(lambda_symbol, cons(parameters, body));
}

char is_lambda(object *exp) {
	return is_tagged_list(exp, lambda_symbol);
}

object *lambda_parameters(object *exp) {
	return cadr(exp);
}

object *lambda_body(object *exp) {
	return cddr(exp);
}

char is_last_exp(object *seq) {
	return is_the_empty_list(cdr(seq));
}

object *first_exp(object *seq) {
	return car(seq);
}

object *rest_exps(object *seq) {
	return cdr(seq);
}

char is_begin(object *exp) {
	return is_tagged_list(exp, begin_symbol);
}

object *begin_actions(object *exp) {
	return cdr(exp);
}

object *make_begin(object *seq) {
	return cons(begin_symbol, seq);
}

char is_application(object *exp) {
	return is_pair(exp); 
}

object *operator(object *exp) {
	return car(exp);
}

object *operands(object *exp) {
	return cdr(exp);
}

char is_no_operands(object *ops) {
	return is_the_empty_list(ops);
}

object *first_operand(object *ops) {
	return car(ops);
}

object *rest_operands(object *ops) {
	return cdr(ops);
}

char is_primitive_proc(object *obj) {
	return obj->type == PRIMITIVE_PROC;
}

char is_let(object *exp) {
    return is_tagged_list(exp, let_symbol);
}

object *let_bindings(object *exp) {
    return cadr(exp);
}

object *let_body(object *exp) {
    return cddr(exp);
}

object *binding_parameter(object *binding) {
    return car(binding);
}

object *binding_argument(object *binding) {
    return cadr(binding);
}

object *bindings_parameters(object *bindings) {
    return is_the_empty_list(bindings) ?
               the_empty_list :
               cons(binding_parameter(car(bindings)),
                    bindings_parameters(cdr(bindings)));
}

object *bindings_arguments(object *bindings) {
    return is_the_empty_list(bindings) ?
               the_empty_list :
               cons(binding_argument(car(bindings)),
                    bindings_arguments(cdr(bindings)));
}

object *let_parameters(object *exp) {
    return bindings_parameters(let_bindings(exp));
}

object *let_arguments(object *exp) {
    return bindings_arguments(let_bindings(exp));
}

object *let_to_application(object *exp) {
    return make_application(
               make_lambda(let_parameters(exp),
                           let_body(exp)),
               let_arguments(exp));
}

object *make_application(object *operator, object *operands) {
	return cons(operator, operands);
}

object *is_null_proc(object *arguments) {
	return is_the_empty_list(car(arguments)) ? true : false;
}

object *is_boolean_proc(object *arguments) {
	return is_boolean(car(arguments)) ? true : false;
}

object *is_symbol_proc(object *arguments) {
	return is_symbol(car(arguments)) ? true : false;
}

object *is_integer_proc(object *arguments) {
	return is_fixnum(car(arguments)) ? true : false;
}

object *is_char_proc(object *arguments) {
	return is_character(car(arguments)) ? true : false;
}

object *is_string_proc(object *arguments) {
	return is_string(car(arguments)) ? true : false;
}

object *is_pair_proc(object *arguments) {
	return is_pair(car(arguments)) ? true : false;
}

object *is_procedure_proc(object *arguments) {
	return is_primitive_proc(car(arguments)) ? true : false;
}


object *char_to_integer_proc(object *arguments) {
    return make_fixnum((car(arguments))->data.character.value);
}

object *integer_to_char_proc(object *arguments) {
    return make_character((car(arguments))->data.fixnum.value);
}

object *number_to_string_proc(object *arguments) {
    char buffer[100];

    sprintf(buffer, "%ld", (car(arguments))->data.fixnum.value);
    return make_string(buffer);
}

object *string_to_number_proc(object *arguments) {
    return make_fixnum(atoi((car(arguments))->data.string.value));
}

object *symbol_to_string_proc(object *arguments) {
    return make_string((car(arguments))->data.symbol.value);
}

object *string_to_symbol_proc(object *arguments) {
    return make_symbol((car(arguments))->data.string.value);
}

object *add_proc(object *arguments) {
    long result = 0;
    
    while (!is_the_empty_list(arguments)) {
        result += (car(arguments))->data.fixnum.value;
        arguments = cdr(arguments);
    }
    return make_fixnum(result);
}

object *sub_proc(object *arguments) {
    long result;
    
    result = (car(arguments))->data.fixnum.value;
    while (!is_the_empty_list(arguments = cdr(arguments))) {
        result -= (car(arguments))->data.fixnum.value;
    }
    return make_fixnum(result);
}

object *mul_proc(object *arguments) {
    long result = 1;
    
    while (!is_the_empty_list(arguments)) {
        result *= (car(arguments))->data.fixnum.value;
        arguments = cdr(arguments);
    }
    return make_fixnum(result);
}

object *quotient_proc(object *arguments) {
    return make_fixnum(
        ((car(arguments) )->data.fixnum.value)/
        ((cadr(arguments))->data.fixnum.value));
}

object *remainder_proc(object *arguments) {
    return make_fixnum(
        ((car(arguments) )->data.fixnum.value)%
        ((cadr(arguments))->data.fixnum.value));
}

object *is_number_equal_proc(object *arguments) {
    long value;
    
    value = (car(arguments))->data.fixnum.value;
    while (!is_the_empty_list(arguments = cdr(arguments))) {
        if (value != ((car(arguments))->data.fixnum.value)) {
            return false;
        }
    }
    return true;
}

object *is_less_than_proc(object *arguments) {
    long previous;
    long next;
    
    previous = (car(arguments))->data.fixnum.value;
    while (!is_the_empty_list(arguments = cdr(arguments))) {
        next = (car(arguments))->data.fixnum.value;
        if (previous < next) {
            previous = next;
        }
        else {
            return false;
        }
    }
    return true;
}

object *is_greater_than_proc(object *arguments) {
    long previous;
    long next;
    
    previous = (car(arguments))->data.fixnum.value;
    while (!is_the_empty_list(arguments = cdr(arguments))) {
        next = (car(arguments))->data.fixnum.value;
        if (previous > next) {
            previous = next;
        }
        else {
            return false;
        }
    }
    return true;
}

object *cons_proc(object *arguments) {
    return cons(car(arguments), cadr(arguments));
}

object *car_proc(object *arguments) {
    return caar(arguments);
}

object *cdr_proc(object *arguments) {
    return cdar(arguments);
}

object *set_car_proc(object *arguments) {
    set_car(car(arguments), cadr(arguments));
    return ok_symbol;
}

object *set_cdr_proc(object *arguments) {
    set_cdr(car(arguments), cadr(arguments));
    return ok_symbol;
}

object *list_proc(object *arguments) {
    return arguments;
}

object *is_eq_proc(object *arguments) {
    object *obj1;
    object *obj2;
    
    obj1 = car(arguments);
    obj2 = cadr(arguments);
    
    if (obj1->type != obj2->type) {
        return false;
    }
    switch (obj1->type) {
        case FIXNUM:
            return (obj1->data.fixnum.value ==
                    obj2->data.fixnum.value) ?
                        true : false;
            break;
        case CHARACTER:
            return (obj1->data.character.value ==
                    obj2->data.character.value) ?
                        true : false;
            break;
        case STRING:
            return (strcmp(obj1->data.string.value,
                           obj2->data.string.value) == 0) ?
                        true : false;
            break;
        default:
            return (obj1 == obj2) ? true : false;
    }
}

char is_and(object *exp) {
    return is_tagged_list(exp, and_symbol);
}

object *and_tests(object *exp) {
    return cdr(exp);
}

char is_or(object *exp) {
    return is_tagged_list(exp, or_symbol);
}

object *or_tests(object *exp) {
    return cdr(exp);
}

object *apply_proc(object *arguments) {
	crash_error("illegal state: the body of the apply primitive procedure should not execute.");
	exit(1);
}

object *apply_operator(object *arguments) {
	return car(arguments);
}

object *prepare_apply_operands(object *arguments) {
    if (is_the_empty_list(cdr(arguments))) {
        return car(arguments);
    } else {
        return cons(car(arguments), prepare_apply_operands(cdr(arguments)));
    }
}

object *apply_operands(object *arguments) {
	return prepare_apply_operands(cdr(arguments));
}

object *exit_proc(object *arguments) {
	exit(0);
}








