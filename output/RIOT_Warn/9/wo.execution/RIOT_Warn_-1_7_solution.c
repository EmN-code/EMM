#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mqttclient.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "paho_mqtt.h"
#include "event/thread.h"
#include "thread.h"
#include "dht_params.h"
#include "dht.h"

#define TEMP_THRESHOLD 25
#define DHT_PIN GPIO_NUM_4
#define LED_PIN GPIO_NUM_2

void read_temp_and_control_led(void) {
    dht_t dev;
    int16_t temperature = 0, humidity = 0;
    
    // Initialize the DHT sensor
    if (dht_init(&dev, DHT_PIN, false) != 0) {
        printf("DHT initialization failed\n");
        return;
    }

    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temperature, &humidity) != 0) {
            printf("Failed to read from DHT sensor\n");
        } else {
            printf("Temperature: %d.%d C, Humidity: %d.%d\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);
            if (temperature > TEMP_THRESHOLD * 10) {
                gpio_set(LED_PIN);
                printf("Temperature threshold exceeded! LED turned on.\n");
            } else {
                gpio_clear(LED_PIN);
                printf("Temperature normal. LED turned off.\n");
            }
        }
        rtc_sleep(5);
    }
}

int main(void) {
    // Initialize LED pin as output
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Failed to initialize GPIO for LED\n");
        return 1;
    }
    
    // Call the temperature reading and LED control function
    read_temp_and_control_led();

    return 0;
}