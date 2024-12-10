#include "xtimer.h"
#include "ztimer.h"
#include "ztimer/periodic.h"
#include "stdio.h"

#define INTERVAL (1U * US_PER_SEC) // 1 second interval

void timer_callback(void *arg)
{
    (void)arg;
    printf("Timer triggered!\n");
}

int main(void)
{
    ztimer_t timer;
    ztimer_set_periodic(ZTIMER_SEC, &timer, INTERVAL, timer_callback, NULL);

    while (1) {
        ztimer_sleep(ZTIMER_SEC, INTERVAL);
    }

    return 0;
}