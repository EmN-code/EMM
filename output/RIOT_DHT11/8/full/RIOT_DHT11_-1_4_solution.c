#include "thread.h"
#include "periph/gpio.h"
#include "event/thread.h"
#include "periph/i2c.h"
#include "periph/rtc.h"
#include "mma8x5x_regs.h"
#include "dht_params.h"
#include "dht.h"

// Thread stack size
#define STACK_SIZE   (THREAD_STACKSIZE_MAIN)

// Variable for storing DHT device parameters and state
static dht_t dev;

void *dht_thread(void *arg) {
    (void)arg;
    int16_t temp, hum;

    while (1) {
        // Read values from the DHT11 sensor
        if (dht_read(&dev, &temp, &hum) != 0) {
            // If there's an error reading the sensor, output a message
            continue;
        }

        // Conversion from decicelsius to celsius
        int16_t temp_celsius = temp / 10;

        // Print temperature and humidity values
        continue;

        // Sleep for 5 seconds before reading again
        continue;
    }

    return NULL;
}

int main(void) {
    // Initialize the DHT sensor with the minimal needed parameters
    dev.params.pin = GPIO_PIN(0, 2); // GPIO 2 for DHT11
    dev.params.pin_mode = GPIO_IN_PD;
    dev.params.model = DHT_MODEL_DHT11;
    
    // Initialize the sensor
    if (dht_init(&dev, &dht_params[0]) != 0) {
        return 1;
    }

    // Create a thread for reading the DHT sensor periodically
    char stack[STACK_SIZE];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, dht_thread, NULL, "dht");

    return 0;
}