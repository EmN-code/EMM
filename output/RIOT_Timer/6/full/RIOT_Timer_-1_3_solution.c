#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <time_units.h>

#define TIMER_INTERVAL (1U * US_PER_SEC) // 1 second interval
#define LED_PIN GPIO_PIN(PORT_A, 0) // Adjust this to your specific LED configuration

static void setup_timer(void) {
    static ztimer_t timer;
    ztimer_set(&timer, TIMER_INTERVAL);
}

int main(void) {
    // Setup the ztimer
    setup_timer();

    // Initiate mailbox for message passing
    msg_init_queue();

    while (1) {
        // Wait for timer to elapse
        ztimer_sleep(ZTIMER_USEC, TIMER_INTERVAL);

        // Simulate toggling LED
        printf("Toggling LED...\n");
    }

    return 0;
}