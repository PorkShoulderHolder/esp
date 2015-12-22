#include "env.h"


struct Env new_env(void){
	struct Env e;
	e.var_table = NULL;

	e.queue_handle = NULL;

	e.task_handle = NULL; 
	return e;
}

