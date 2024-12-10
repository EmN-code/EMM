#include "ztimer.h"
#include "stdio.h"
#include "periph/gpio.h"
#include "dht.h"
#include "msg.h"
#include "timex.h"
#include "thread.h"

#define DHT_PIN GPIO_PIN(PORT_A, 0) // Assume PORT_A, pin 0 is connected to the DHT
#define LED_PIN GPIO_PIN(PORT_A, 1) // Assume PORT_A, pin 1 is connected to the LED

#define TEMP_THRESHOLD 30.0 // Temperature threshold in Celsius
#define SLEEP_INTERVAL (5U) // Sleep interval in seconds

int main(void) {
    dht_t dev;
    int16_t temp, hum;
    msg_t msg_queue[8];
    
    msg_init_queue(msg_queue, 8);

    // Init DHT sensor
    if (dht_init(&dev, DHT_PIN) != 0) {
        puts("Failed to initialize DHT sensor");
        return 1;
    }

    // Init LED pin
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN); // Ensure LED is off initially

    while (1) {
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %.1f°C, Humidity: %.1f%%\n", temp / 10.0, hum / 10.0);
            if ((temp / 10.0) > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            puts("Failed to read from DHT sensor");
        }
        ztimer_sleep(ZTIMER_MSEC, SLEEP_INTERVAL * 1000U);
    }
    return 0;
}