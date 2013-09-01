/*
 * sleep and wakeup (condition variables)
 */

typedef struct {
	QLock	*l;
	Tasklist waiting;
} rendez;

void	tasksleep(Rendez*);
int	taskwakeup(Rendez*);
int	taskwakeupall(Rendez*);
