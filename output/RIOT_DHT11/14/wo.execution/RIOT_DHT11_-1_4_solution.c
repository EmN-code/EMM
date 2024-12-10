
#include "thread.h"
#include "MQTTClient.h"
#include "dht_params.h"
#include "dht.h"
#include "event/thread.h"
#include "paho_mqtt.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "mma8x5x_regs.h"
#include "periph/gpio.h"

#define THREAD_PRIORITY   (THREAD_PRIORITY_MAIN - 1)
#define THREAD_STACKSIZE  (THREAD_STACKSIZE_MAIN)

dht_t dev;
kernel_pid_t thread_pid;
static char thread_stack[THREAD_STACKSIZE];

void *read_dht(void *arg) {
    (void)arg;
    int16_t temperature, humidity;
    while (1) {
        if (dht_read(&dev, &temperature, &humidity) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        } else {
            printf("Could not read data from DHT11 sensor\n");
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    dht_params_t params;
    params.pin = GPIO_PIN(0, 2);
    params.type = DHT11;

    if (dht_init(&dev, &params) != DHT_OK) {
        printf("DHT11 initialization failed\n");
        return 1;
    }
    
    thread_pid = thread_create(thread_stack, sizeof(thread_stack),
                               THREAD_PRIORITY, 0, read_dht,
                               NULL, "read_dht");
    
    if (thread_pid <= KERNEL_PID_UNDEF) {
        printf("Error initializing thread\n");
        return 1;
    }
    return 0;
}