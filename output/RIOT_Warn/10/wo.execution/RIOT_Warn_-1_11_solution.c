#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stdbool.h"
#include "sys/socket.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "event/thread.h"
#include "ztimer.h"
#include "ztimer_sleep"
#include "xtimer.h"
#include "unistd.h"
#include "time_units.h"
#include "timex.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mm8x5x.h"
#include "mm24x5x_regs.h"
#include "mm8x5x_params.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "mbox.h"
#include "dht.h"
#include "dht_params.h"
#include "msg.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "thread.h"
#include "kernel_defines.h"
#include "mutex.h"
#include "arpa/inet.h"
#include "netinet/in.h"

#define DHT_PIN  GPIO_PIN(0, 4)  // Define GPIO PIN where DHT11 is connected
#define LED_PIN GPIO_PIN(0, 2)  // Define GPIO PIN where LED is connected
#define TEMP_THRESHOLD 30  // Define the temperature threshold

static void *dht_thread(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_t));

    dht_t dev;
    if (dht_init(&dev, DHT_PIN, DHT11) != 0) {
        printf("Failed to initialize DHT sensor\n");
        return NULL;
    }

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) != 0) {
            printf("Failed to read DHT sensor\n");
        } else {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            
            if (temp / 10 > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return NULL;
}

#define THREAD_STACKSIZE_DEFAULT (1024)
char thread_stack[THREAD_STACKSIZE_DEFAULT];
msg_t msg_queue[8];  // Message queue for the thread

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, dht_thread, NULL, "dht_thread");

    return 0;
}