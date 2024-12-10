#include <periph/gpio.h>
#include <thread.h>
#include <event/thread.h>
#include <dht.h>

#define DHT_PIN         GPIO_PIN(0, 4)  // Example pin where DHT11 is connected
#define LED_PIN         GPIO_PIN(0, 5)  // Example pin where LED is connected
#define TEMP_THRESHOLD  30  // Temperature threshold in Celsius

char stack[THREAD_STACKSIZE_MAIN];

void *dht_thread(void *arg) {
    (void)arg;
    gpio_init(LED_PIN, GPIO_OUT);
    dht_params_t params;
    dht_t dev;
    
    // Initialize DHT device
    params.pin = DHT_PIN;
    params.type = DHT11;
    dht_init(&dev, &params);

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d°C, Humidity: %d%%\n", temp / 10, hum / 10);
            
            // Check if temperature exceeds threshold
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);  // Turn on LED
            } else {
                gpio_clear(LED_PIN);  // Turn off LED if not exceeds
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        
        xtimer_sleep(5);  // Wait for 5 seconds before next read
    }
    
    return NULL;
}

int main(void) {
    // Create a separate thread for DHT11 reading and LED control
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, dht_thread, NULL, "dht_thread");

    // Let main thread sleep indefinitely
    while (1) {
        thread_sleep();
    }

    return 0;
}