#include <ztimer.h>
#include <stdio.h>

static void callback(void *arg) {
    (void)arg;  // To avoid unused parameter warning
    puts("Callback function triggered!");
}

int main(void) {
    ztimer_t timer;

    ztimer_set_msg(ZTIMER_MSEC, &timer, 1000, NULL, 0);  // 1000 ms = 1 second

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);  // Sleep for 1 second
        callback(NULL);  // Trigger the callback
    }

    return 0;
}