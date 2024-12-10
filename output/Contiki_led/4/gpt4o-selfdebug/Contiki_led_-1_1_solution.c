#include "etimer.h"
#include "dht11-sensor.h"
#include "contiki-default-conf.h"

PROCESS(dht11_led_process, "DHT11 Sensor and LED Process");
AUTOSTART_PROCESSES(&dht11_led_process);

static struct etimer et;

PROCESS_THREAD(dht11_led_process, ev, data) {
  static int temperature, humidity;
  
  PROCESS_BEGIN();
  
  // Initialize the DHT11 sensor
  SENSORS_ACTIVATE(dht11_sensor);

  // Initialize the event timer
  etimer_set(&et, CLOCK_SECOND * 5);

  while (1) {
    // Wait for the timer event
    PROCESS_WAIT_EVENT_UNTIL(ev = PROCESS_EVENT_TIMER);
    
    // Read temperature and humidity
    temperature = dht11_sensor.value(DHT11_SENSOR_TEMP);
    humidity = dht11_sensor.value(DHT11_SENSOR_HUMIDITY);

    printf("Temperature: %d °C, Humidity: %d %%\n", temperature, humidity);
    
    // Logic to turn on RGB LED
    // Assuming there are functions to control RGB LED such as rgb_on() or similar
    printf("Turning on RGB LED\n");
    
    // Reset the timer
    etimer_reset(&et);
  }

  PROCESS_END();
}