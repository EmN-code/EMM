#include "timex.h"
#include "thread.h"
#include "periph/gpio.h"
#include "dht.h"

#define DHT_PIN 2  // Define the GPIO pin connected to the DHT11
#define LED_PIN 14  // Define GPIO pin for the LED
#define TEMP_THRESHOLD 30  // Temperature threshold

static dht_t dev;

void *dht11_thread(void *arg) {
    int16_t temp, hum;
    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                    temp / 10, temp % 10, hum / 10, hum % 10);
            
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Error reading from DHT11\n");
        }
        xtimer_sleep(5);  // Wait for 5 seconds
    }
    return NULL;
}

int main(void) {
    // Initialize GPIO for LED
    gpio_init(LED_PIN, GPIO_OUT);
    
    // Initialize DHT11
    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT11;
    dht_init(&dev, &params);
    
    // Start the DHT11 reading thread
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  0, dht11_thread, NULL, "dht11_thread");

    // Prevent main from exiting
    while (1) {
        thread_yield();
    }
    return 0;
}