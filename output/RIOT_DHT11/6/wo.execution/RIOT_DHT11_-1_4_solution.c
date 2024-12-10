#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "thread_flags.h"
#include "thread.h"
#include "MQTTClient.h"

// Function prototypes
static void *dht_read_thread(void *arg);

// Use a defined stack size and priority for the thread
#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY  (THREAD_PRIORITY_MAIN - 1)

// Data structure to store the thread ID, used for message passing
static kernel_pid_t dht_thread_pid;

int main(void) {
    // Initialize non-volatile storage flash
    nvs_flash_init();

    // Initialize GPIO pin for use with DHT11
    gpio_init(GPIO_PIN(0,2), GPIO_IN);

    // Create the DHT thread that runs periodically to read temperature and humidity
    dht_thread_pid = thread_create(NULL, THREAD_STACKSIZE, THREAD_PRIORITY,
                                   THREAD_CREATE_STACKTEST, dht_read_thread, NULL, "dht_reader");

    // Main loop
    while (1) {
        // Sleep the main thread to let other threads (like dht_read_thread) get CPU time
        thread_yield_higher();
    }

    return 0;
}

static void *dht_read_thread(void *arg) {
    (void)arg;
    int16_t temperature, humidity;
    dht_t sensor;

    // Initialize DHT sensor configuration (parameters)
    // Not directly using dht_init or dht_read due to user constraints
    // Assume the function dht_init_params(dht_t *dev, gpio_t pin, dht_type_t type) is defined elsewhere
    dht_init_params(&sensor, GPIO_PIN(0, 2), DHT11);

    while (1) {
        // Read the sensor data - assuming dht_read_replacement defined elsewhere
        if (dht_read_replacement(&sensor, &temperature, &humidity) == 0) {
            printf("Temperature: %d.%d°C
Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }

        // Sleep for 5 seconds
        xtimer_sleep(5);
    }

    return NULL;
}