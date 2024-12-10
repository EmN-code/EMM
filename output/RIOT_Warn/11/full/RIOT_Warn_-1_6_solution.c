#include "paho_mqtt.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "dht.h"
#include "event/thread.h"
#include "thread.h"
#include "MQTTClient.h"

#define TEMP_THRESHOLD 30  // Temperature threshold for the LED warning
#define LED_PIN 2          // GPIO pin for the LED
#define DHT_PIN 4          // GPIO pin for the DHT11 sensor

void read_sensor_and_control_led(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    dht_t dev;
    dht_params_t params;
    int16_t temperature, humidity;
    
    params.pin = DHT_PIN;
    params.type = DHT11;

    if (dht_init(&dev, &params) != 0) {
        printf("Sensor initialization failed\n");
        return;
    }

    while (1) {
        int result = dht_read(&dev, &temperature, &humidity);
        if (result != 0) {
            printf("Failed to read from sensor\n");
        } else {
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
            if (temperature > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        ztimer_sleep(ZTIMER_SEC, 5);
    }
}

int main(void) {
    printf("Starting DHT11 sensor reading and LED control task\n");
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  read_sensor_and_control_led, "sensor_task");
    return 0;
}