#include <xtimer.h>
#include <thread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define PERIODIC_INTERVAL        (1000000U)  // 1 second in microseconds


char periodic_thread_stack[THREAD_STACKSIZE_MAIN];

void* event_periodic(void* arg){
    (void)arg;
    while(1) {
        puts("Periodic event triggered!");
        xtimer_usleep(PERIODIC_INTERVAL);
    }
    return NULL;
}

int main(void) {
    puts("Starting RIOT application with periodic event...");

    thread_create(periodic_thread_stack, sizeof(periodic_thread_stack),
                  THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  event_periodic, NULL, "event_periodic");

    return 0;
}
