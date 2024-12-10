
#include "contiki.h"
#include "sys/etimer.h"
#include "dev/gpio-hal.h"
#include "dht11-sensor.h"

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN 2

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

static struct etimer read_timer;

PROCESS_THREAD(dht11_read_process, ev, data)
{
  PROCESS_BEGIN();

  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PORT);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);

  etimer_set(&read_timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&read_timer));

    dht11_sensor.configure(SENSORS_ACTIVE, 1);

    int humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
    int temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

    etimer_reset(&read_timer);
  }

  PROCESS_END();
}

int main(void)
{
  process_start(&dht11_read_process, NULL);
  return 0;
}
