#include "net/ipv6/uip.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/tcpip.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/netstack.h"
#include "net/routing/routing.h"
#include "coap-timer.h"
#include "coap-request-state.h"
#include "coap-observe.h"
#include "coap-observe-client.h"
#include "coap-engine.h"
#include "coap-blocking-api.h"
#include "mqtt.h"
#include "contiki-net.h"
#include "contiki-default-conf.h"
#include "contiki.h"
#include "dht11-sensor.h"
#include "dev/button-hal.h"
#include <stdio.h>

#define LED_PIN_1 17
#define LED_PIN_2 18
#define LED_PIN_3 19

static struct etimer timer;

PROCESS(dht11_process, "DHT11 Process");
AUTOSTART_PROCESSES(&dht11_process);

void read_dht11_and_control_led(void) {
    int temperature_integer = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
    int humidity_integer = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

    printf("Temperature: %dC\n", temperature_integer);
    printf("Humidity: %d%%\n", humidity_integer);

    // Simplified logic to control RGB LED
    if (temperature_integer > 25) {
        NRF_GPIO->OUTSET = (1UL << LED_PIN_1); // Turn on LED if temperature > 25C
    } else {
        NRF_GPIO->OUTCLR = (1UL << LED_PIN_1); // Turn off LED otherwise
    }

    if (humidity_integer > 50) {
        NRF_GPIO->OUTSET = (1UL << LED_PIN_2); // Turn on LED if humidity > 50%
    } else {
        NRF_GPIO->OUTCLR = (1UL << LED_PIN_2); // Turn off LED otherwise
    }
}

PROCESS_THREAD(dht11_process, ev, data)
{
    PROCESS_BEGIN();
    
    SENSORS_ACTIVATE(dht11_sensor);

    while(1) {
        etimer_set(&timer, CLOCK_SECOND * 5);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        read_dht11_and_control_led();
    }

    PROCESS_END();
}