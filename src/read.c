#include "scheme.h"
#include "models.h"
#include "util.h"

object *read(FILE *in);

char is_delimiter(int c) {
	return isspace(c) || c == EOF ||
		   c == '('   || c == ')' ||
		   c == '"'   || c == ';' ||
		   c == '['   || c == ']';
}

char is_initial(int c) {
	return isalpha(c) || c == '*' || c == '/' || c == '>' ||
		     c == '<' || c == '=' || c == '?' || c == '!' || c == '%';
}

int peek(FILE *in) {
	int c;
	c = getc(in);
	ungetc(c, in);
	return c;
}

void eat_whitespace(FILE *in) {
	int c;
	
	while ((c = getc(in)) != EOF) {
		if (isspace(c)) {
			continue;
		} else if (c == ';') {
			while (((c = getc(in)) != EOF) && (c != '\n'));
			continue;
		}
		ungetc(c, in);
		break;
	}
}

void eat_expected_string(FILE *in, char *str) {
	int c;
	
	while (*str != '\0') {
		c = getc(in);
		if (c != *str) {
			crash_error("unexpected character");
		}
		str++;
	}
}

void peek_expected_delimiter(FILE *in) {
	if (!is_delimiter(peek(in))) {
		crash_error("character not followed by delimiter");
	}
}

object *read_character(FILE *in) {
	int c;
	
	c = getc(in);
	switch (c) {
		case EOF:
			crash_error("incomplete character literal");
		case 's':
			if(peek(in) == 'p') {
				eat_expected_string(in, "pace");
				peek_expected_delimiter(in);
				return make_character(' ');
			}
			break;
		case 'n':
			if (peek(in) == 'e') {
				eat_expected_string(in, "ewline");
				peek_expected_delimiter(in);
				return make_character('\n');
			}
			break;
	}
	peek_expected_delimiter(in);
	return make_character(c);
}

object *read_pair(FILE *in) {
	int c;
	object *car_obj;
	object *cdr_obj;
	
	eat_whitespace(in);
	c = getc(in);
	
	if (c == ')' || c == ']') {
		return the_empty_list;
	}
	ungetc(c, in);
	
	car_obj = read(in);
	
	eat_whitespace(in);
	
	c = getc(in);
	
	if (c == '.') {
		c = peek(in);
		if (!is_delimiter(c)) {
			crash_error("dot not followed by delimiter");
		}
		cdr_obj = read(in);
		eat_whitespace(in);
		c = getc(in);
		if (c != ')' || c!= ']') {
			crash_error("missing trailing right paren");
		}
		return cons(car_obj, cdr_obj);
	} else {
		ungetc(c, in);
		cdr_obj = read_pair(in);
		return cons(car_obj, cdr_obj);
	}
}

object *read(FILE *in) {
	int c;
	int i;
	short sign = 1;
	long num = 0;
	char buffer[BUFFER_MAX];
	
	eat_whitespace(in);
	c = getc(in);
	
	if (c == '#') {
		c = getc(in);
		switch (c) {
			case 't':
				return true;
			case 'f':
				return false;
			case '\\':
				return read_character(in);
			default:
				crash_error("unknown boolean literal");
		}
	} else if (isdigit(c) || (c == '-' && isdigit(peek(in)))) {
		if (c == '-') {
			sign = -1;
		} else {
			ungetc(c, in);
		}
		while (isdigit(c = getc(in))) {
			num = (num * 10) + (c - '0');
		}
		num *= sign;
		if (is_delimiter(c)) {
			ungetc(c, in);
			return make_fixnum(num);
		} else {
			crash_error("number not followed by delimiter");
		}
	} else if (is_initial(c) || ((c == '+' || c == '-') && is_delimiter(peek(in)))) {
		i = 0;
		while (is_initial(c) || isdigit(c) || c == '+' || c == '-') {
			if (i < BUFFER_MAX - 1) {
				buffer[i++] = c;
			} else {
				crash_error("symbol too long. exceed BUFFER_MAX");
			}
			c = getc(in);
		}
		if (is_delimiter(c)) {
			buffer[i] = '\0';
			ungetc(c, in);
			return make_symbol(buffer);
		} else {
			crash_error("symbol not followed by delimiter");
		}
	} else if (c == '"') {
		i = 0;
		
		while ((c = getc(in)) != '"') {
			if (c == '\\') {
				c = getc(in);
				if (c == 'n') {
					c = '\n';
				}
			}
			if (c == EOF) {
				crash_error("non-terminated string literal");
			}
			if (i < BUFFER_MAX - 1) {
				buffer[i++] = c;
			} else {
				crash_error("string too long. exceeded MAX_BUFFER");
			}
		}
		buffer[i] = '\0';
		return make_string(buffer);
	} else if (c == '(' || c == '[') {
		return read_pair(in);
	} else if (c == '\'') {
		return cons(quote_symbol, cons(read(in), the_empty_list));
	} else if (c == EOF) {
		return NULL;
	} else {
		crash_error("bad input. Unexpected input.");
	}
	crash_error("read illegal state");
	exit(1);
}