#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(PORT_B, 5)

void _blink(void)
{
    /* turn LED on */
    gpio_set(LED_PIN);
    
    /* wait a second */
    xtimer_sleep(1);

    /* turn LED off */
    gpio_clear(LED_PIN);
    
    /* wait a second */
    xtimer_sleep(1);
}

int main(void)
{
    printf("Starting LED blink example\n");
    
    /* initialize the LED pin */
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        _blink();
    }

    return 0;
}