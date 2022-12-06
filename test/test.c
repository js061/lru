#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/time.h>
#include <stdint.h>

int main()
{
  char c;
  int i;
  char *argv[3];
  char *cmd;
  pid_t pid, pgid, cpid;
  struct timespec start, end;
  cmd = "wc";
  argv[2] = NULL;
  argv[0] = "wc";
  printf("current pid: %d\n", getpid());
  setpgid(0, 0);
  pgid = getpgid(0);
  printf("current gpid: %d\n", getpgid(0));
  do {
    c = getchar();
    printf("START\n");
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    pid = fork();
    if (pid == 0) {
      setpgid(0, pgid);
      printf("pid: %d, gpid: %d\n", getpid(), getpgid(0));
      printf("read 40M file 4 times\n");
      cmd = "zsh";
      argv[0] = "zsh";
      argv[1] = "./wc_2.sh";
      execvp(cmd, argv);
    } else {
      waitpid(pid, 0, 0);
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    uint64_t delta_1 = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    pid = fork();
    if (pid == 0) {
      setpgid(0, pgid);
      printf("pid: %d, gpid: %d\n", getpid(), getpgid(0));
      printf("read 100 x 1M files\n");
      cmd = "zsh";
      argv[0] = "zsh";
      argv[1] = "./wc.sh";
      execvp(cmd, argv);
    } else {
      waitpid(pid, 0, 0);
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    uint64_t delta_2 = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;

    //clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    //pid = fork();
    //if (pid == 0) {
    //  setpgid(0, pgid);
    //  printf("pid: %d, gpid: %d\n", getpid(), getpgid(0));
    //  printf("read 40M file 4 times\n");
    //  cmd = "zsh";
    //  argv[0] = "zsh";
    //  argv[1] = "./wc_2.sh";
    //  execvp(cmd, argv);
    //} else {
    //  waitpid(pid, 0, 0);
    //}

    // pid = fork();
    // if (pid == 0) {
    //   setpgid(0, pgid);
    //   printf("pid: %d, gpid: %d\n", getpid(), getpgid(0));
    //   printf("read 100 x 2M files\n");
    //   cmd = "zsh";
    //   argv[0] = "zsh";
    //   argv[1] = "./wc_1.sh";
    //   execvp(cmd, argv);
    // } else {
    //   waitpid(pid, 0, 0);
    // }
    printf("END\n");
    //clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    uint64_t delta_3 = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
    printf("time 2: %lu\n", delta_2);
    printf("time 1: %lu, time 3: %lu, time 1 + 3: %lu\n", delta_1, delta_3, delta_1 + delta_3);
  } while (c != 'q' && c != EOF);
  printf("QUIT\n");
  return 0;
}
