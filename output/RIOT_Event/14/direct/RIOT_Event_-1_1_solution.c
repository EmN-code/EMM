#include <stdio.h>
#include "xtimer.h"
#include "thread.h"

#define PERIOD (1U * US_PER_SEC) // 1 second period

void event_periodic(void)
{
    printf("Periodic event trigger\n");
}

void *thread_handler(void *arg)
{
    (void)arg;
    while (1) {
        xtimer_sleep(1);
        event_periodic();
    }
    return NULL;
}

int main(void)
{
    printf("RIOT event_periodic example starting\n");
    
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, 0, thread_handler, NULL, "periodic_thread");
    
    return 0;
}