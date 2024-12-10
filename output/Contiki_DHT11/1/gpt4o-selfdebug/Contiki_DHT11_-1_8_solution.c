#include "os/sys/etimer.h"
#include "os/dev/gpio-hal.h"
#include "os/dev/dht11-sensor.h"
#include "os/net/netstack.h"
#include "stdio.h"

#define DHT11_PIN 2

PROCESS(dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data)
{
  static struct etimer timer;
  static int temperature, humidity;
  
  PROCESS_BEGIN();
  
  // Set the GPIO pin connected to DHT11 as input
  gpio_hal_arch_pin_set_input(DHT11_PIN);

  while(1) {
    // Set a timer for 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);
    
    // Wait for the timer event
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    
    // Activate the DHT11 sensor
    SENSORS_ACTIVATE(dht11_sensor);
    
    // Read temperature and humidity
    temperature = dht11_sensor.value(DHT11_SENSOR_TEMP);
    humidity = dht11_sensor.value(DHT11_SENSOR_HUMIDITY);
    
    // Print the values
    printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
    
    // Deactivate the sensor to save energy
    SENSORS_DEACTIVATE(dht11_sensor);
  }
  
  PROCESS_END();
}