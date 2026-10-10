
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

int
match(char *pattern, char *name)
{
  if (*pattern == '\0')
    return *name == '\0';

  if (*pattern == '*') {
    // Match zero characters or consume one character.
    return match(pattern + 1, name) ||
           (*name != '\0' && match(pattern, name + 1));
  }

  if (*name != '\0' && *pattern == *name)
    return match(pattern + 1, name + 1);

  return 0;
}

void
find(char *path, char *target)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if ((fd = open(path, O_RDONLY)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  char *name = path;

  for (char *q = path; *q; q++) {
    if (*q == '/')
      name = q + 1;
  }

  if (match(target, name))
  printf("%s\n", path);

  if (st.type == T_DIR) {
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
      fprintf(2, "find: path too long\n");
      close(fd);
      return;
    }

    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';

    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
      if (de.inum == 0)
        continue;

      if (strcmp(de.name, ".") == 0 ||
          strcmp(de.name, "..") == 0)
        continue;

      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;

      find(buf, target);
    }
  }

  close(fd);
}

int
main(int argc, char *argv[])
{
  if (argc != 3) {
    fprintf(2, "Usage: find path filename\n");
    exit(1);
  }

  find(argv[1], argv[2]);
  exit(0);
}
