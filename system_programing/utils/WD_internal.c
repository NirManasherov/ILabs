enum {FALSE = 0, TRUE = 1};

#define UNUSED(x) (void)(x)

typedef struct
{
	int argc; 
	char **argv; 
	size_t interval;
	size_t max_misses;
	pid_t *partner_pid; 
	int *is_child; 

}wd_params_ty; 

typedef struct
{
	wd_params_ty wd_params; 
	scheduler_ty *sched;
}wd_task_ty; 

int Watch(wd_params_ty *watchdog_params); 
