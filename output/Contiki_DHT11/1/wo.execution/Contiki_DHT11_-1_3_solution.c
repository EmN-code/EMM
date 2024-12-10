#include "sys/etimer.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/psock.h"
#include "net/mac/framer/framer.h"
#include "net/netstack.h"
#include "coap-engine.h"
#include "coap-blocking-api.h"
#include "coap-log.h"
#include "dht11-sensor.h"
#include "contiki.h"

#include <stdio.h>

static struct etimer timer;

PROCESS(dht11_process, "DHT11 Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data)
{
  PROCESS_BEGIN();

  // Setup GPIO for the DHT11 on NRF52840
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, 0);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, 2);
  dht11_sensor.configure(SENSORS_ACTIVE, 1);

  while(1) {
    etimer_set(&timer, CLOCK_SECOND * 5);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    if(dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER) != -1) {
      printf("Temperature: %d.%d°C\n", dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER), dht11_sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL));
      printf("Humidity: %d.%d\n", dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER), dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL));
    } else {
      printf("Failed to read from DHT11 sensor\n");
    }
  }

  PROCESS_END();
}

int main(void) {
  process_start(&dht11_process, NULL);
  return 0;
}