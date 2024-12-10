#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <time_units.h>
#include <timex.h>
#include <thread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdio.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <mutex.h>
#include <mqttclient.h>
#include <msg_type.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <event/thread.h>
#include <dht_params.h>
#include <dht.h>
#include <arpa/inet.h>

#define DHT11_PIN GPIO_PIN(0, 2)
#define READ_INTERVAL (5U * US_PER_SEC)

static char thread_stack[THREAD_STACKSIZE_MAIN];

typedef struct {
    int temperature;
    int humidity;
} dht_data_t;

static dht_data_t dht_data;

void *read_dht11(void *arg) {
    (void) arg;
    msg_t msg;
    msg_init_queue(&msg, 8);

    while (1) {
        int16_t temperature = 0, humidity = 0;
        if (dht_read(DHT_SENSOR_DHT11, DHT11_PIN, &humidity, &temperature) == DHT_OK) {
            dht_data.temperature = temperature;
            dht_data.humidity = humidity;
            printf("Temperature: %d.%d C, Humidity: %d.%d \n",
                   temperature / 10, temperature % 10,
                   humidity / 10, humidity % 10);
        }
        else {
            printf("Failed to read from DHT11 sensor\n");
        }
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    thread_create(thread_stack, sizeof(thread_stack), 
                  THREAD_PRIORITY_MAIN - 1, 0, 
                  read_dht11, NULL, "read_dht11");
    
    msg_t msg;
    msg_init_queue(&msg, 8);

    while(1) {
        pause(); // Keeps main thread running
    }
    return 0;
}