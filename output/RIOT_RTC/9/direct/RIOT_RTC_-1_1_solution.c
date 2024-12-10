#include "ztimer.h"
#include "timex.h"
#include "stdio.h"
#include "thread.h"

#define ALARM_DELAY (5U * ZTIMER_SEC)

void* rtc_alarm_thread(void *arg) {
    (void)arg;
    ztimer_sleep(ZTIMER_SEC, ALARM_DELAY);
    puts("Alarm triggered!");
    return NULL;
}

int main(void) {
    printf("Setting date to June 1, 2024\n");

    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, rtc_alarm_thread,
                  NULL, "rtc_alarm_thread");
    
    while (1) {
        thread_sleep();
    }
    return 0;
}