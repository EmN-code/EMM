#include <ztimer.h>
#include <netinet/in.h>
#include <event/thread.h>
#include <xtimer.h>
#include <msg.h>
#include <net/sock/udp.h>
#include <dht.h>
#include <mutex.h>

#define DHT_PIN  GPIO_PIN(0, 1)  // Example GPIO pin for DHT11
#define LED_PIN  GPIO_PIN(0, 2)  // Example GPIO pin for LED
#define TEMP_THRESHOLD 30        // Temperature threshold in degrees Celsius

static dht_t dev;
static int16_t temp, hum;

int main(void) {
    // Initialize the DHT sensor
    if (dht_init(&dev, &dht_params[0]) != 0) {
        printf("DHT initialization failed\n");
        return 1;
    }

    // Configure LED GPIO pin
    gpio_init(LED_PIN, GPIO_OUT);

    while (1) {
        if (dht_read(&dev, &temp, &hum) != 0) {
            printf("Error reading DHT sensor\n");
        }
        else {
            printf("Temperature: %d, Humidity: %d\n", temp, hum);
            if (temp >= TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            }
            else {
                gpio_clear(LED_PIN);
            }
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return 0;
}