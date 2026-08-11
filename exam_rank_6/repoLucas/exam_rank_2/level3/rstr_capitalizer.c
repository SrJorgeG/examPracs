#include <unistd.h>

int is_space(char c) {
  if (c == ' ' || (c >= '\t' && c <= '\r'))
    return 1;
  return 0;
}

int is_alpha(char c) {
  if (c >= 'a' && c <= 'z')
    return 1;
  return 0;
}

int is_upper(char c) {
  if (c >= 'A' && c <= 'Z')
    return 1;
  return 0;
}

char *to_lower(char *s) {
  int i = -1;
  while (s[++i]) {
    if (is_upper(s[i]))
      s[i] += 32;
  }
  return s;
}

int main(int ac, char **av) {
  if (ac > 1) {
    int i = 0;
    while (av[++i]) {
      av[i] = to_lower(av[i]);
      int j = -1;
      while(av[i][++j]) {
        if ((is_space(av[i][j + 1]) && is_alpha(av[i][j])) || (!av[i][j + 1] && is_alpha(av[i][j])))
          av[i][j] -= 32;
        write(1, &av[i][j], 1);
      }
      write(1, "\n", 1);
    }
  }
  else
    write(1, "\n", 1);
  return 0;
}