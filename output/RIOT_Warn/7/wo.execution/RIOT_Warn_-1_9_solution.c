#include <ztimer.h>  // Necessary for timer functions
#include <thread.h>  // Required for threading functions
#include <msg.h>  // Needed for message handling
#include <dht.h>  // For interacting with DHT sensors
#include <esp_system.h>  // Essential for ESP32 system functions

#define DHT_PIN GPIO_NUM_4  // GPIO pin connected to the DHT11
#define LED_PIN GPIO_NUM_2  // GPIO pin connected to the LED
#define TEMP_THRESHOLD 30  // Temperature threshold for LED warning

// Function to read temperature and control LED according to threshold
dht_t dev;
void read_sensor(void) {
    int16_t temp, hum;
    if (dht_read(&dev, &temp, &hum) == 0) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        if (temp / 10 > TEMP_THRESHOLD) {
            gpio_set(LED_PIN);  // Turn LED on
        } else {
            gpio_clear(LED_PIN);  // Turn LED off
        }
    } else {
        puts("Failed to read from DHT sensor!");
    }
}

// Thread for continuously checking the sensor
dht_t dev = { .pin = DHT_PIN };
char stack[THREAD_STACKSIZE_MAIN];
void *sensor_thread(void *arg) {
    (void)arg;
    while (1) {
        read_sensor();
        ztimer_sleep(ZTIMER_USEC, 5000000);  // Sleep for 5 seconds
    }
    return NULL;
}

// Main function - entry point of the application
int main(void) {
    dht_init(&dev);
    gpio_init(LED_PIN, GPIO_OUT);
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, sensor_thread, NULL, "sensor_thread");
    return 0;
}