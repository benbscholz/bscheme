#ifndef UTIL_H
#define UTIL_H

char is_if(object *expression);
object *if_predicate(object *exp);
object *if_consequent(object *exp);
object *if_alternative(object *exp);
object *make_if(object *predicate, object *consequent, object *alternative);

char is_cond(object *exp);
object *cond_clauses(object *exp);
object *cond_predicate(object *clause);
object *cond_actions(object *clause);
char is_cond_else_clause(object *clause);
object *sequence_to_exp(object *seq);
object *expand_clauses(object *clauses);
object *cond_to_if(object *exp);

object *make_primitive_proc(object *(*fn)(struct object *arguments));
char is_application(object *exp);
object *operator(object *exp);
object *operands(object *exp);
char is_no_operands(object *ops);
object *first_operand(object *ops);
object *rest_operands(object *ops);

object *make_compound_proc(object *parameters, object *body, object *env);
char is_compound_proc(object *obj);

object *make_lambda(object *parameters, object *body);
char is_lambda(object *exp);
object *lambda_parameters(object *exp);
object *lambda_body(object *exp);
char is_last_exp(object *seq);
object *first_exp(object *seq);
object *rest_exps(object *seq);

char is_let(object *exp);
object *let_bindings(object *exp);
object *let_body(object *exp);
object *binding_parameter(object *binding);
object *binding_argument(object *binding);
object *bindings_parameters(object *bindings);
object *bindings_arguments(object *bindings);
object *let_parameters(object *exp);
object *let_arguments(object *exp);
object *let_to_application(object *exp);

char is_begin(object *exp);
object *begin_actions(object *exp);
object *make_begin(object *seq);

char is_primitive_proc(object *obj);
object *is_null_proc(object *arguments);
object *is_boolean_proc(object *arguments);
object *is_symbol_proc(object *arguments);
object *is_integer_proc(object *arguments);
object *is_float_proc(object *arguments);
object *is_char_proc(object *arguments);
object *is_string_proc(object *arguments);
object *is_pair_proc(object *arguments);
object *is_procedure_proc(object *arguments);

object *char_to_integer_proc(object *arguments);
object *integer_to_char_proc(object *arguments);
object *number_to_string_proc(object *arguments);
object *string_to_number_proc(object *arguments);
object *symbol_to_string_proc(object *arguments);
object *string_to_symbol_proc(object *arguments);
object *string_to_list_proc(object *arguments);
object *list_to_string_proc(object *arguments);

object *add_proc(object *arguments);
object *sub_proc(object *arguments);
object *mul_proc(object *arguments);
object *quotient_proc(object *arguments);
object *remainder_proc(object *arguments);

object *is_number_equal_proc(object *arguments);
object *is_less_than_proc(object *arguments);
object *is_greater_than_proc(object *arguments);
object *sqrt_proc(object *arguments);

object *cons_proc(object *arguments);
object *car_proc(object *arguments);
object *cdr_proc(object *arguments);
object *set_car_proc(object *arguments);
object *set_cdr_proc(object *arguments);
object *list_proc(object *arguments);
object *is_eq_proc(object *arguments);

char is_and(object *exp);
object *and_tests(object *exp);
char is_or(object *exp);
object *or_tests(object *exp);

object *apply_proc(object *arguments);
object *apply_operator(object *arguments);
object *prepare_apply_operands(object *arguments);
object *apply_operands(object *arguments);

object *exit_proc(object *arguments);
object *reload_proc(object *arguments);
object *time_proc(object *arguments);

#endif UTIL_H