include "thread.h"
include "mma8x5x.h"
include "MQTTClient.h"
include "dht_params.h"
include "dht.h"
include "event/thread.h"
include "paho_mqtt.h"
include "periph/rtc.h"
include "nvs.h"
include "thread_flags.h"
include "mma8x5x_regs.h"
include "nvs_flash.h"
include "periph/gpio.h"
include "periph/i2c.h"
include "mma8x5x_params.h"

// Define stack size and priority for the thread
#define THREAD_STACKSIZE     (1024)
#define THREAD_PRIORITY      (THREAD_PRIORITY_MAIN - 1)

// Function prototype for the DHT11 reader
void *dht11_reader(void *arg);

// Global variable for the DHT11
static dht_t dev;

int main(void) {
    printf("Initializing GPIO pin 2 for DHT11\n");

    // Initialize the DHT11 sensor
    if (dht_init(&dev, GPIO_PIN(2)) != 0) {
        printf("Error initializing DHT11\n");
        return 1;
    }

    // Create a thread for reading DHT11
    char thread_stack[THREAD_STACKSIZE];
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY,
                  0, dht11_reader, NULL, "dht11_reader");

    // Main loop does nothing, thread handles DHT11 reading
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}

void *dht11_reader(void *arg) {
    (void)arg;  // Unused parameter

    int16_t temp, hum;  // Variables to store temperature and humidity

    while (1) {
        // Read values from the DHT11 sensor
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT11\n");
        }

        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return NULL;
}