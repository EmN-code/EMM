#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "thread_config.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "periph/rtc.h"
#include "msg.h"
#include "mbox.h"
#include "event/timeout.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "dht.h"
#include "time_units.h"
#include "MQTTClient.h"

#define DHT_PIN GPIO_PIN(0, 2)  // GPIO pin for the DHT11.
#define DHT_TYPE DHT11         // Sensor type can be either DHT11 or DHT22.

void *dht_thread(void *arg) {
    (void)arg;
    int16_t temp, hum;
    while (1) {
        if (dht_read(DHT_TYPE, DHT_PIN, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from the sensor\n");
        }
        xtimer_sleep(5);  // Wait for 5 seconds before the next read.
    }
    return NULL;
}

int main(void) {
    // Initialize the DHT sensor.
    if (dht_init(DHT_TYPE, DHT_PIN) != 0) {
        printf("Error: Unable to initialize the DHT sensor\n");
        return 1;
    }

    char stack[THREAD_STACKSIZE_MAIN];

    // Create a thread to periodically read from the sensor.
    kernel_pid_t dht_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                         THREAD_CREATE_STACKTEST, dht_thread, NULL, "dht_reader");

    if (dht_pid <= KERNEL_PID_UNDEF) {
        printf("Error: Unable to create thread\n");
        return 1;
    }

    return 0;
}