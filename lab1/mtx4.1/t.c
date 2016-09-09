#define NPROC 9                // number of PROCs
#define SSIZE 1024             // per proc stack area
#define RED
/******* PROC status *******/
#define FREE    0
#define READY   1
#define STOP    2
#define DEAD    3



typedef struct proc{
    struct proc *next;
    int    *ksp;               // saved ksp when not running
    int    pid;                // add pid for identify the proc
    int    ppid;               // = parent pid
    int    status;             // = proc status = FREE|READY|STOP|DEAD, etc.
    int    priority;           // = proc scheduling priority
    int    kstack[SSIZE];      // proc stack area
}PROC;

int  procSize = sizeof(PROC);

PROC proc[NPROC], *running, *freeList, *readyQueue;    // define NPROC procs
extern int color;
// #include "io.c"    // include I/O funcions based on getc()/putc()
// #include "queue.c" // implement your own queue functions

/***** Fucntion Headers ****/
// PROC *get_proc(PROC **list);
// int enqueue (PROC **queue, PROC *p);
// PROC *dequeue(PROC **queue);
// printList(char *name, PROC *list);

/****************** queue.c functions ******************/
PROC *get_proc() // return a FREE PROC pointer from list
{
  PROC *p = freeList;
  if (freeList)
    freeList = freeList->next;
  return p;
}

// int put_proc(PROC **list, PROC *p)  // enter p into list
// {
//
// }

int enqueue (PROC **queue, PROC *p)
{
  PROC *q = *queue;
  if(q == 0 || p->priority > q->priority)
  {
    *queue = p;
    p->next;
    return;
  }
  while(q->next && q->next->priority >= p->priority)
  {
    q = q->next;
  }
  p->next = q->next;
  q->next = p;
}

PROC *dequeue(PROC **queue)  //return first element removed from queue
{
  PROC *p = *queue;
  if(p)
  {
    *queue = (*queue)->next;
  }
  return p;
}

int printList(char *name, PROC *p)
{
   printf("%s = ", name);
   while(p){
       printf("%d --> ", p->pid);
       p = p->next;
   }
   printf("NULL\n");
}

// printList(char *name, PROC *list)  //
// {
//   PROC *p = list;
//   printf("%s = \n", name);
//   while (p)
//   {
//     printf("pid: %d, ppid: %d, status: %d, priority: %d\n", p->pid, p->ppid, p->status, p->priority);
//     p = p->next;
//   }
// }



/***************** t.c **********************/

int body()
{
   char c;
   printf("proc %d resumes to body()\n", running->pid);
   while(1){
     color = running->pid + 7;
     printList("freelist  ", freeList); // optional: show the freeList
     printList("readyQueue", readyQueue); // show the readyQueue
     printf("proc %d running : enter a key : ", running->pid);
     c = getc();
     printf("%c\n", c);
     switch (c) {
       case 'f' : do_kfork();     break;
       case 's' : do_tswitch();   break;
     }
   }
}

PROC *kfork() // create a child process begin from body()
{
  int i;
  PROC *p = get_proc();
  if (!p) {
    printf("no more PROC, kfork() failed\n");
    return 0;
  }
  p->status = READY;
  p->priority = 1;
  p->ppid = running->pid;
  //p->parent = running;
  p->kstack[SSIZE-1] = (int)body;
  for (i = 1; i < 10; i++)
    p->kstack[SSIZE-i] = 0;
  p->kstack[SSIZE-1] = (int)body;
  p->ksp = &p->kstack[SSIZE-9];
  enqueue (&readyQueue, p);
  return p;
}

/************* kernel command functions ***************/
int do_kfork()
{
  PROC *p = kfork();
  if (p == 0) { printf("kfork failed\n"); return -1;}
  printf("PROC %d kfork a child %d\n", running->pid, p->pid );
  return p->pid;
}

int do_tswitch()
{
  tswitch();
}

int init()
{
   PROC *p;
   int i, j;
   printf("init ....\n");
   /* initialize all proc's */
   for (i=0; i<NPROC; i++){
       p = &proc[i];
       p->pid = i;
       p->status = FREE;
       p->priority = 0;                       // pid = 0,1,2,..NPROC-1
       p->next = &proc[i+1];              // point to next proc
    }
    proc[NPROC-1].next = 0;
    freeList = &proc[0];
    readyQueue = 0;
    /**** create P0 as running ******/
    p = get_proc();
    p->ppid = 0;
    p->status = READY;
    running = p;
 }

int scheduler()
{
  printf("scheduler called\n");
  if (running->status == READY)
    enqueue (&readyQueue, running);
  running = dequeue(&readyQueue);
}




main()
{
  printf("MTX starts in main()\n");
  init();
  printf("real?\n");
  kfork();
  while(1){
    printf("proc 0  running\n");
    if(readyQueue)
    {
      printf("readyQueue thing happened\n");
      tswitch();
    }

  }
}
