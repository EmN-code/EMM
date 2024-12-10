#include "thread.h"
#include "architecture.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "dht_params.h"
#include "dht.h"
#include "event/thread.h"
#include "paho_mqtt.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "mma8x5x_regs.h"
#include "periph/gpio.h"
#include "thread_arch.h"
#include "mma8x5x_params.h"

#define THREAD_STACKSIZE_MAIN (THREAD_STACKSIZE_DEFAULT + THREAD_CREATE_STACKTEST)

static char stack[THREAD_STACKSIZE_MAIN];

void *read_dht11(void *arg) {
    (void) arg;
    dht_t dev;
    dht_params_t params;

    params.pin = GPIO_PIN(0, 2); // GPIO 2
    params.type = DHT11;
    
    if (dht_init(&dev, &params) == DHT_OK) {
        printf("DHT11 initialization succeeded\n");
    } else {
        printf("DHT11 initialization failed\n");
        return NULL;
    }

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Could not read data from DHT11\n");
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                                     THREAD_CREATE_STACKTEST,
                                     read_dht11, NULL, "read_dht11");

    if (pid <= KERNEL_PID_UNDEF) {
        printf("Failed to create thread\n");
        return 1;
    }

    return 0;
}
