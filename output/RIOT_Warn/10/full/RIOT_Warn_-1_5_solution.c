#include "mma8x5x_params.h"
#include "dht_params.h"
#include "thread.h"
#include "dht.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "mma8x5x_regs.h"
#include "event/thread.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "periph/i2c.h"

#define TEMP_THRESHOLD 30
#define GPIO_LED 2
#define DHT_PIN 4

void *read_dht_and_control_led(void *arg) {
    (void)arg;
    dht_t dev;
    int16_t temp, hum;
    
    while (1) {
        int rc = dht_read(&dev, &temp, &hum);
        if (rc != 0) {
            printf("DHT read failed\n");
            continue;
        }

        printf("Temperature: %d, Humidity: %d\n", temp, hum);

        if (temp > TEMP_THRESHOLD) {
            printf("Temperature exceeded threshold!\n");
            gpio_set(GPIO_LED);
        } else {
            gpio_clear(GPIO_LED);
        }
        ztimer_sleep(ZTIMER_SEC, 5);
    }
    return NULL;
}

int main(void) {
    gpio_init(GPIO_LED, GPIO_OUT);
    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1, 0,
                  read_dht_and_control_led, NULL, "DHT Thread");
    return 0;
}