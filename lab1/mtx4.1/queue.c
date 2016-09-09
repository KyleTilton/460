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

PROC *get_proc(PROC **list) // return a FREE PROC pointer from list
{
  PROC *p = list;
  if (list)
    list = list->next;
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
  if(p != null)
  {
    *queue = (*queue)->next;
  }
  return p;
}

printList(char *name, PROC *list)  //
{
  PROC *p = list;
  printf("%s = ", name);
  while (p)
  {
    printf("pid: %d, ppid: %d, status: %d, priority: %d", p->pid, p->ppid, p->status, p->priority);
    p = p->next;
  }
}
