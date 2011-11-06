#ifndef ENVIRON_H
#define ENVIRON_H

object *enclosing_environment(object *env);
object *first_frame(object *env);
object *make_frame(object *vars, object *vals);
object *frame_variables(object *frame);
object *frame_values(object *frame);
void add_binding_to_frame(object *var, object *val, object *frame);
object *extend_environment(object *vars, object *vals, object *base_env);
object *lookup_variable_value(object *var, object *env);
void set_variable_value(object *var, object *val, object *env);
void define_variable(object *var, object *val, object *env);
object *setup_environment(void);
object *interaction_environment_proc(object *arguments);
object *make_environment(void);

#endif