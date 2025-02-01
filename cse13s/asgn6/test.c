#include <stdio.h>
static int x = 5;

int main(void) {
    x -= 1;
    x = 3;
    printf("%d", x);
    return 0;
}
