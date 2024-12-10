#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "paho_mqtt.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "thread.h"

#define DHT_PIN GPIO_PIN(0, 2)

void *dht_read_thread(void *arg) {
    (void) arg;
    dht_t dev;
    int16_t temp, hum;
    
    // Initialize DHT sensor
    dht_init(&dev, DHT_PIN);

    while (1) {
        // Read from sensor
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d, Humidity: %d\n", temp, hum);
        } else {
            printf("Failed to read from sensor\n");
        }
        
        // Sleep for 5 seconds
        thread_sleep(5);
    }
    return NULL;
}

int main(void) {
    thread_create(NULL, 0, THREAD_PRIORITY_MAIN, 0, dht_read_thread, NULL, "dht_reader");
    
    // Main loop can handle more tasks
    while (1) {
        thread_sleep(10);
    }
    return 0;
}