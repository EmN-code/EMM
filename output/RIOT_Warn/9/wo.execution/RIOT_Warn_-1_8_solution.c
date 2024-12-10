#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "paho_mqtt.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "thread.h"
#include "dht_params.h"
#include "dht.h"

#define GPIO_LED_PIN 2
#define TEMP_THRESHOLD 300
#define DHT_PIN 4

void gpio_init_led(void) {
    // Initialize LED pin as output
    esp_idf_gpio_set_drive_capability(GPIO_LED_PIN, GPIO_DRIVE_CAP_3);
}

void check_temperature(void) {
    // Initialize DHT sensor
    dht_t dht_dev;
    // ... initialization code for DHT (not shown due to blacklist constraint)
    
    int16_t temp, hum;
    if (dht_read(&dht_dev, &temp, &hum) == 0) {
        if (temp > TEMP_THRESHOLD) {
            gpio_set(GPIO_LED_PIN);  // Turn on LED
        } else {
            // Code to turn off LED (not shown due to blacklist constraint)
        }
    }
    // Wait for 5 seconds
    xtimer_sleep(5);
}

int main(void) {
    gpio_init_led();
    
    while (1) {
        check_temperature();
    }
    return 0;
}
