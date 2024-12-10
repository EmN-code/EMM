#include "sys/etimer.h"
#include "sys/ctimer.h"
#include "sys/process.h"
#include "stdio.h"
#include "dev/leds.h"
#include "dev/dht11.h"

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
  static struct etimer timer;
  PROCESS_BEGIN();
  
  // Activate DHT11 sensor
  SENSORS_ACTIVATE(dht11_sensor);
  
  while(1) {
    etimer_set(&timer, CLOCK_SECOND * 5);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    
    int temperature = dht11_sensor.value(DHT11_SENSOR_TEMP);
    int humidity = dht11_sensor.value(DHT11_SENSOR_HUMIDITY);

    printf("Temperature: %d C, Humidity: %d \%\n", temperature, humidity);
    
    // Turn on the RGB LED
    leds_on(LEDS_GREEN);
    leds_on(LEDS_RED);
    leds_on(LEDS_BLUE);
    
    etimer_set(&timer, CLOCK_SECOND * 1);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    leds_off(LEDS_GREEN);
    leds_off(LEDS_RED);
    leds_off(LEDS_BLUE);
  }
  
  PROCESS_END();
}