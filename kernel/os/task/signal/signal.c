#include "signal.h"


static void signal_kill(void* arg);
static void signal_child(void* arg);
void handle_signal(signal_t* signal)
{
	switch (signal->type) {
	case SIGNAL_KILL:
		signal_kill(signal->arg);
		break;
	case SIGNAL_CHILD:
		signal_child(signal->arg);
		break;
	}
}



static void signal_kill(void* arg)
{
	
}

static void signal_child(void* arg)
{

}

