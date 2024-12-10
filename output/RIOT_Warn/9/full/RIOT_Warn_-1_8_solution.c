#include <netinet/in.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <unistd.h>
#include <ztimer.h>
#include <paho_mqtt.h>
#include <net/sock/udp.h>
#include <arpa/inet.h>
#include <mutex.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <xtimer.h>
#include <time_units.h>
#include <timex.h>
#include <thread.h>
#include <stdatomic.h>
#include <dht_params.h>
#include <dht.h>
#include <gpio.h>

#define DHT_PIN GPIO_PIN(0, 4)
#define LED_PIN GPIO_PIN(0, 5)
#define TEMP_THRESHOLD 30

void read_dht_sensor(void) {
    dht_t dev;
    int16_t temp, hum;
    dht_setup(&dev, DHT_PIN);

    while (1) {
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d, Humidity: %d\n", temp, hum);
            if (temp > TEMP_THRESHOLD) {
                gpio_set(LED_PIN);
            } else {
                gpio_clear(LED_PIN);
            }
        } else {
            printf("Failed to read from DHT sensor\n");
        }
        xtimer_sleep(5); // wait for 5 seconds
    }
}

int main(void) {
    gpio_init(DHT_PIN, GPIO_IN);
    gpio_init(LED_PIN, GPIO_OUT);

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, read_dht_sensor, NULL, "dht_sensor_thread");
    return 0;
}
