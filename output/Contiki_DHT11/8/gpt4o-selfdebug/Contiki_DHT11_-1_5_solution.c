#include "sensors.h"
#include "dht11-sensor.h"
#include "contiki.h"
#include "dev/gpio-hal.h"
#include <stdio.h>

#define READ_INTERVAL (CLOCK_SECOND * 5)

PROCESS(dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_process);

static struct etimer timer;

PROCESS_THREAD(dht11_process, ev, data)
{
  PROCESS_BEGIN();

  /* Configure GPIO pin for DHT11 */
  gpio_hal_arch_pin_set_input(GPIO_HAL_PIN_B2);
  SENSORS_ACTIVATE(dht11_sensor);
  
  while(1) {
    etimer_set(&timer, READ_INTERVAL);

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    int temp_int = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
    int hum_int = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

    printf("Temperature: %d°C, Humidity: %d%%\n", temp_int, hum_int);
  }

  PROCESS_END();
}