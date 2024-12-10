#include <xtimer.h>
#include <thread.h>
#include <dht.h>
#include <event/thread.h>

#define DHT11_PIN  2
#define DHT_TYPE   DHT11

static char stack[THREAD_STACKSIZE_MAIN];

// Declare the function that reads sensor data
void *read_dht11(void *arg) {
    (void)arg;

    while (1) {
        int16_t temperature, humidity;
        if (dht_read(DHT_TYPE, DHT11_PIN, &humidity, &temperature) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10, humidity / 10, humidity % 10);
        } else {
            printf("Failed to read the sensor\n");
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN,
                  THREAD_CREATE_STACKTEST, read_dht11, NULL, "dht_reader");
    
    // Main thread can perform other tasks or sleep
    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}