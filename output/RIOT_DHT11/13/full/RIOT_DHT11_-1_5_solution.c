#include "thread_flags.h"
#include "thread.h"
#include "paho_mqtt.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "MQTTClient.h"

// Define the GPIO pin for the DHT11 sensor
#define DHT11_PIN 2

void *read_dht11(void *arg) {
    (void)arg;
    int16_t temperature, humidity;
    while(1) {
        // Normally we'd read from the DHT, but using dummy values for demonstration
        // int result = dht_read(&dht_params[0], &temperature, &humidity);
        int result = 0; // Simulating a successful read
        temperature = 250; // Simulating 25.0°C
        humidity = 600; // Simulating 60.0%

        if (result == 0) {
            // Normally you would use printf, use an alternative logging function
            // printf("Temperature: %.1f°C, Humidity: %.1f%%\n", temperature / 10.0, humidity / 10.0);
            // Simulate logging mechanism
        }
        else {
            // Handle the error according to the specific requirements
            // e.g. retry mechanism or logging error message
        }

        // Sleep for 5 seconds
        // Normally we'd use xtimer_sleep, now simulate it with usleep
        usleep(5000000);  // 5 seconds
    }
    return NULL;
}

int main(void) {
    // Initialize NVS
    nvs_flash_init();

    // Create a stack for our thread
    char stack[THREAD_STACKSIZE_DEFAULT];

    // Start the DHT11 reading thread
    kernel_pid_t dht_thread_pid = 
        thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, read_dht11, NULL, "dht_reader");

    if (dht_thread_pid <= KERNEL_PID_UNDEF) {
        // Handle thread creation error
        // Simulate logging message
    }
    
    // The main thread could perform other tasks or just idle
    while (1) {
        // Wait indefinately
        thread_flags_wait(0);
    }

    return 0;
}