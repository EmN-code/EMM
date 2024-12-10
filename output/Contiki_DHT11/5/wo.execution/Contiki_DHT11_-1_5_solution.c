#include "coap-blocking-api.h"
#include "net/ipv6/uip-ds6.h"
#include "mqtt.h"
#include "net/ipv6/simple-udp.h"
#include "dev/button-hal.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-nameserver.h"
#include "contiki-conf.h"
#include "sys/etimer.h"
#include "net/mac/framer/framer.h"
#include "sys/pt.h"
#include "sys/clock.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uiplib.h"
#include "coap-observe-client.h"
#include "coap-log.h"
#include "net/ipv6/psock.h"
#include "random.h"
#include "contiki-default-conf.h"
#include "sys/process.h"
#include "sys/autostart.h"
#include "sensors.h"
#include "net/netstack.h"
#include "net/ipv6/uip.h"
#include "contiki-net.h"
#include "sys/rtimer.h"
#include "net/ipv6/uip-ds6-route.h"
#include "coap-engine.h"
#include "contiki.h"
#include "contiki-conf.h"
#include "contiki-lib.h"
#include "net/routing/routing.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/tcpip.h"
#include "sys/timer.h"
#include "sys/ctimer.h"
#include "sys/log.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uip-icmp6.h"
#include "dht11-sensor.h"
#include "dev/etc/rgb-led/rgb-led.h"
#include "mqtt-prop.h"

PROCESS(dht11_example_process, "DHT11 Example Process");
AUTOSTART_PROCESSES(&dht11_example_process);

PROCESS_THREAD(dht11_example_process, ev, data)
{
  static struct etimer timer;
  PROCESS_BEGIN();

  // Initialize the DHT11 sensor
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, 0);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, 2);

  while(1) {
    // Set the timer for 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    // Wait for the timer event
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Trigger a new reading
    dht11_sensor.configure(SENSORS_ACTIVE, 1);

    // Log the values
    LOG_INFO("Temperature: %d.%d C, Humidity: %d.%d %%RH\n",
             dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER),
             dht11_sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL),
             dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER),
             dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL));
  }

  PROCESS_END();
}

int main(void) {
    process_start(&dht11_example_process, NULL);
    return 0;
}