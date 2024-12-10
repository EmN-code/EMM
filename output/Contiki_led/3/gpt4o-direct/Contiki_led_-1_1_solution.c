#include "contiki.h"
#include "dev/leds.h"
#include "dev/dht11-sensor.h"

PROCESS(read_dht11_process, "DHT11 Sensor Reading Process");
AUTOSTART_PROCESSES(&read_dht11_process);

PROCESS_THREAD(read_dht11_process, ev, data)
{
  static struct etimer timer;
  static int temperature, humidity;

  PROCESS_BEGIN();

  // Initialize DHT11
  SENSORS_ACTIVATE(dht11_sensor);

  while(1) {
    // Set the timer for 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    // Wait until the timer event occurs
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read temperature and humidity
    temperature = dht11_sensor.value(DHT11_SENSOR_TEMP);
    humidity = dht11_sensor.value(DHT11_SENSOR_HUMIDITY);

    if(temperature != -1 && humidity != -1) {
      printf("Temperature: %d, Humidity: %d\n", temperature, humidity);

      // Turn on the RGB LED. Here, we just turn on all the LEDs.
      leds_on(LEDS_RED);
      leds_on(LEDS_GREEN);
      leds_on(LEDS_BLUE);
    }
    else {
      printf("Failed to read from DHT11 sensor\n");
    }
  }

  PROCESS_END();
}
