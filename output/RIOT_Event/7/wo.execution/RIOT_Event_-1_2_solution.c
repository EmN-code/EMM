#include <ztimer/periph.h>

void callback(void *arg) {
    // Your callback logic here
    printf("Callback executed\n");
}

static ztimer_t timer;

int main(void) {
    ztimer_periodic_init(ZTIMER_MSEC, &timer, callback, NULL, 1000);
    while (1) {
        ztimer_periodic_wakeup(ZTIMER_MSEC, 1000);
        // Other tasks in the main loop
    }
    return 0;
}