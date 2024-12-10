
#include "contiki-default-conf.h"
#include "sys/rtimer.h"
#include "coap-observe-client.h"
#include "sys/etimer.h"
#include "contiki-net.h"
#include "contiki.h"
#include "net/netstack.h"
#include "contiki-conf.h"
#include "contiki-lib.h"
#include "coap-blocking-api.h"
#include "sys/ctimer.h"
#include "sys/timer.h"
#include "coap-log.h"
#include "coap-engine.h"
#include "dev/button-hal.h"
#include "dht11-sensor.h"

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN 2

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
  static struct etimer et;
  static int humidity, temperature;

  PROCESS_BEGIN();

  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PORT);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);
  dht11_sensor.configure(SENSORS_ACTIVE, 1);

  etimer_set(&et, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    if(dht11_sensor.status(0) == DHT11_STATUS_OKAY) {
      humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
      temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
      printf("Humidity: %d %%, Temperature: %d C\n", humidity, temperature);
    } else {
      printf("Failed to read DHT11 sensor\n");
    }

    etimer_reset(&et);
  }

  PROCESS_END();
}

int main() {
  process_init();
  process_start(&etimer_process, NULL);
  process_start(&dht11_read_process, NULL);
  while(1) {
    process_run();
  }
  return 0;
}
