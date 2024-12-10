#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/tcpip.h"
#include "net/ipv6/uiplib.h"
#include "contiki-net.h"
#include "contiki.h"
#include "coap-engine.h"
#include "coap-log.h"
#include "coap-request-state.h"
#include "coap-observe-client.h"
#include "coap-blocking-api.h"
#include "dev/button-hal.h"
#include "dev/etc/rgb-led/rgb-led.h"
#include "dht11-sensor.h"
#include "mqtt.h"
#include "mqtt-prop.h"
#include "net/netstack.h"
#include "net/routing/routing.h"
#include "random.h"
#include "sys/etimer.h"

#define DHT11_PORT 0
#define DHT11_PIN 13

static struct etimer periodic_timer;

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data) {
    PROCESS_BEGIN();

    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, DHT11_PORT);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_PIN);

    etimer_set(&periodic_timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
        etimer_reset(&periodic_timer);

        SENSORS_ACTIVATE(dht11_sensor);

        int status = dht11_sensor.configure(SENSORS_ACTIVE, 1);
        if (status == DHT11_STATUS_OKAY) {
            int temp = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
            int humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

            // Simulating LED toggle
            if (temp > 25) {
                // Turn on the LED
                // This requires actual board API calls to manipulate LED
                printf("LED ON: Temperature: %d C, Humidity: %d %%\n", temp, humidity);
            } else {
                // Turn off the LED
                printf("LED OFF: Temperature: %d C, Humidity: %d %%\n", temp, humidity);
            }
        }
        SENSORS_DEACTIVATE(dht11_sensor);
    }
    PROCESS_END();
}