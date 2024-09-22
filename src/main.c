#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define bb "\x1b[1;32m\x1b[0m"
#define bg "\x1b[34m\x1b[0m"
#define re "\x1b[0m"

typedef struct {
  char *buf;
  size_t len_buf;
  ssize_t len_src;
} Buffer;

Buffer *new_buf() {
  Buffer *b = (Buffer *)malloc(sizeof(Buffer));
  b->buf = NULL;
  b->len_buf = 0;
  b->len_src = 0;
  return b;
}

Buffer *new_buf_from_src(char *src) {
  Buffer *b = (Buffer *)malloc(sizeof(Buffer));
  b->len_src = strlen(src);
  b->len_buf = b->len_src + 1;
  b->buf = (char *)malloc(b->len_buf);
  strncpy(b->buf, src, b->len_src);
  b->buf[b->len_src] = '\0';
  return b;
}

void prompt(char *msg) {
  if (strlen(msg) > 0) {
    printf("\x1b[1;32m[\x1b[34mep\x1b[1;32mdb %s] \x1b[34m:\x1b[1;32m> \x1b[0m",
           msg);
    return;
  }
  printf("\x1b[1;32m[\x1b[34mep\x1b[1;32mdb] \x1b[34m:\x1b[1;32m> \x1b[0m");
}

void help(char *cmd) {
  if (strcmp(cmd, ".exit") == 0) {
    printf("Exit the program.\n");
    return;
  }
  printf("HELP\n\n\tCOMMANDS\n\n\t.exit\n\t\tExit the program.\n");
}

void read_in(Buffer *b) {
  ssize_t b_read = getline(&(b->buf), &(b->len_buf), stdin);
  if (b_read <= 0) {
    printf("Error reading input\n");
    exit(EXIT_FAILURE);
  }
  b->len_src = b_read - 1;
  b->buf[b_read - 1] = 0;
}

void close_buf(Buffer *b) {
  free(b->buf);
  free(b);
}

void exit_ok(Buffer *b) {
  close_buf(b);
  exit(EXIT_SUCCESS);
}

typedef enum Cmd {
  CMD_EXIT = 0,
  CMD_HELP = 1,
} Cmd;

int main(int argc, const char *arg[]) {
  Buffer *b = new_buf();
  do {
    prompt("");
    read_in(b);
    if (strcmp(b->buf, ".exit") == 0)
      exit_ok(b);
    else if (strcmp(b->buf, ".help") == 0)
      help("");
    else
      printf("Unrecognized command '%s'.\n", b->buf);
  } while (1);
}
