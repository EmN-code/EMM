#include "paho_mqtt.h"
#include "periph/i2c.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "dht_params.h"
#include "dht.h"
#include "event/thread.h"
#include "thread.h"

#define DHT_PIN GPIO_PIN(PORT_A, 0)
#define LED_PIN GPIO_PIN(PORT_A, 1)
#define TEMP_THRESHOLD 30 // Temperature threshold in Celsius

void read_sensor_task(void) {
    int16_t temp, hum;
    dht_t dev;
    
    // Initialize the DHT sensor
    dev.pin = DHT_PIN;
    
    while (1) {
        // Read temperature and humidity
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d.%d C, Humidity: %d.%d %%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            
            // Check if temperature exceeds the threshold
            if (temp / 10 >= TEMP_THRESHOLD) {
                gpio_set(LED_PIN); // Turn on the LED
            } else {
                gpio_clear(LED_PIN); // Turn off the LED
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        
        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
}

int main(void) {
    // Initialize GPIO for LED
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Create a thread for reading the sensor
    thread_create(read_sensor_task, 1024, THREAD_PRIORITY_MAIN - 1, 0, NULL, "read_sensor");
    
    return 0;
}