#include "thread.h"
#include "MQTTClient.h"
#include "dht.h"
#include "event/thread.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"

#define DHT_PIN GPIO_PIN(0, 2)  // Using GPIO 2 on the ESP32
#define DHT_TYPE DHT11
#define INTERVAL (5U) // 5 seconds interval

static kernel_pid_t dht_thread_pid;

static char dht_thread_stack[THREAD_STACKSIZE_MAIN];

static void *dht_thread(void *arg) {
    (void)arg;
    int16_t temp, hum;
    dht_t dev;
    dht_init(&dev, DHT_PIN, DHT_TYPE);

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temp: %d.%d°C, Hum: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        xtimer_sleep(INTERVAL);
    }
    return NULL;
}

int main(void) {
    puts("RIOT DHT11 example on ESP32");
    
    dht_thread_pid = thread_create(
        dht_thread_stack,
        sizeof(dht_thread_stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_WOUT_YIELD,
        dht_thread,
        NULL,
        "dht_reader"
    );

    /* To keep the main alive forever */
    thread_sleep();
    return 0;
}