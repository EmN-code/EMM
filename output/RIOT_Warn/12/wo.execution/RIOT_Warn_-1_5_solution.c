#include "paho_mqtt.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x_regs.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "MQTTClient.h"
#include "thread.h"

#define LED_PIN GPIO_PIN(0, 2)  // Select the appropriate pin here
#define TEMP_THRESHOLD 30  // Temperature threshold for LED warning

void *sensor_thread(void *arg) {
    dht_t dev;
    int16_t temp, hum;
    
    // Initialize DHT device
    if (dht_init(&dev, &dht_params[0]) != DHT_OK) {
        return NULL;  // Initialization failed
    }
    
    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            // Convert to Celsius
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        // Sleep for 5 seconds
        xtimer_usleep(5000000);
    }
    return NULL;
}

int main(void) {
    // Initialize GPIO
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        return 1;
    }
    
    char stack[THREAD_STACKSIZE_MAIN];
    
    // Create sensor reading thread
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                  sensor_thread, NULL, "sensor_thread");
                  
    return 0;
}