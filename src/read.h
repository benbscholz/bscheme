#ifndef READ_H
#define READ_H

char is_delimiter(int c);
int peek(FILE *in);
void eat_whitespace(FILE *in);
void eat_expected_string(FILE *in, char *str);
void peek_expected_delimiter(FILE *in);
object *read_character(FILE *in);
object *read(FILE *in);

#endif