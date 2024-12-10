
#include "contiki-default-conf.h"
#include "contiki-net.h"
#include "contiki.h"
#include "net/netstack.h"
#include "dht11-sensor.h"
#include "contiki-conf.h"
#include "contiki-lib.h"
#include "coap-blocking-api.h"
#include "coap-log.h"
#include "coap-engine.h"
#include "coap-observe-client.h"
#include "dev/button-hal.h"

#define DHT11_GPIO_PIN 2

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

static struct etimer timer;

PROCESS_THREAD(dht11_read_process, ev, data)
{
  PROCESS_BEGIN();

  // Configure the DHT11 sensor
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, NRF_GPIO_PORT_SELECT_PORT0);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);
  dht11_sensor.configure(SENSORS_ACTIVE, 1);

  // Set the timer to read the DHT11 sensor every 5 seconds
  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read the DHT11 sensor
    dht11_sensor.configure(SENSORS_READY, 1);

    // Get the temperature and humidity values
    int temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
    int humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

    // Print the values
    printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);

    // Reset the timer
    etimer_reset(&timer);
  }

  PROCESS_END();
}

int main() {
  // Initialize the Contiki system
  contiki_init();

  // Start the DHT11 read process
  process_start(&dht11_read_process, NULL);

  // Enter the Contiki main loop
  while(1) {
    process_run();
  }

  return 0;
}
