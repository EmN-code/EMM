#include "contiki.h"
#include "dev/leds.h"
#include "dht11-sensor.h"
#include "sys/clock.h"
#include net/ipv6/uip-icmp6.h
#include net/ipv6/uip-sr.h
#include net/ipv6/psock.h
#include net/ipv6/uip-ds6-route.h
#include net/ipv6/uiplib.h
#include net/ipv6/uip-ds6.h
#include net/ipv6/udp-socket.h
#include "mqtt.h"
#include net/ipv6/tcpip.h
#include net/ipv6/tcp-socket.h
#include "coap-request-state.h"
#include net/ipv6/simple-udp.h
#include net/ipv6/uip.h
#include net/ipv6/uip-nameserver.h
#include net/routing/routing.h
#include net/netstack.h
#include "contiki-net.h"
#include net/ipv6/uip-udp-packet.h
#include coap-observe-client.h

#define LEDS_RED    LEDS_ALL
#define LEDS_GREEN    LEDS_ALL
#define LEDS_BLUE    LEDS_ALL

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
  static struct etimer timer;
  static int temperature, humidity;

  PROCESS_BEGIN();

  while(1) {
    etimer_set(&timer, CLOCK_SECOND * 5);

    SENSORS_ACTIVATE(dht11_sensor);
    temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
    humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

    SENSORS_DEACTIVATE(dht11_sensor);

    printf("Temperature: %d C, Humidity: %d\n", temperature, humidity);

    leds_on(LEDS_RED);
    leds_on(LEDS_GREEN);
    leds_on(LEDS_BLUE);
    
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    leds_off(LEDS_RED);
    leds_off(LEDS_GREEN);
    leds_off(LEDS_BLUE);
  }

  PROCESS_END();
}