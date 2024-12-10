
#include "contiki.h"
#include "contiki-net.h"
#include "net/netstack.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
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
#include "dht11-sensor.h"

#define DHT11_GPIO_PIN 2

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

static struct etimer read_timer;

static void
dht11_init(void)
{
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, 0);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);
  dht11_sensor.configure(SENSORS_HW_INIT, 0);
}

static void
dht11_read_and_print(void)
{
  int humidity_integer, humidity_decimal, temperature_integer, temperature_decimal;

  dht11_sensor.configure(SENSORS_ACTIVE, 1);

  humidity_integer = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
  humidity_decimal = dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL);
  temperature_integer = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
  temperature_decimal = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL);

  coap_log_string("Humidity: ", 10);
  coap_log_string(humidity_integer, 1);
  coap_log_string(".", 1);
  coap_log_string(humidity_decimal, 1);
  coap_log_string("%\n", 2);

  coap_log_string("Temperature: ", 13);
  coap_log_string(temperature_integer, 1);
  coap_log_string(".", 1);
  coap_log_string(temperature_decimal, 1);
  coap_log_string("C\n", 2);
}

PROCESS_THREAD(dht11_read_process, ev, data)
{
  PROCESS_BEGIN();

  dht11_init();

  etimer_set(&read_timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&read_timer));

    dht11_read_and_print();

    etimer_reset(&read_timer);
  }

  PROCESS_END();
}
