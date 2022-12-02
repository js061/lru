#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void f_open_cnt(char *name) {
  int cnt;
  FILE *fp;
  char ch;
  cnt = 0;
  fp = fopen(name, "r");
  if (fp == NULL) {
    printf("file can't be opened \n");
    return;
  }
  do {
    ch = fgetc(fp);
    cnt += 1;
  } while (ch != EOF);
  fclose(fp);
  printf("number of chars: %d\n", cnt);
}

int main()
{
  char c;
  int i;
  char *argv[3];
  char *cmd;
  pid_t pid, pgid, cpid;
  cmd = "wc";
  argv[2] = NULL;
  argv[0] = "wc";
  printf("current pid: %d\n", getpid());
  setpgid(0, 0);
  pgid = getpgid(0);
  printf("current gpid: %d\n", getpgid(0));
  do {
    c = getchar();
    pid = fork();
    if (pid == 0) {
      setpgid(0, pgid);
      printf("pid: %d, gpid: %d\n", getpid(), getpgid(0));
      if (c == '0') {
        printf("option 0\n");
        argv[1] = "../files/small.txt";
        execvp(cmd, argv);
      } else if (c == '1') {
        printf("option 1\n");
        argv[1] = "../files/1M.txt";
        execvp(cmd, argv);
      } else if (c == '2') {
        argv[1] = "../files/10M.txt";
        execvp(cmd, argv);
      } else if (c == '3') {  
        argv[1] = "../files/100M.txt";
        execvp(cmd, argv);
      } else if (c == '5') {
        printf("option 5: read 100 x 1M files\n");
        cmd = "zsh";
        argv[0] = "zsh";
        argv[1] = "./wc.sh";
        execvp(cmd, argv);
      } else {
        exit(0);
      }
    } else {
      waitpid(pid, 0, 0);
    }
  } while (c != 'q' && c != EOF);
  printf("QUIT\n");
  return 0;
}
