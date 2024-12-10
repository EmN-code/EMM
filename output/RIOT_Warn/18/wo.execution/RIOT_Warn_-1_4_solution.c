#include "thread.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mqttclient.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "dht.h"

#define GPIO_LED 2  // GPIO pin for the LED
#define DHT_PIN 4   // GPIO pin connected to the DHT11 sensor data pin
#define TEMP_THRESHOLD 30  // Temperature threshold in Celsius

void *read_dht(void *arg) {
    (void)arg;
    dht_t dev;
    int16_t temp, hum;
    while (1) {
        if (dht_read(&dev, &temp, &hum) != ESP_OK) {
            printf("Could not read data from DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C
Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(GPIO_LED);
            } else {
                gpio_clear(GPIO_LED);
            }
        }
        ztimer_sleep(ztimer_now() + 5000);
    }

    return NULL;
}

int main(void) {
    // Initialize GPIO for LED
    gpio_init(GPIO_LED, GPIO_OUT);

    // Start the DHT11 reading thread
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  0, read_dht, NULL, "dht_reader");

    return 0;
}