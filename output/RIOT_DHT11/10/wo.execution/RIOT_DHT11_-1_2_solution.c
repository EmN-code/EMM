#include <xtimer.h>
#include <thread_config.h>
#include <thread.h>
#include <periph/gpio.h>
#include <event/thread.h>
#include <event/timeout.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <dht.h>

#define DHT_PIN GPIO_PIN(0, 2)
#define THREAD_STACKSIZE (THREAD_STACKSIZE_MAIN)
#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)

char stack[THREAD_STACKSIZE];

void* dht_thread(void *arg) {
    (void)arg;
    dht_params_t my_dht_params;
    my_dht_params.pin = DHT_PIN;
    my_dht_params.type = DHT11;

    printf("DHT thread started\n");

    while(1) {
        int16_t temp, hum;
        if (dht_read(&my_dht_params, &temp, &hum) != DHT_OK) {
            printf("Error reading DHT11\n");
        } 
        else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                    temp / 10, temp % 10, hum / 10, hum % 10);
        }

        xtimer_usleep(5000000); // Sleep for 5 seconds
    }

    return NULL;
}

int main(void) {
    thread_create(stack, sizeof(stack), THREAD_PRIORITY, 0, dht_thread, NULL, "dht_reader");
    return 0;
}
