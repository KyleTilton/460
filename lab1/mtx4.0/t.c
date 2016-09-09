#define SSIZE 1024             // per proc stack area
typedef struct proc{
    struct proc *next;
    int    *ksp;               // saved ksp when not running              // add pid for identify the proc
    int    kstack[SSIZE];      // proc stack area
}PROC;
int  procSize = sizeof(PROC);
PROC proc0, *running;    // define NPROC procs
int scheduler() { running = &proc0; }

main() {
  running = &proc0;
  printf("call tswitch()\n");
  tswitch();
  printf("back to main()\n");
}
