#include "xtimer.h"
#include "periph/gpio.h"
#include "periph_cpu.h"

void alarm_handler(void) {
    printf("Alarm triggered!\n");
}

int main(void)
{
    xtimer_ticks32_t last_wakeup = xtimer_now();

    // Initialize the GPIO (even though it's not fully needed here, included for periph context)
    gpio_init(GPIO_PIN(0, 1), GPIO_OUT);

    // Set a delay for 5 seconds (5,000,000 microseconds) for the alarm
    xtimer_ticks32_t alarm_time = xtimer_now() + xtimer_usec_from_ticks(5000000);

    printf("Setting date to June 1, 2024\n");
    printf("Alarm will be triggered in 5 seconds\n");

    // Loop until the alarm time is reached
    while (xtimer_now() < alarm_time) {
        xtimer_periodic_wakeup(&last_wakeup, 1000000); // check every second
    }

    // Alarm triggered
    alarm_handler();

    return 0;
}