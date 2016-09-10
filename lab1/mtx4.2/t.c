#define NPROC 9                // number of PROCs
#define SSIZE 1024             // per proc stack area
#define RED

#define FREE 0
#define READY 1
#define STOP 2
#define DEAD 3

//#include "io.c"

typedef struct proc{
    struct proc *next;
    int    *ksp;               // saved ksp when not running
    int    pid;                // add pid for identify the proc
    int    kstack[SSIZE];      // proc stack area

	int status;				   // This is the proc status: FREE|READ|STOP|DEAD
	int priority;			   // This is the proc scheduling priority
	int ppid;				   // parent pid
}PROC;

PROC proc[NPROC], *running, *freeList, *readyQueue;
int  procSize = sizeof(PROC);

PROC proc[NPROC], *running;    // define NPROC procs
extern int color;

// Enter p into queue by priority
int enqueue(PROC **queue, PROC *p)
{
	PROC *current, *next;

	if (*queue == 0)
	{
		*queue = p;
		(*queue)->next = 0;
	}
	else if (p->priority > (*queue)->priority)
	{
		p->next = *queue;
		*queue = p;
	}
	else
    {
        current = (*queue);
        next = current->next;

        while (current != 0 && p->priority <= next->priority)
        {
            current = next;
            next = next->next;
        }

        current->next = p;
        p->next = next;
    }
}


// return first element removed from queue
PROC *dequeue(PROC **queue)
{
	PROC *p = *queue;
	if (p == 0)
	{
		return;
	}
	else
	{
		*queue = (*queue)->next;
		return p;
	}
}

// return a FREE PROC pointer from list
PROC *get_proc(PROC **list)
{
	PROC *p = *list;
	if (p == 0)
	{
		return;
	}
	else
	{
		*list = (*list)->next;
		return p;
	}
}


int body()
{
	char c;
	printf("proc %d resumes to body()\n", running->pid);
	while(1)
	{
		printf("--------------------------------------------\n");
		printList("freeList  ", freeList);		// Show the freelist (optional)
		printList("readyQueue", readyQueue);	// Show the readyQueue
		printf("--------------------------------------------\n");
		printf("proc %d running: parent=%d", running->pid, running->ppid);
		printf("	enter a char [s|f] : ");
		c = getc(); printf("%c\n", c);
		switch(c)
		{
			case 'f' : do_kfork();	break;
			case 'c' : do_tswitch(); break;
		}
	}
}

// Create a child process, begin from body()
PROC *kfork()
{
	int i;
	PROC *p = get_proc(&freeList);
	if (!p){
		printf("no more PROC, kfork() failed\n");
		return 0;
	}
	p->status = READY;
	p->priority= 1;				// priority = 1 for all PROC except P0
	p->ppid = running->pid;		// parent = running
	/* initialize new proc's kstack[ ] */
	for (i=1; i<10; i++)		// saved CPU registers
		p->kstack[SSIZE-i]=0;	//all 0's
	p->kstack[SSIZE-1] = (int)body;	// resume point = address of body()
	p->ksp = &p->kstack[SSIZE-9];	// proc saved sp
	enqueue(&readyQueue, p);	// enter p into readyQueue by priority
	return p;					// return child PROC pointer
}

int init()
{
   PROC *p;
   int i;

	printf("init ....\n");
	for (i=0; i<NPROC; i++){	// Initialize all PROCs
		p = &proc[i];
		p->pid = i;
		p->status = FREE;
		p->priority = 0;
		p->next = &proc[i+1];
	}

	proc[NPROC-1].next = 0;
	freeList = &proc[0];		// all procs are in freelist
	readyQueue = 0;
	/*** create P0 as running ******/
	p = get_proc(&freeList);	// allocate a PROC from freelist
	p->ppid = 0;				// P0's parent is itself
	p->status = READY;
	running = p;				// P0 is now running
}

int scheduler()
{
  printf("running: %d, running->status %d", running, running->status);
	if (running->status == READY)		// if running is still READY
		enqueue(&readyQueue, running);	// enter it into readyQueue
	running = dequeue(&readyQueue);		// new running
  printf("running: %d, running->status %d", running, running->status);
}



main()
{
  printf("MTX starts in main()\n");
  init();
  kfork();
  while(1){
	if (readyQueue)
		tswitch();
  }
}
/********** kernal command functions **********/
int do_kfork()
{
	PROC *p = kfork();
	if (p == 0) { printf("kfork failed\n"); return -1; }
	printf("proc %d kfork a child\n", running->pid);
	printf("child pid = %d\n", p->pid);
	return p->pid;
}
int do_tswitch() {	tswitch();	};

// print name=list contents
printList(char *name, PROC *list)
{
	PROC *current = list;
	printf("%s = ", name);
	while (current)
	{
		printf("%d-> ", current->pid);
		current = current->next;
	}
	printf("NULL");
	printf("\n");
}
