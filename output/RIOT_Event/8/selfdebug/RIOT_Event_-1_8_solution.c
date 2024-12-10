#include <ztimer.h>
#include <stdio.h>
#include <inttypes.h>

static void callback(void *arg) {
    printf("Callback triggered with arg: %p\n", arg);
}

int main(void) {
    static ztimer_t timer;
    ztimer_set_msg(ZTIMER_SEC, &timer, 1U, callback, NULL);

    while (1) {
        // Put the main thread to sleep to wait for timer events
        ztimer_sleep(ZTIMER_MSEC, 1000);  // sleep for 1 second
    }

    return 0;
}