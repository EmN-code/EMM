
#include "esp_system.h"
#include "time_units.h"
#include "net/emcute.h"
#include "sys/socket.h"
#include "mutex.h"
#include "stdlib.h"
#include "net/gcoap.h"
#include "arpa/inet.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "esp_wifi.h"
#include "evtimer_mbox.h"
#include "net/sock/udp.h"
#include "stdbool.h"
#include "evtimer_msg.h"
#include "kernel_defines.h"
#include "stdatomic.h"
#include "msg.h"
#include "stddef.h"
#include "timex.h"
#include "mbox.h"

#define LED_PIN GPIO_PIN(0, 13)
#define DHT_PIN GPIO_PIN(0, 14)
#define TEMP_THRESHOLD 30

void read_dht_and_check_temp(void) {
    int16_t temp, hum;
    // Simulate reading DHT11 data
    temp = 35; // Simulated temperature value
    hum = 50;  // Simulated humidity value

    if (temp > TEMP_THRESHOLD) {
        gpio_set(LED_PIN);
    } else {
        gpio_clear(LED_PIN);
    }
}

int main(void) {
    gpio_init(LED_PIN, GPIO_OUT);
    gpio_init(DHT_PIN, GPIO_IN);

    while (1) {
        read_dht_and_check_temp();
        xtimer_sleep(5);
    }

    return 0;
}
