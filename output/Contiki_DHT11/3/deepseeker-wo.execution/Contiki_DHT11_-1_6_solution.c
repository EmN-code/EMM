
#include "contiki.h"
#include "sys/etimer.h"
#include "dev/gpio-hal.h"
#include "dht11-sensor.h"

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN 2

static struct etimer et;

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
  PROCESS_BEGIN();

  // Configure the DHT11 sensor
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PORT);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);
  dht11_sensor.configure(SENSORS_ACTIVE, 1);

  while(1) {
    // Set the etimer to trigger every 5 seconds
    etimer_set(&et, CLOCK_SECOND * 5);

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    // Read the DHT11 sensor
    dht11_sensor.configure(SENSORS_ACTIVE, 1);

    // Print the values
    printf("Humidity: %d.%d %% Temperature: %d.%d C\n",
           dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER),
           dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL),
           dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER),
           dht11_sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL));
  }

  PROCESS_END();
}

int main(void)
{
  contiki_init();
  return 0;
}
