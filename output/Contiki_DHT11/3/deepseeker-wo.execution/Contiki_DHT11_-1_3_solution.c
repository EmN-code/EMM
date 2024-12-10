
#include "contiki.h"
#include "contiki-net.h"
#include "net/netstack.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/tcpip.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/uip-udp-packet.h"
#include "contiki-conf.h"
#include "contiki-default-conf.h"
#include "contiki-lib.h"
#include "coap-engine.h"
#include "coap-blocking-api.h"
#include "coap-log.h"
#include "dev/button-hal.h"
#include "dht11-sensor.h"

#define DHT11_GPIO_PIN 2

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
  static struct etimer timer;
  static int humidity, temperature;

  PROCESS_BEGIN();

  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, 0);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);
  dht11_sensor.configure(SENSORS_HW_INIT, 0);

  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    dht11_sensor.configure(SENSORS_ACTIVE, 1);

    if(dht11_sensor.status(0) == DHT11_STATUS_OKAY) {
      humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
      temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

      printf("Humidity: %d%%\n", humidity);
      printf("Temperature: %d°C\n", temperature);
    } else {
      printf("Failed to read DHT11 sensor\n");
    }

    etimer_reset(&timer);
  }

  PROCESS_END();
}

int main()
{
  contiki_init();
  process_start(&dht11_read_process, NULL);
  process_run();
  return 0;
}
