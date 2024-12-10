
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

#define INTERVAL_SEC 1

void periodic_callback(void) {
    printf("Periodic callback triggered!\n");
}

int main(void) {
    while (true) {
        periodic_callback();
        sleep(INTERVAL_SEC);
    }
    return 0;
}
