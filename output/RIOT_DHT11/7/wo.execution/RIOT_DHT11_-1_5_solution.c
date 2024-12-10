#include <periph/gpio.h>
#include <periph/i2c.h>
#include <periph/rtc.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <dht_params.h>
#include <dht.h>
#include <MQTTClient.h>
#include <architecture.h>
#include <event/thread.h>
#include <paho_mqtt.h>
#include <thread.h>
#include <thread_arch.h>

#include <stdio.h>

#define DHT_PIN GPIO_PIN(0, 2) // GPIO Pin 2 for the ESP32
#define READ_INTERVAL (5U * 1000000U) // Time interval for 5 seconds

static char thread_stack[THREAD_STACKSIZE_DEFAULT];

static void *dht_thread(void *arg) {
    (void)arg;
    int16_t temperature = 0, humidity = 0;
    while(1) {
        if (dht_read(&temperature, &humidity, 0) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", 
                   temperature / 10, temperature % 10, humidity / 10, humidity % 10);
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        xtimer_usleep(READ_INTERVAL); // Sleep for 5 seconds
    }
    return NULL;
}

int main(void) {
    gpio_init(DHT_PIN, GPIO_IN);
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST | THREAD_CREATE_SLEEPING,
                  dht_thread, NULL, "dht_thread");
    return 0;
}