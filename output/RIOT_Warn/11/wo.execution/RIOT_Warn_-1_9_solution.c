#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "thread.h"
#include "sys/socket.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stdio.h"
#include "stddef.h"
#include "string.h"
#include "periph/gpio.h"
#include "netinet/in.h"
#include "msg.h"
#include "mbox.h"
#include "mutex.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "kernel_defines.h"
#include "lib/ztimer/ztimer.h"
#include "lib/timex/timex.h"
#include "lib/evtimer/evtimer_msg.h"
#include "lib/evtimer/evtimer_mbox.h"
#include "kernel/include/kernel.h"
#include "esp_system.h"
#include "event/thread.h"
#include "dht_params.h"
#include "dht.h"
#include "arpa/inet.h"
#include "MQTTClient.h"

#define THREAD_STACKSIZE_DEFAULT (1024)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)

#define TEMP_THRESHOLD 30  // Example temperature threshold in degree Celsius
#define DHT_PIN GPIO_PIN(PA,0)
#define LED_PIN GPIO_PIN(PA,1)
#define SLEEP_INTERVAL (5U * US_PER_SEC)  // 5 seconds sleep interval

static char thread_stack[THREAD_STACKSIZE_DEFAULT];

void *read_dht(void *arg) {
    (void)arg;  // Unused parameter
    int16_t temp, hum;
    dht_t dev = {.pin = DHT_PIN};  // Initialize DHT device
    gpio_init(LED_PIN, GPIO_OUT);
    while (1) {
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d.%d C, Humidity: %d.%d %%\n", 
                   temp / 10, temp % 10, hum / 10, hum % 10);
            if (temp > TEMP_THRESHOLD * 10) {  // Threshold check
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read sensor\n");
        }
        ztimer_sleep(ZTIMER_SEC, SLEEP_INTERVAL);
    }
    return NULL;
}

int main(void) {
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN, 0,
                  read_dht, NULL, "dht_thread");
    return 0;
}
