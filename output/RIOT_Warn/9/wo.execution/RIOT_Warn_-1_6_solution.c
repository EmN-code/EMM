#include "paho_mqtt.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "dht.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "thread.h"

#define DHT_PIN 4
#define LED_PIN 2
#define TEMP_THRESHOLD 30

void init_sensors(void) {
    // Initialize DHT sensor and other required peripherals
}

void read_temperature(void) {
    int16_t temperature = 0;
    int16_t humidity = 0;
    dht_t dev;
    
    // Assume dev is initialized correctly
    if (dht_read(&dev, &temperature, &humidity) == 0) {
        printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
        
        if (temperature > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);
        }
        else {
            gpio_clear(LED_PIN);
        }
    }
    else {
        printf("Error reading DHT sensor\n");
    }
}

int main(void) {
    // Initialize hardware components
    gpio_init(LED_PIN, GPIO_OUT);
    init_sensors();

    while (1) {
        read_temperature();
        xtimer_sleep(5);
    }

    return 0;
}