#include "thread_flags.h"
#include "thread.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "dht.h"
#include "MQTTClient.h"
#include "event/thread.h"

#include "xtimer.h"
#include "periph/gpio.h"

#define DHT_TYPE DHT11
#define DHT_PIN GPIO_PIN(0, 2) // GPIO2

static dht_t dht_dev;

void *read_dht(void *arg) {
    (void)arg;
    int16_t temperature, humidity;
    while (1) {
        if (dht_read(&dht_dev, &temperature, &humidity) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                   temperature / 10, abs(temperature % 10),  
                   humidity / 10, abs(humidity % 10));
        } else {
            puts("Error reading from DHT sensor");
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_t));

    dht_params_t params;
    params.pin = DHT_PIN;
    params.type = DHT_TYPE;
    
    if (dht_init(&dht_dev, &params) != 0) {
        puts("Failed to initialize DHT sensor");
        return 1;
    }

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  read_dht, NULL, "dht_reader");

    return 0;
}