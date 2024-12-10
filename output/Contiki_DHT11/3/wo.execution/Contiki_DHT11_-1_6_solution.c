#include "dev/gpio-hal.h"
#include "sensors.h"
#include "coap-request-state.h"
#include "contiki.h"

PROCESS(example_process, "Example Process");
AUTOSTART_PROCESSES(&example_process);

PROCESS_THREAD(example_process, ev, data){
  static struct etimer timer;

  PROCESS_BEGIN();

  SENSORS_ACTIVATE(dht11_sensor);

  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, GPIO_HAL_PORT_A);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, 2);

  while(1){
    etimer_set(&timer, CLOCK_SECOND * 5);
    
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    if(dht11_sensor.status(SENSORS_READY) == DHT11_STATUS_OKAY){
        int humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
        int temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
        // Log the sensor readings
        printf("Humidity: %d%%\n", humidity);
        printf("Temperature: %d°C\n", temperature);
    }else{
        printf("Failed to read from DHT11 sensor!\n");
    }
  }

  SENSORS_DEACTIVATE(dht11_sensor);

  PROCESS_END();
}