#include <ztimer.h>
#include <stdlib.h>
#include <stdio.h>
#include <mutex.h>  
#include <event/thread.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <event/timeout.h>
#include <kernel_defines.h>
#include <thread.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <dht_params.h>
#include <sys/socket.h>
#include <unistd.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <architecture.h>
#include <mma8x5x.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <periph/i2c.h>
#include <periph/rtc.h>
#include <periph/gpio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <timex.h>
#include <xtimer.h>
#include <time_units.h>
#include <thread_config.h>

#define GPIO_PIN 2

void *thread_handler(void *arg) {
    (void)arg;
    int16_t temp, hum;
    dht_t dev;
    while (1) {
        if (dht_read(&dev, &temp, &hum) == 0) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
        } else {
            printf("Failed to read from sensor\n");
        }
        ztimer_sleep(ZTIMER_SEC, 5);
    }
    return NULL;
}

int main(void) {
    dht_t dev;

    // Initialize GPIO
    gpio_init(GPIO_PIN, GPIO_IN);

    // Create a thread to handle DHT11 reading
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, thread_handler, NULL, "dht11_thread");

    return 0;
}