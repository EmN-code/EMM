#include "string.h"
#include "mma8x5x_params.h"
#include "dht_params.h"
#include "thread.h"
#include "dht.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "mma8x5x_regs.h"
#include "event/thread.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "periph/i2c.h"

#define DHT_PIN 4
#define TEMP_THRESHOLD 30
#define LED_PIN 2

void *read_dht_and_control_led(void *arg) {
    dht_t dev;

    // Initialize DHT
    while (dht_init(&dev, &dht_params[0]) != 0) {
        printf("DHT init failed\n");
        xtimer_sleep(1);
    }

    while (1) {
        int16_t temp, hum;
        // Read DHT
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d, Humidity: %d\n", temp / 10, hum / 10);
            
            if (temp / 10 > TEMP_THRESHOLD) {
                // Temperature exceeds threshold, turn on LED
                gpio_set(GPIO_PIN(0, LED_PIN));
            } else {
                // Temperature is below threshold, turn off LED
                gpio_clear(GPIO_PIN(0, LED_PIN));
            }
        }
        else {
            printf("Failed to read DHT\n");
        }
        
        // Delay
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    printf("DHT11 and LED control example\n");

    // Initialize GPIO for LED
    gpio_init(GPIO_PIN(0, LED_PIN), GPIO_OUT);

    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  read_dht_and_control_led, NULL, "dht_led_thread");

    return 0;
}