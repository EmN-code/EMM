#include "nrf52840.h"
#include "contiki.h"
#include "dev/leds.h"
extern const struct sensors_sensor dht11_sensor;

PROCESS(dht11_led_process, "DHT11 and LED Control Process");
AUTOSTART_PROCESSES(&dht11_led_process);

#define DHT11_READ_INTERVAL CLOCK_SECOND * 5

PROCESS_THREAD(dht11_led_process, ev, data){
  static struct etimer timer;
  static int temperature, humidity;

  PROCESS_BEGIN();

  while(1) {
    // Start the timer (5 seconds)
    etimer_set(&timer, DHT11_READ_INTERVAL);

    // Wait for an event: timer, sensor reading, etc.
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read from the DHT11 sensor
    SENSORS_ACTIVATE(dht11_sensor);
    temperature = dht11_sensor.value(DHT11_SENSOR_TYPE_TEMP);
    humidity = dht11_sensor.value(DHT11_SENSOR_TYPE_HUM);
    SENSORS_DEACTIVATE(dht11_sensor);

    // Check if the readings are successful
    if(temperature != -1 && humidity != -1) {
      // Turn on the RGB LED
      leds_on(LEDS_RED);
      leds_on(LEDS_GREEN);
      leds_on(LEDS_BLUE);
    } else {
      // Optionally handle the error, for instance, flash the red LED
      leds_on(LEDS_RED);
    }

    // For demo purposes, turn off the LED after a short delay
    etimer_set(&timer, CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    leds_off(LEDS_ALL);
  }

  PROCESS_END();
}