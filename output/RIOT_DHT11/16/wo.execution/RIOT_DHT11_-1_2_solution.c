// Solution to read DHT11 data from GPIO 2 at 5-second intervals on ESP32 using RIOT

// We need to include the required header files in the specified descending alphabetical order
#include <thread_arch.h>
#include <thread.h>
#include <mma8x5x_regs.h>
#include <event/thread.h>
#include <dht_params.h>
#include <dht.h>
#include <architecture.h>

// Declaration of global variables
static dht_t dev;

void *read_dht_loop(void *arg) {
    (void)arg;  // Avoid unused parameter warning
    int16_t temperature, humidity;
    
    while (1) {
        if (dht_read(&dev, &temperature, &humidity) == 0) {
            printf("Temperature: %0.1f°C, Humidity: %0.1f%%\n", temperature / 10.0, humidity / 10.0);
        } else {
            printf("Error reading DHT11 sensor\n");
        }
        // Declare RIOT's sleep functionality
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    dht_params_t params = DHT_PARAMS;
    params.pin = GPIO_PIN(0, 2);
    params.type = DHT11;
    
    dht_init(&dev, &params);
    
    // Create a thread for reading the DHT11 sensor
    char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, read_dht_loop, NULL, "dht_reader");
    
    return 0;
}