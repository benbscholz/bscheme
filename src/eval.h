#ifndef EVAL_H
#define EVAL_H

char is_self_evaluating(object *exp);
char is_tagged_list(object *expression, object *tag);
char is_quoted(object *expression);
object *text_of_quotation(object *exp);
object *eval(object *exp, object *env);
object *eval_proc(object *arguments);
object *eval_expression(object *arguments);
object *eval_environment(object *arguments);


#endif