#include <ztimer.h>
#include <stdio.h>

void periodic_callback(void *arg) {
    puts("Periodic event triggered!");
}

int main(void) {
    printf("Starting periodic timer example on ESP32\n");
    
    ztimer_t periodic_timer;
    ztimer_set_wakeup(&periodic_timer, ztimer_msec_from_ms(0));
    
    while (1) {
        ztimer_periodic_wakeup(ZTIMER_MSEC, &periodic_timer, 1000);
        periodic_callback(NULL);
    }

    return 0;
}