#include "event/thread.h"
#include "thread_arch.h"
#include "thread.h"
#include "paho_mqtt.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "dht_params.h"
#include "dht.h"
#include "architecture.h"

#define DHT_PIN GPIO_PIN(0,2)

void* read_dht(void* arg) {
    int16_t temperature, humidity;
    gpio_init(DHT_PIN, GPIO_IN);
    while (1) {
        if (dht_read(&humidity, &temperature, DHT11) == DHT_OK) {
            printf("Temperature: %d.%d C, Humidity: %d.%d %%\n", temperature/10, temperature%10, humidity/10, humidity%10);
        }
        else {
            printf("Failed to read DHT11\n");
        }
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return NULL;
}

int main(void) {
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, read_dht, NULL, "dht_thread");
    return 0;
}