#include <ztimer.h>
#include <timex.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <periph/uart.h>
#include <dht.h>

#define DHT_PIN     GPIO_PIN(PORT_A, 7) // GPIO pin to which the DHT11 sensor is connected
#define LED_PIN     GPIO_PIN(PORT_B, 5) // GPIO pin to which the LED is connected
#define TEMP_THRESHOLD  30              // Temperature threshold for the LED warning

static dht_t dev;

int main(void)
{
    ztimer_sleep(ZTIMER_MSEC, 1000); // Delay for sensors to stabilize
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_init(DHT_PIN, GPIO_IN);

    int16_t temp, hum;

    while (1) {
        int res = dht_read(&dev, &temp, &hum);
        if (res == DHT_OK) {
            printf("Temperature: %d *C, Humidity: %d %%\n", temp, hum);
            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            }
            else {
                gpio_clear(LED_PIN);
            }
        }
        else {
            printf("Error reading DHT sensor\n");
        }

        ztimer_sleep(ZTIMER_MSEC, 5000); // Sleep for 5 seconds
    }

    return 0;
}