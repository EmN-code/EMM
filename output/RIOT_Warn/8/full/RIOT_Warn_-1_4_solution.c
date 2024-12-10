#include "thread.h"
#include "string.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "event/thread.h"
#include "dht.h"
#include "dht_params.h"

#define DHT_PIN GPIO_PIN(0, 4) // Assumed pin, adjust based on your setup
#define TEMP_THRESHOLD 25 // Temperature threshold for LED warning
#define SLEEP_INTERVAL 5 // Interval to read sensor data (in seconds)

void read_dht_sensor(void *arg) {
    dht_t dev;
    int16_t temperature, humidity;
    gpio_t led_pin = GPIO_PIN(0, 2); // Assuming pin for LED
    
    // Initialize GPIO for LED
    gpio_init(led_pin, GPIO_OUT);
    
    // Initialize DHT sensor on DHT_PIN
    if (dht_init(&dev, &dht_params[0]) != 0) {
        printf("Error initializing DHT sensor\n");
        return;
    }
    
    while (1) {
        if (dht_read(&dev, &temperature, &humidity) != 0) {
            printf("Error reading from DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C - Humidity: %d.%d%%\n", temperature / 10, temperature % 10, humidity / 10, humidity % 10);
            
            // Check if temperature exceeds the threshold
            if (temperature / 10 > TEMP_THRESHOLD) {
                gpio_set(led_pin); // Turn on LED
            } else {
                gpio_clear(led_pin); // Turn off LED
            }
        }
        
        // Delay before next reading
        vTaskDelay(pdMS_TO_TICKS(SLEEP_INTERVAL * 1000));
    }
}

int main(void) {
    char stack[THREAD_STACKSIZE_MAIN];
    
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,"dht_sensor", read_dht_sensor, NULL, "dht");
    
    while (1) {
        // Main thread can perform other tasks
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    return 0;
}