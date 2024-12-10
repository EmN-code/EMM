#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <timex.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <esp_system.h>
#include <dht.h>
#include <periph/gpio.h>
#include <mutex.h>
#include <thread.h>
#include <msg.h>
#include <event/thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <sys/socket.h>
#include <kernel_defines.h>
#include <periph/i2c.h>
#include <periph/rtc.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <paho_mqtt.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "dht_params.h"

// Definitions
#define DHT_PIN  GPIO_PIN(PORT_A, 10) // Example pin definition, replace with actual pin used
#define LED_PIN  GPIO_PIN(PORT_B, 1)  // Example pin definition, replace with actual pin used
#define TEMP_THRESHOLD 30             // Temperature threshold for LED indication
#define SLEEP_INTERVAL (5 * 1000U * 1000U) // 5 seconds

// Thread stack
static char stack[THREAD_STACKSIZE_MAIN];

// Function to read DHT and control LED
void *dht_thread(void *arg) {
    (void)arg;
    int16_t temp, hum;
    dht_t dev;

    // Initialize the GPIO for the LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the DHT sensor
    dht_init(&dev, &dht_params[DHT_PIN]);

    while (1) {
        int res = dht_read(&dev, &temp, &hum);
        if (res == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
            // Turn on LED if temperature exceeds threshold
            if ((temp / 10) > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Error reading DHT sensor\n");
        }
        // Sleep for the defined interval
        ztimer_sleep(ZTIMER_MSEC, SLEEP_INTERVAL);
    }
}

int main(void) {
    // Create the thread for DHT reading and LED control
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST | THREAD_CREATE_SLEEPING, dht_thread, NULL, "dht_thread");
    // Start the DHT task thread
    thread_wakeup(stack);
    return 0;
}