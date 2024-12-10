
#include "contiki-default-conf.h"
#include "sys/rtimer.h"
#include "net/ipv6/simple-udp.h"
#include "coap-observe-client.h"
#include "dev/etc/rgb-led/rgb-led.h"
#include "mqtt.h"
#include "sys/etimer.h"
#include "contiki-net.h"
#include "contiki.h"
#include "net/netstack.h"
#include "contiki-conf.h"
#include "net/ipv6/simple-udp.h"
#include "sys/etimer.h"
#include "contiki-net.h"
#include "dht11-sensor.h"
#include "contiki-conf.h"
#include "contiki-default-conf.h"
#include "net/ipv6/uip-udp-packet.h"
#include "contiki-lib.h"
#include "coap-blocking-api.h"
#include "sys/ctimer.h"
#include "sys/timer.h"
#include "coap-log.h"
#include "contiki-lib.h"
#include "coap-engine.h"
#include "random.h"
#include "dev/button-hal.h"
#include "coap-observe-client.h"
#include "net/routing/routing.h"
#include "mqtt-prop.h"
#include "net/ipv6/uiplib.h"

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN 2

static struct etimer et;

void read_dht11(void) {
  static clock_time_t last_read = 0;
  clock_time_t now = clock_time();

  if (now - last_read >= CLOCK_SECOND * 5) {
    last_read = now;
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PORT);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);
    dht11_sensor.configure(SENSORS_ACTIVE, 1);

    int humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
    int temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

    printf("Humidity: %d, Temperature: %d\n", humidity, temperature);
  }
}

int main(void) {
  clock_init();
  etimer_set(&et, CLOCK_SECOND * 5);

  while (1) {
    etimer_request_poll();
    if (etimer_expired(&et)) {
      read_dht11();
      etimer_reset(&et);
    }
  }

  return 0;
}
