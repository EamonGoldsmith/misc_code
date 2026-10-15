#include <stdio.h>

#define lambda(return_type, body)                                              \
  ({                                                                           \
    return_type __fn__ body;                                                   \
    __fn__;                                                                    \
  })

typedef int (*func)(int, int);

int add(int a, int b) { return a + b; }

void *make_adder(int n) {
  return lambda(int, (int a) { return add(a, n); });
}

int main() {
  void *constructed = make_adder(3);

  typedef int (*con_func)(int);
  int res = ((con_func)constructed)(4);

  printf("result: %i\n", res);
  return 0;
}
