#include "scheme.h"
#include "util.h"
#include "models.h"
#include "environment.h"

object *eval(object *exp, object *env);
object *eval_proc(object *arguments);
object *eval_expression(object *arguments);
object *eval_environment(object *arguments);

char is_self_evaluating(object *exp) {
	return is_boolean(exp)   ||
		   is_fixnum(exp)    ||
		   is_character(exp) ||
		   is_string(exp);
}

char is_variable(object *expression) {
	return is_symbol(expression);
}

char is_tagged_list(object *expression, object *tag) {
	object *the_car;
	
	if (is_pair(expression)) {
		the_car = car(expression);
		return is_symbol(the_car) && (the_car == tag);
	}
	return 0;
}

char is_quoted(object *expression) {
	return is_tagged_list(expression, quote_symbol);
}

object *text_of_quotation(object *exp) {
	return cadr(exp);
}

char is_assignment(object *exp) {
	return is_tagged_list(exp, set_symbol);
}

object *assignment_variable(object *exp) {
	return car(cdr(exp));
}

object *assignment_value(object *exp) {
	return car(cdr(cdr(exp)));
}

char is_definition(object *exp) {
	return is_tagged_list(exp, define_symbol);
}

object *definition_variable(object *exp) {
    if (is_symbol(cadr(exp))) {
        return cadr(exp);
    } else {
        return caadr(exp);
    }
}

object *definition_value(object *exp) {
    if (is_symbol(cadr(exp))) {
        return caddr(exp);
    } else {
        return make_lambda(cdadr(exp), cddr(exp));
    }
}

object *list_of_values(object *exps, object *env) {
	if (is_no_operands(exps)) {
		return the_empty_list;
	} else {
		return cons(eval(first_operand(exps), env),
					list_of_values(rest_operands(exps), env));
	}
}

object *eval_assignment(object *exp, object *env) {
	set_variable_value(assignment_variable(exp),
					   eval(assignment_value(exp), env),
					   env);
	return ok_symbol;
}

object *eval_definition(object *exp, object *env) {
	define_variable(definition_variable(exp),
				    eval(definition_value(exp), env),
				    env);
	return ok_symbol;
}

object *eval(object *exp, object *env) {
	object *procedure;
	object *arguments;
	object *result;
	
tailcall:
	if (is_self_evaluating(exp)) {
		return exp;
	} else if (is_variable(exp)) {
		return lookup_variable_value(exp, env);
	} else if (is_quoted(exp)) {
		return text_of_quotation(exp);
	} else if (is_assignment(exp)) {
		return eval_assignment(exp, env);
	} else if (is_definition(exp)) {
		return eval_definition(exp, env);
	} else if (is_if(exp)) {
		exp = is_true(eval(if_predicate(exp), env)) ?
				if_consequent(exp) :
				if_alternative(exp);
		goto tailcall;
	} else if (is_lambda(exp)) {
		return make_compound_proc(lambda_parameters(exp),
								  lambda_body(exp),
								  env);
	} else if (is_begin(exp)) {
		exp = begin_actions(exp);
		while (!is_last_exp(exp)) {
			eval(first_exp(exp), env);
			exp = rest_exps(exp);
		}
		exp = first_exp(exp);
		goto tailcall;
	} else if (is_cond(exp)) {
		exp = cond_to_if(exp);
		goto tailcall;
	} else if (is_let(exp)) {
		exp = let_to_application(exp);
		goto tailcall;
	} else if (is_and(exp)) {
		exp = and_tests(exp);
		if (is_the_empty_list(exp)) {
			return true;
		}
		while (!is_last_exp(exp)) {
			result = eval(first_exp(exp), env);
			if (is_false(result)) {
				return result;
			}
			exp = rest_exps(exp);
		}
		exp = first_exp(exp);
		goto tailcall;
	} else if (is_or(exp)) {
        exp = or_tests(exp);
        if (is_the_empty_list(exp)) {
            return false;
        }
        while (!is_last_exp(exp)) {
            result = eval(first_exp(exp), env);
            if (is_true(result)) {
                return result;
            }
            exp = rest_exps(exp);
        }
        exp = first_exp(exp);
        goto tailcall;
	} else if (is_application(exp)) {
		procedure = eval(operator(exp), env);
		arguments = list_of_values(operands(exp), env);
		
		if (is_primitive_proc(procedure) && procedure->data.primitive_proc.fn == eval_proc) {
            exp = eval_expression(arguments);
            env = eval_environment(arguments);
        	goto tailcall;
        }
		if (is_primitive_proc(procedure) && procedure->data.primitive_proc.fn == apply_proc) {
            procedure = apply_operator(arguments);
            arguments = apply_operands(arguments);
        }
		if (is_primitive_proc(procedure)) {
			return (procedure->data.primitive_proc.fn)(arguments);
		} else if (is_compound_proc(procedure)) {
			env = extend_environment(procedure->data.compound_proc.parameters,
									 arguments,
									 procedure->data.compound_proc.env);
			exp = procedure->data.compound_proc.body;
			while (!is_last_exp(exp)) {
				eval(first_exp(exp), env);
				exp = rest_exps(exp);
			}
			exp = first_exp(exp);
			goto tailcall;
		} else {
			crash_error("unknown procedure type");
		}
	} else {
		crash_error("cannot eval unknown expression type");
	}
	crash_error("eval illegal state");
	exit(1);
}

object *eval_proc(object *arguments) {
	crash_error("illegal state: the body of the eval primitive procedure should not execute");
	exit(1);
}

object *eval_expression(object *arguments) {
    return car(arguments);
}

object *eval_environment(object *arguments) {
    return cadr(arguments);
}














