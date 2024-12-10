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

#define DHT_PIN GPIO_PIN(0, 4)
#define LED_PIN GPIO_PIN(0, 5)
#define TEMP_THRESHOLD 30

void read_dht_and_control_led(void) {
    dht_t dev;
    int16_t temperature, humidity;
    gpio_init(LED_PIN, GPIO_OUT);
    
    while (1) {
        if (dht_read(&dev, &temperature, &humidity) == 0) {
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
            if (temperature > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("Warning: High temperature!\n");
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from sensor\n");
        }
        xtimer_sleep(5);
    }
}

int main(void) {
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, 0, read_dht_and_control_led, NULL, "dht_reader");
    return 0;
}