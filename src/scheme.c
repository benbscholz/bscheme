#include "models.h"
#include "read.h"
#include "write.h"
#include "eval.h"
#include "environment.h"
#include "util.h"
#include "io.h"

int main(void) {
	object *exp;
	
	init();
	
	write(stdout, load_proc(cons(make_string("../lib/stdlib.ss"), the_empty_list)));
	printf("\n");		
	
	for (;;) {
		printf("[scheme][λ] ");
		
        exp = read(stdin);
        if (exp == NULL) {
            break;
        }
        write(stdout, eval(exp, the_global_environment));
        printf("\n");
	}
	
	return 0;
}