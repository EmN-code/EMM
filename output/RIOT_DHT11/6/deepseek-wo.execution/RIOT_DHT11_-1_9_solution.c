
#include <arpa/inet.h>
#include <mma8x5x.h>
#include <periph/rtc.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <stdio.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stddef.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <stdlib.h>
#include <periph/i2c.h>
#include <event/thread.h>
#include <periph/gpio.h>
#include <mma8x5x_params.h>
#include <dht_params.h>
#include <esp_system.h>
#include <ztimer.h>
#include <netinet/in.h>
#include <xtimer.h>
#include <msg.h>
#include <timex.h>
#include <net/sock/udp.h>
#include <evtimer_mbox.h>
#include <mutex.h>
#include <thread.h>
#include <mbox.h>
#include <sys/socket.h>
#include <mma8x5x_regs.h>
#include <dht.h>
#include <stdbool.h>
#include <evtimer_msg.h>
#include <net/sock/util.h>
#include <time_units.h>
#include <kernel_defines.h>

#define DHT_PIN GPIO_PIN(0, 2)
#define DHT_TYPE DHT11

static dht_t dht_dev;

void read_dht(void) {
    int16_t temp, hum;
    if (dht_read(&dht_dev, &temp, &hum) == 0) {
        printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp / 10, temp % 10, hum / 10, hum % 10);
    } else {
        puts("Failed to read from DHT sensor");
    }
}

int main(void) {
    dht_params_t dht_params = { .pin = DHT_PIN, .type = DHT_TYPE };
    if (dht_init(&dht_dev, &dht_params) == DHT_OK) {
        while (1) {
            read_dht();
            ztimer_sleep(ZTIMER_MSEC, 5000);
        }
    } else {
        puts("Initialization of DHT sensor failed");
    }
    return 0;
}
