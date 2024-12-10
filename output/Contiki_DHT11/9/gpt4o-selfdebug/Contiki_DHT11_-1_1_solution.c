#include <contiki.h>
#include <stdio.h>
#include <nrf.h>
#include <dev/gpio.h>
#include <dev/sensor/dht11-sensor.h>

/*---------------------------------------------------------------------------*/
PROCESS(dht11_read_process, "DHT11 Sensor Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(dht11_read_process, ev, data)
{
  static struct etimer timer;
  PROCESS_BEGIN();

  // Initialize the sensor
  SENSORS_ACTIVATE(dht11_sensor);

  // Set the timer to go off every 5 seconds
  etimer_set(&timer, CLOCK_SECOND * 5);

  while (1) {
    // Wait for the timer event
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read the sensor value
    int temperature = dht11_sensor.value(DHT11_SENSOR_TEMP);
    int humidity = dht11_sensor.value(DHT11_SENSOR_HUM);

    // Check if reading is valid
    if(temperature != -1 && humidity != -1) {
      printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);
    } else {
      printf("Failed to read from DHT11 sensor!\n");
    }

    // Reset the timer
    etimer_reset(&timer);
  }

  // Deactivate the sensor
  SENSORS_DEACTIVATE(dht11_sensor);

  PROCESS_END();
}