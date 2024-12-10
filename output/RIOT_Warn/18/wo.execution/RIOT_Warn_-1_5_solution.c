#include "thread.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "paho_mqtt.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "dht_params.h"

#define DHT_PIN GPIO_PIN(0, 1)
#define LED_PIN GPIO_PIN(0, 2)
#define TEMP_THRESHOLD 30

static kernel_pid_t main_thread_pid;

void* read_dht_thread(void *arg) {
    (void) arg;
    int16_t temp, hum;
    dht_t dev;

    // Initialize the DHT sensor
    dht_init(&dev, DHT_PIN);

    while (1) {
        if (dht_read(&dev, &temp, &hum) == 0) {
            if (temp > (TEMP_THRESHOLD * 10)) { // DHT11 gives temp in tenths of degrees
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        }
        ztimer_sleep(ZTIMER_MSEC, 5000); // Sleep for 5 seconds
    }

    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, ARRAY_SIZE(msg_queue));

    // Initialize GPIO for LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Create thread for reading DHT sensor
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, read_dht_thread, NULL, "read_dht_thread");

    main_thread_pid = thread_getpid();

    // Enter infinite loop
    while (1) {
        event_wait(main_thread_pid);
    }

    return 0;
}