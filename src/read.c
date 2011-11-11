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
			fprintf(stderr, "unexpected character: %c\n", c);
			exit(1);
		}
		str++;
	}
}

void peek_expected_delimiter(FILE *in) {
	if (!is_delimiter(peek(in))) {
		fprintf(stderr, "character not followed by delimiter\n");
		exit(1);
	}
}

object *read_character(FILE *in) {
	int c;
	
	c = getc(in);
	switch (c) {
		case EOF:
			fprintf(stderr, "incomplete character literal\n");
			exit(1);
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
			fprintf(stderr, "dot not followed by delimiter\n");
			exit(1);
		}
		cdr_obj = read(in);
		eat_whitespace(in);
		c = getc(in);
		if (c != ')' || c!= ']') {
			fprintf(stderr, "missing trailing right paren\n");
			exit(1);
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
	long dec = 0;
	long temp;
	int len = 0;
	float flnum;
	char buffer[BUFFER_MAX];
	short fl = 0;
	
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
				fprintf(stderr, "unknown boolean literal: %c\n", c);
				exit(1);
		}
	} else if (isdigit(c) || (c == '-' && isdigit(peek(in)))) {
		if (c == '-') {
			sign = -1;
		} else {
			ungetc(c, in);
		}
		while (isdigit(c = getc(in)) || c == '.') {
			if (isdigit(c)) {
				num = (num * 10) + (c - '0');
			} else {
				fl = 1;
				while (isdigit(c = getc(in))) {
					dec = (dec * 10) + (c - '0');
				}
				temp = dec;
				while (temp > 0) {
					len *= 10;
					temp /= 10;
				}
				if (len != 0) {
					flnum = (float) (num + (dec / len));
				} else {
					flnum = (float) (num += dec);
				}
				break;
			}
		}
		
		num *= sign;
		
		if (is_delimiter(c) && fl == 0) {
			ungetc(c, in);
			return make_fixnum(num);
		} else if ((is_delimiter(c) && fl == 1) || c == 'f') {
			if (c != 'f') {
				ungetc(c, in);
			}
			return make_floatnum(num);
		} else {
			fprintf(stderr, "number not followed by delimiter: %d\n", c);
			exit(1);
		}
	} else if (is_initial(c) || ((c == '+' || c == '-') && is_delimiter(peek(in)))) {
		i = 0;
		while (is_initial(c) || isdigit(c) || c == '+' || c == '-') {
			if (i < BUFFER_MAX - 1) {
				buffer[i++] = c;
			} else {
				fprintf(stderr, "symbol too long. exceed BUFFER_MAX\n");
				exit(1);
			}
			c = getc(in);
		}
		if (is_delimiter(c)) {
			buffer[i] = '\0';
			ungetc(c, in);
			return make_symbol(buffer);
		} else {
			fprintf(stderr, "symbol not followed by delimiter: %s\n", buffer);
			exit(1);
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
				fprintf(stderr, "non-terminated string literal\n");
				exit(1);
			}
			if (i < BUFFER_MAX - 1) {
				buffer[i++] = c;
			} else {
				fprintf(stderr, "string too long. exceeded MAX_BUFFER\n");
				exit(1);
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
		fprintf(stderr, "bad input. Unexpected input: %c\n", c);
		exit(1);
	}
	fprintf(stderr, "read illegal state\n");
	exit(1);
}