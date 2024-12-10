#include <ztimer.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <msg.h>
#include <mma8x5x_params.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <netinet/in.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <xtimer.h>
#include <unistd.h>
#include <architecture.h>
#include <kernel_defines.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <time_units.h>
#include <timex.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <dht.h>
#include <dht_params.h>
#include <stdatomic.h>
#include <mutex.h>
#include <mbox.h>

#define DHT_PIN GPIO_PIN(0, 2)  // GPIO 2
#define DHT_TYPE DHT11
#define SLEEP_INTERVAL (5U * US_PER_SEC)

int main(void) {
    // Initialize GPIO pin
    if (gpio_init(DHT_PIN, GPIO_IN) != 0) {
        printf("Error initializing GPIO pin\n");
        return 1;
    }

    // Initialize DHT device
    dht_t dev;
    dev.params.type = DHT_TYPE;
    dev.params.pin = DHT_PIN;

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dev, &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp/10, temp%10, hum/10, hum%10);
        }
        else {
            printf("Failed to read data from DHT sensor\n");
        }
        ztimer_sleep(ZTIMER_USEC, SLEEP_INTERVAL);
    }
    return 0;
}