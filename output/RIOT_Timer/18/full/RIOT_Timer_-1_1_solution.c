#include "timex.h"
#include "thread.h"
#include "timers.h"
#include "periph/gpio.h"

#define INTERVAL (1U * US_PER_SEC)
#define LED_PIN GPIO_PIN(PORT_A, 0)

static void timer_callback(void *arg)
{
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void)
{
    gpio_init(LED_PIN, GPIO_OUT);
    
    timer_t timer;
    int result = timer_init(&timer, 1, timer_callback, NULL);
    if (result < 0) {
        printf("Error initializing timer\n");
        return 1;
    }

    timer_set(&timer, INTERVAL, INTERVAL);

    while (1) {
        thread_sleep();
    }

    return 0;
}