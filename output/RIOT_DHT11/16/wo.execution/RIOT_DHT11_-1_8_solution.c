#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "thread.h"
#include "thread_arch.h"
#include "architecture.h"
#include "dht_params.h"
#include "dht.h"
#include "paho_mqtt.h"

#include <stdint.h>
#include <stdlib.h>

#define DHT_PIN 2
#define STACKSIZE THREAD_STACKSIZE_DEFAULT
#define PRIORITY THREAD_PRIORITY_MAIN - 1

void *dht_read_thread(void *arg) {
    (void)arg;
    dht_t dev;
    int16_t temperature, humidity;
    
    // Initialize DHT sensor structure
    while(1) {
        if (dht_read(&dev, &temperature, &humidity) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                   temperature / 10, temperature % 10, humidity / 10, humidity % 10);
        }
        else {
            printf("Could not read data from sensor\n");
        }
        
        // Sleep for 5 seconds
        sleep(5);
    }
    return NULL;
}

int main(void) {
    char stack[STACKSIZE];
    thread_create(stack, sizeof(stack), PRIORITY, 0, dht_read_thread, NULL, "dht_reader");
    return 0;
}