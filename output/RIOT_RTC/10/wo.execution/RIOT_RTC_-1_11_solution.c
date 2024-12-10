#include <ztimer.h>
#include <thread.h>
#include <stdio.h>

void *alarm_thread(void *arg) {
    ztimer_sleep(ZTIMER_MSEC, 5000);  // Sleep for 5000 milliseconds (5 seconds)
    printf("Alarm triggered!\n");
    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0, alarm_thread, NULL, "alarm_thread");
    
    while (1) {
        ztimer_sleep(ZTIMER_SEC, 1);  // Main thread sleeps to avoid exiting
    }
    return 0;
}