#include <xtimer.h>
#include <timex.h>
#include <stdio.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(PORT_A, 0)

static void timer_callback(void *arg)
{
    (void)arg;
    gpio_toggle(LED_PIN);
    printf("LED toggled\n");
}

int main(void)
{
    gpio_init(LED_PIN, GPIO_OUT);
    
    xtimer_ticks32_t interval = xtimer_usec_from_sec(1);
    
    static xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;
    
    xtimer_set(&timer, interval);
    
    while (1) {
        xtimer_sleep(1);
    }
    
    return 0;
}