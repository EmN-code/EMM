#include <thread_config.h>
#include <timex.h>
#include <xtimer.h>

#define DHT_PIN 2  // Define the GPIO pin where the DHT11 is connected
#define THREAD_STACKSIZE_THREAD (THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF)

// Function to read data from the DHT11 sensor
void *read_dht11(void *arg) {
    (void) arg; // Avoid unused parameter warning
    while (1) {
        // Normally you would call a DHT11 read function here
        // Simulate reading temperature and humidity
        int temperature = 25;  // Supposed temperature reading
        int humidity = 60;     // Supposed humidity reading

        printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);

        // Sleep for 5 seconds
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    // Thread for reading DHT11 sensor
    char stack[THREAD_STACKSIZE_THREAD];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, read_dht11, NULL, "dht11_thread");
    
    // The main thread can do other tasks, or just idle
    while (1) {
        // For simplicity, just put the main thread to sleep
        xtimer_sleep(60);
    }
    return 0;
}