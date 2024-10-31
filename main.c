#include <stdio.h>
#include <unistd.h>

int main(char *argv[])
{
    const int rc = access(argv[1], 0);
    printf("rc: %d\n", rc);
    return 0;
}