#include <xtimer.h>
#include <timex.h>
#include <stdio.h>

#define INTERVAL (1U * US_PER_SEC)

static void callback(void *arg) {
    puts("Callback trigger every second");
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
        callback(NULL);
    }
    return 0;
}