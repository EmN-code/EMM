#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht.h"

#define DHT_PIN GPIO_PIN(PORT_A, 2) // Use a valid PORT and PIN for DHT11
#define LED_PIN GPIO_PIN(PORT_B, 5) // Use a valid PORT and PIN for the LED
#define TEMP_THRESHOLD 30 // Temperature threshold in Celsius
#define DHT_TYPE DHT11
#define READ_INTERVAL (5U * US_PER_SEC) // Read every 5 seconds

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_clear(LED_PIN);

    dht_t dev;
    if (dht_init(&dev, &DHT_TYPE, DHT_PIN) != DHT_OK) {
        printf("Failed to initialize DHT11\n");
        return 1;
    }
    
    while (1) {
        int16_t temperature, humidity;
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temperature/10, temperature%10, humidity/10, humidity%10);
            if (temperature/10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read sensor\n");
        }
        xtimer_usleep(READ_INTERVAL);
    }
    return 0;
}