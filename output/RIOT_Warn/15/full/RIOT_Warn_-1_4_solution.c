// Required header files for the RIOT application
#include <string.h>
#include <mma8x5x_params.h>
#include <dht_params.h>
#include <thread.h>
#include <dht.h>
#include <mma8x5x.h>
#include <MQTTClient.h>
#include <mma8x5x_regs.h>
#include <event/thread.h>
#include <periph/rtc.h>
#include <periph/gpio.h>
#include <periph/i2c.h>

// Define a threshold temperature for LED activation
#define TEMP_THRESHOLD 30

// GPIO pin for LED
#define LED_PIN GPIO_PIN

// Stack size for the thread
#define THREAD_STACKSIZE (THREAD_STACKSIZE_DEFAULT)

// Function prototypes
void *dht_thread(void *arg);

// Define a stack and a thread, ensuring proper declarations
char dht_thread_stack[THREAD_STACKSIZE];
kernel_pid_t dht_thread_pid;

// Main entry of the program
int main(void) {
    // Initialize the LED GPIO pin
    if (gpio_init(LED_PIN, GPIO_OUT) < 0) {
        printf("Error initializing pin %d\n", LED_PIN);
        return 1;
    }

    // Create the thread to read the DHT11 sensor
    dht_thread_pid = thread_create(dht_thread_stack, THREAD_STACKSIZE,
                                   THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                   dht_thread, NULL, "dht_thread");

    while (1) {
        // Placeholder for maintaining main thread activity if needed
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    return 0;
}

void *dht_thread(void *arg) {
    // Define and initialize the DHT11 sensor
    dht_t dht;
    dht_params_t params;
    params.pin = GPIO_PIN;
    dht_init(&dht, &params);

    // Stores temperature and humidity values
    int16_t temp, hum;

    // Simulate networking logic 
    while (1) {
        if (NetworkConnect() != 0) {
            printf("Network connection failed. Retrying...\n");
            ztimer_sleep(ZTIMER_SEC, 1);
            continue;
        } else {
            printf("Network connected successfully.\n");
            break;
        }
    }

    while (1) {
        // Read the temperature and humidity from the DHT11 sensor
        if (dht_read(&dht, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);

            // Turn on LED if temperature exceeds threshold
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
                printf("Warning! Temperature exceeds threshold!\n");
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_SEC, 5);
    }
    return NULL;
}