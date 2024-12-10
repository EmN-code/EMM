#include "thread_arch.h"
#include "thread.h"
#include "paho_mqtt.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "dht.h"
#include "event/thread.h"
#include "architecture.h"
#include "MQTTClient.h"

#define DHT_PIN 2 // Define the GPIO pin where the DHT11 is connected
#define READ_INTERVAL (5U) // 5 seconds interval

static char stack[THREAD_STACKSIZE_MAIN];

void *dht11_thread(void *arg) {
    (void) arg;
    
    dht_t dev;
    int16_t temperature, humidity;
    
    while (1) {
        if (dht_read(&dev, &temperature, &humidity) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                   temperature / 10, abs(temperature % 10), humidity / 10, abs(humidity % 10));
        } else {
            printf("Failed to read data from DHT11 sensor\n");
        }
        
        xtimer_sleep(READ_INTERVAL);
    }

    return NULL;
}

int main(void) {
    puts("RIOT DHT11 Example");

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, 0, dht11_thread, NULL, "dht11");

    return 0;
}