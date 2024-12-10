#include "xtimer.h"
#include "periph/gpio.h"
#include "dht.h"
#include "msg.h"

#define DHT_PIN GPIO_PIN(PORT_A, 0)  // Use actual GPIO pin
#define LED_PIN GPIO_PIN(PORT_B, 1)  // Use actual GPIO pin
#define TEMP_THRESHOLD 30
#define SLEEP_TIME 5

int main(void)
{
    kernel_pid_t pid = thread_getpid();
    msg_t msg;
    int16_t temperature, humidity;
    dht_t dev;

    // Initialize GPIO for LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize DHT sensor
    if (dht_init(&dev, DHT_DHT11, DHT_PIN) != 0) {
        printf("DHT sensor initialization failed\n");
        return 1;
    }

    while (1) {
        // Read values from the DHT11 sensor
        if (dht_read(&dev, &temperature, &humidity) != DHT_OK) {
            printf("Failed to read from DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);
            
            // Check if temperature exceeds the threshold
            if (temperature / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }

        // Sleep for 5 seconds before reading again
        xtimer_sleep(SLEEP_TIME);
    }

    return 0;
}