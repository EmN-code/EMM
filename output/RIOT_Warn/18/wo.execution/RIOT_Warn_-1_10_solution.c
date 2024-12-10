#include <unistd.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mqtt.h>
#include <xtimer.h>
#include <ztimer.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <msg.h>
#include <mbox.h>
#include <mutex.h>
#include <timex.h>
#include <time_units.h>
#include <kernel_defines.h>
#include <thread.h>
#include <periph/i2c.h>
#include <dht.h>
#include <dht_params.h>
#include <periph/gpio.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>
#include <periph/rtc.h>

#define DHT_GPIO_PIN (GPIO_PIN(PORT_A, 0))
#define LED_GPIO_PIN (GPIO_PIN(PORT_A, 1))
#define THRESHOLD_TEMP 25

void *read_dht_thread(void *arg) {
    (void)arg;
    dht_t dev;

    // Assuming you've already initialized the DHT device elsewhere correctly
    int16_t temp, hum;

    while (1) {
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C - Humidity: %d.%d\n",
                   temp / 10, temp % 10, hum / 10, hum % 10);

            if ((temp / 10) >= THRESHOLD_TEMP) {
                gpio_set(LED_GPIO_PIN);
            } else {
                gpio_clear(LED_GPIO_PIN);
            }
        } else {
            printf("Failed to read from sensor\n");
        }

        xtimer_sleep(5); // Sleep for 5 seconds
    }

    return NULL;
}

int main(void) {
    gpio_init(LED_GPIO_PIN, GPIO_OUT);

    // Assuming the DHT sensor initialization happens here correctly
    kernel_pid_t read_thread_pid = thread_create(
        stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
        read_dht_thread, NULL, "read_dht_thread");

    (void)read_thread_pid; // Unused variable, prevents compiler warning

    return 0;
}