#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "paho_mqtt.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "dht.h"
#include "event/thread.h"
#include "MQTTClient.h"
#include "thread.h"

#define DHT_PIN GPIO_PIN(0, 4)  // Example pin for DHT11
#define LED_PIN GPIO_PIN(0, 5)  // Example pin for LED
#define TEMP_THRESHOLD 30

void read_sensor_and_control_led(void) {
    dht_t dht_dev;
    int16_t temperature = 0, humidity = 0;
    
    dht_init(&dht_dev, &dht_params[DHT_TYPE_DHT11], DHT_PIN);

    while (1) {
        if (dht_read(&dht_dev, &temperature, &humidity) != 0) {
            // Error reading DHT sensor
            puts("DHT read failed");
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
    printf("Starting DHT11 temperature monitoring...\n");

    // Initialize LED pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Run the sensor reading and control loop
    read_sensor_and_control_led();

    return 0;
}