#include "xtimer.h"
#include "ztimer.h"
#include "thread.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "unistd.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "MQTTClient.h"
#include "timex.h"
#include "time_units.h"
#include "paho_mqtt.h"
#include "periph/rtc.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "dht_params.h"
#include "dht.h"
#include "stdio.h"

#define DHT_PIN GPIO_PIN(PORT_A, 5)
#define LED_PIN GPIO_PIN(PORT_B, 0)
#define TEMP_THRESHOLD 30  // Temperature threshold in degree Celsius

void *dht_thread(void *arg) {
    (void) arg;
    dht_t dev;
    int16_t temp, hum;
    dht_init(&dev, &dht_params[0]);
    
    while (true) {
        dht_read(&dev, &temp, &hum);
        printf("Temperature: %d, Humidity: %d\n", temp, hum);

        if (temp / 10 > TEMP_THRESHOLD) {  // Convert deci-degrees to degrees
            gpio_set(LED_PIN);
        } else {
            gpio_clear(LED_PIN);
        }

        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    printf("Starting DHT11 monitoring with LED warning\n");
    gpio_init(LED_PIN, GPIO_OUT);
    
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST,
                  dht_thread, NULL, "dht");

    return 0;
}