#include <ztimer.h>
#include <stdio.h>

void periodic_callback(void *arg) {
    printf("Periodic timer callback executed.\n");
}

int main(void) {
    ztimer_periodic_callback_t callback;
    callback.callback = periodic_callback;
    callback.arg = NULL;

    ztimer_periodic_start(&ztimer_sec, &callback, 1 * ZTIMER_SEC_PER_SEC);

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return 0;
}