
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "kernel/uproc.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  struct uproc procs[NPROC];
  int count;

  count = getprocs(procs, NPROC);

  if(count < 0) {
    fprintf(2, "ps: getprocs failed\n");
    exit(1);
  }

  printf("PID\tPPID\tSTATE\tSIZE\tNAME\n");

  for(int i = 0; i < count; i++) {
    printf("%d\t%d\t%d\t%ld\t%s\n",
           procs[i].pid,
           procs[i].ppid,
           procs[i].state,
           procs[i].sz,
           procs[i].name);
  }

  exit(0);
}
