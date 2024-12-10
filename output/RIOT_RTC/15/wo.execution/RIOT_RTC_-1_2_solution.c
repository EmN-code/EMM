#include <ztimer.h>
#include <event/periodic.h>
#include <event/callback.h>
#include <periph_cpu.h>

void callback(void *arg) {
    (void)arg;
    puts("Alarm triggered!");
}

int main(void) {
    ztimer_sleep(ZTIMER_MSEC, 5000);
    
    printf("Setting RTC to June 1, 2024\n");

    /* Set a timer event */
    ztimer_t timer;
    timer.callback = callback;
    ztimer_set_cb(&timer, 5000);

    /* Wait indefinitely, relying on the callback to act when the timer expires */
    while(1) {
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    return 0;
}