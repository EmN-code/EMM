#include <stdlib.h>
#include <stdio.h>
#include "sensors/sensor.h"
#include "sensors/dht11_sensor.h"
#include "sensors/rgb_led.h"
#include "contiki.h"

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
  static struct etimer timer;
  PROCESS_BEGIN();

  // Initialize the DHT11 sensor
  SENSORS_ACTIVATE(dht11_sensor);

  // Initialize the RGB LED
  SENSORS_ACTIVATE(rgb_led);

  while(1) {
    // Set the timer to fire every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);
    
    // Wait for the timer event to occur
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    
    // Read temperature and humidity from DHT11 sensor
    int temperature = dht11_sensor.value(DHT11_SENSOR_TEMP);
    int humidity = dht11_sensor.value(DHT11_SENSOR_HUMIDITY);

    printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);

    // Turn on RGB LED
    rgb_led_set(RGB_LED_RED, 1);
    rgb_led_set(RGB_LED_GREEN, 0);
    rgb_led_set(RGB_LED_BLUE, 0);
  }
  
  PROCESS_END();
}