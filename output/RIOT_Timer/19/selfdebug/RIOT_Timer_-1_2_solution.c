#include <xtimer.h>
#include <tim.h>
#include <msg.h>

#define LED_PIN GPIO_PIN(PORT_A, 5)

static void timer_callback(void *arg) {
    (void)arg; // Cast arg to void to avoid unused parameter warning
    LED_TOGGLE(LED_PIN);
}

int main(void) {
    printf("Timer example with LED toggle
");

    // Initialize GPIO
    gpio_init(LED_PIN, GPIO_OUT);

    // Setup xtimer
    xtimer_init();
    static xtimer_ticks32_t xt;
    xt = xtimer_now();

    // Initialize a periodic timer
    timer_t my_timer;
    my_timer.dev = TIMER_DEV(0);
    my_timer.freq = 1; // Frequency of 1 Hz
    my_timer.cb = timer_callback; // assign correct callback
    my_timer.arg = NULL;

    // Initialize the timer
    int res = timer_init(my_timer.dev, my_timer.freq, my_timer.cb, my_timer.arg);
    if (res != XTIMER_OK) {
        printf("Error initializing timer\n");
        return res;
    }

    while (1) {
        // Sleep to avoid busy-waiting
        xtimer_periodic_wakeup(&xt, 1 * US_PER_SEC);
    }
    return 0;
}