
#include "contiki.h"
#include "sys/etimer.h"
#include "dev/gpio-hal.h"
#include "dht11-sensor.h"

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN 2
#define READ_INTERVAL (5 * CLOCK_SECOND)

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
  static struct etimer et;

  PROCESS_BEGIN();

  // Configure DHT11 GPIO
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PORT);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);

  // Set the initial timer
  etimer_set(&et, READ_INTERVAL);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    // Read DHT11 sensor
    dht11_sensor.configure(SENSORS_ACTIVE, 1);

    // Reset the timer for the next reading
    etimer_reset(&et);
  }

  PROCESS_END();
}

int main(void)
{
  // Initialize Contiki
  contiki_init();

  // Start the process
  process_start(&dht11_read_process, NULL);

  // Run the process
  process_run();

  return 0;
}
