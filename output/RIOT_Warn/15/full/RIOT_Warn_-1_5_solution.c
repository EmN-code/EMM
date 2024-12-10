#include "string.h"
#include "mma8x5x_params.h"
#include "dht_params.h"
#include "thread.h"
#include "dht.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "mma8x5x_regs.h"
#include "event/thread.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "periph/i2c.h"

#define GPIO_PIN_LED 2  // Assume GPIO 2 is connected to an LED
#define GPIO_PIN_DHT 4  // Assume GPIO 4 is connected to DHT11
#define TEMP_THRESHOLD 30  // Temperature threshold in Celsius

static dht_t dht_dev;

void check_temperature(void) {
    int16_t temp, hum;
    if (dht_read(&dht_dev, &temp, &hum) == DHT_OK) {
        printf("Temperature: %d.%d C, Humidity: %d.%d %%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        if (temp / 10 > TEMP_THRESHOLD) {
            gpio_set(GPIO_PIN_LED);
        } else {
            gpio_clear(GPIO_PIN_LED);
        }
    } else {
        printf("Failed to read from DHT sensor\n");
    }
}

void *temperature_thread(void *arg) {
    (void)arg;
    while (1) {
        check_temperature();
        ztimer_sleep(ztimer_now(STOPWATCH));  // Sleep for 5 seconds
    }
    return NULL;
}

int main(void) {
    printf("Starting DHT11 sensor app\n");
    if (dht_init(&dht_dev, &dht_params[0]) != 0) {
        printf("Failed to initialize DHT sensor\n");
        return 1;
    }
    if (gpio_init(GPIO_PIN_LED, GPIO_OUT) != 0) {
        printf("Failed to initialize LED GPIO\n");
        return 1;
    }
    char thread_stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN, 0, temperature_thread, NULL, "dht_reader");
    return 0;
}