#include <stdio.h>
#include "dev/rgb-led.h"
#include "dev/uart.h"
#include "dht11-sensor.h"
#include "contiki.h"
#include "contiki-net.h"

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
  static struct etimer timer;
  PROCESS_BEGIN();
  
  while(1) {
    etimer_set(&timer, CLOCK_SECOND * 5);
    
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    
    SENSORS_ACTIVATE(dht11_sensor);
    int temp = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
    int humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
    SENSORS_DEACTIVATE(dht11_sensor);

    printf("Temperature: %d C, Humidity: %d\n", temp, humidity);
    
    rgb_led_set(RGB_LED_MAGENTA);
  }
  
  PROCESS_END();
}