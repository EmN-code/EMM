#include "sensors.h"
#include "contiki.h"
#include <stdio.h>

#define DHT11_SENSOR "dht11"
#define DHT11_SENSOR_GPIO 2

PROCESS(dht11_example_process, "DHT11 Sensor Example Process");
AUTOSTART_PROCESSES(&dht11_example_process);

PROCESS_THREAD(dht11_example_process, ev, data) {
  static struct etimer timer;
  PROCESS_BEGIN();

  // Configure DHT11 sensor (this should match your specific hardware setup)
  SENSORS_ACTIVATE(dht11_sensor);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, 0);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_SENSOR_GPIO);

  while(1) {
    etimer_set(&timer, CLOCK_SECOND * 5);  // Wait for 5 seconds
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    int humidity_int = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
    int humidity_dec = dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL);
    int temperature_int = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
    int temperature_dec = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL);

    printf("Humidity: %d.%d%%, Temperature: %d.%dC\n",
           humidity_int, humidity_dec, temperature_int, temperature_dec);
  }

  PROCESS_END();
}
