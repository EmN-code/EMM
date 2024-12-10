#include <thread.h>
#include <stdio.h>
#include <xtimer.h>

#define INTERVAL (1U * US_PER_SEC)  // 1 second

static void *_event_periodic(void *arg) {
    (void)arg;  // suppress unused variable warning
    while (1) {
        printf("Callback event triggered\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    thread_create(event_thread_stack, sizeof(event_thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, _event_periodic, NULL, "event_periodic");
    return 0;
}