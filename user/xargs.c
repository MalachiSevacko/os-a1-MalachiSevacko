#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int
main(int argc, char *argv[])
{
  if (argc < 2) {
    fprintf(2, "Usage: xargs command [arguments...]\n");
    exit(1);
  }

  if (argc >= MAXARG) {
    fprintf(2, "xargs: too many arguments\n");
    exit(1);
  }

  char *args[MAXARG];

  // Copy the command and its existing arguments.
  for (int i = 1; i < argc; i++) {
    args[i - 1] = argv[i];
  }

  char buf[512];
  int n = 0;
  char c;
  int result;

  // Read standard input one character at a time.
  while ((result = read(0, &c, 1)) == 1) {
    if (c == '\n') {
      buf[n] = 0;

      // Add the input line as the final argument.
      args[argc - 1] = buf;
      args[argc] = 0;

      int pid = fork();

      if (pid < 0) {
        fprintf(2, "xargs: fork failed\n");
        exit(1);
      }

      if (pid == 0) {
        exec(args[0], args);
        fprintf(2, "xargs: exec failed\n");
        exit(1);
      }

      wait(0);
      n = 0;
    } else {
      if (n >= sizeof(buf) - 1) {
        fprintf(2, "xargs: input line too long\n");
        exit(1);
      }

      buf[n++] = c;
    }
  }

  if (result < 0) {
    fprintf(2, "xargs: read failed\n");
    exit(1);
  }

  // Handle input that doesn't end with a newline.
  if (n > 0) {
    buf[n] = 0;

    args[argc - 1] = buf;
    args[argc] = 0;

    int pid = fork();

    if (pid < 0) {
      fprintf(2, "xargs: fork failed\n");
      exit(1);
    }

    if (pid == 0) {
      exec(args[0], args);
      fprintf(2, "xargs: exec failed\n");
      exit(1);
    }

    wait(0);
  }

  exit(0);
}
