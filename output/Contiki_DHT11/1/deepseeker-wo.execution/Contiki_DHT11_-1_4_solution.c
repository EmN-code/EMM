
#include "contiki-default-conf.h"
#include "coap-observe-client.h"
#include "dev/etc/rgb-led/rgb-led.h"
#include "mqtt.h"
#include "sys/etimer.h"
#include "contiki-net.h"
#include "contiki.h"
#include "net/netstack.h"
#include "contiki-conf.h"
#include "coap-observe.h"
#include "net/ipv6/simple-udp.h"
#include "contiki-lib.h"
#include "coap-blocking-api.h"
#include "coap-log.h"
#include "coap-engine.h"
#include "net/routing/routing.h"
#include "mqtt-prop.h"

#define DHT11_GPIO_PIN 2
#define READ_INTERVAL (5 * CLOCK_SECOND)

static struct etimer read_timer;

static void read_dht11(void) {
    static uint8_t dht11_data[5];
    uint8_t checksum;

    // Simulate reading from DHT11 (replace with actual GPIO read logic)
    dht11_data[0] = 50; // Humidity integer part
    dht11_data[1] = 0;  // Humidity decimal part
    dht11_data[2] = 25; // Temperature integer part
    dht11_data[3] = 0;  // Temperature decimal part
    dht11_data[4] = dht11_data[0] + dht11_data[1] + dht11_data[2] + dht11_data[3]; // Checksum

    checksum = dht11_data[0] + dht11_data[1] + dht11_data[2] + dht11_data[3];
    if (checksum == dht11_data[4]) {
        printf("Humidity: %d.%d %% Temperature: %d.%d C\n",
               dht11_data[0], dht11_data[1], dht11_data[2], dht11_data[3]);
    } else {
        printf("Checksum error\n");
    }
}

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data) {
    PROCESS_BEGIN();

    etimer_set(&read_timer, READ_INTERVAL);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&read_timer));
        read_dht11();
        etimer_reset(&read_timer);
    }

    PROCESS_END();
}

int main(void) {
    contiki_init();
    return 0;
}
