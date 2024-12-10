#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip-nameserver.h"
#include "contiki-conf.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include <stdlib.h>
#include "net/ipv6/uip-udp-packet.h"
#include <stdbool.h>
#include "dev/radio.h"
#include "coap-observe-client.h"
#include "udp-socket.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uiplib.h"
#include <stdio.h>
#include "net/ipv6/psock.h"
#include "contiki-default-conf.h"
#include "udp-socket.h"
#include "net/ipv6/uiplib.h"
#include "coap-observe.h"
#include "net/netstack.h"
#include "net/ipv6/uip.h"
#include "contiki-net.h"
#include "net/ipv6/uip-ds6-route.h"
#include "coap-engine.h"
#include "contiki.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/tcpip.h"
#include "net/ipv6/tcp-socket.h"
#include "net/linkaddr.h"
#include "contiki-default-conf.h"
#include "coap-observe-client.h"
#include "net/ipv6/simple-udp.h"
#include "tcp-socket.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uip-icmp6.h"
#include "dht11-sensor.h"

PROCESS(nrf52840_dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&nrf52840_dht11_process);

static struct etimer et;

PROCESS_THREAD(nrf52840_dht11_process, ev, data)
{
    PROCESS_BEGIN();
    
    SENSORS_ACTIVATE(dht11_sensor);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, 0);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, 2);

    while(1) {
        etimer_set(&et, CLOCK_SECOND * 5);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        if(dht11_sensor.configure(SENSORS_ACTIVE, 1)) {
            if(dht11_sensor.status(0) == DHT11_STATUS_OKAY) {
                printf("Temperature: %d.%d C, Humidity: %d.%d%%\n",
                       dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER),
                       dht11_sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL),
                       dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER),
                       dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL));
            } else {
                printf("Failed to read from DHT11 sensor\n");
            }
        }
    }

    PROCESS_END();
}