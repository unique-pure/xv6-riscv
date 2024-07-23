// Simple grep.  Only supports ^ . * $ operators.

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

char buf[1024];

// check if the text matches the given regular expression pattern.
int match(char *, char *);

// reads from the given file descriptor and prints lines matching the pattern.
void grep(char *pattern, int fd) {
  int n, m;       // n is bytes read, m is current buffer position.
  char *p, *q;    // Pointers used to iterate through buffer.

  m = 0;
  while ((n = read(fd, buf + m, sizeof(buf) - m - 1)) > 0) {
    m += n;             // update the buffer size
    buf[m] = '\0';      // Null terminate the buffer to make it a valid C string.
    p = buf;
    while ((q = strchr(p, '\n')) != 0) {
      *q = 0;
      if (match(pattern, p)) {
        *q = '\n';
        write(1, p, q + 1 - p); // Write matched line to standard output.
      }
      p = q + 1;  // Move to the next line.
    }
    if (m > 0) {
      m -= p - buf;         // Update the buffer size.
      memmove(buf, p, m);   // Move leftover data to start of buffer.
    }
  }
}

int main(int argc, char *argv[]) {
  int fd, i;
  char *pattern;

  if (argc <= 1) {
    fprintf(2, "usage: grep pattern [file ...]\n");
    exit(1);
  }
  pattern = argv[1];

  if (argc <= 2) {
    grep(pattern, 0);
    exit(0);
  }

  for (i = 2; i < argc; i++) {
    if ((fd = open(argv[i], 0)) < 0) {
      printf("grep: cannot open %s\n", argv[i]);
      exit(1);
    }
    grep(pattern, fd);
    close(fd);
  }
  exit(0);
}

// Regexp matcher from Kernighan & Pike,
// The Practice of Programming, Chapter 9, or
// https://www.cs.princeton.edu/courses/archive/spr09/cos333/beautiful.html

int matchhere(char *, char *);
int matchstar(int, char *, char *);

int match(char *re, char *text) {
  if (re[0] == '^') // must match at beginning
    return matchhere(re + 1, text);
  do { // must look at empty string
    if (matchhere(re, text))
      return 1;
  } while (*text++ != '\0');
  return 0;
}

// matchhere: search for re at beginning of text
int matchhere(char *re, char *text) {
  if (re[0] == '\0')
    return 1;
  if (re[1] == '*')
    return matchstar(re[0], re + 2, text);
  if (re[0] == '$' && re[1] == '\0')
    return *text == '\0';
  if (*text != '\0' && (re[0] == '.' || re[0] == *text))
    return matchhere(re + 1, text + 1);
  return 0;
}

// matchstar: search for c*re at beginning of text
int matchstar(int c, char *re, char *text) {
  do
  { // a * matches zero or more instances
    if (matchhere(re, text))
      return 1;
  } while (*text != '\0' && (*text++ == c || c == '.'));
  return 0;
}
