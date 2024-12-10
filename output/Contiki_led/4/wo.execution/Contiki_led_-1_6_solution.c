#include "sys/etimer.h"
#include "contiki.h"
#include "dht11-sensor.h"
#include "dev/etc/rgb-led/rgb-led.h"
#include "dev/gpio-hal.h"

PROCESS(dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data)
{
  static struct etimer timer;
  PROCESS_BEGIN();

  // Configure the GPIO port and pin for the DHT11 sensor
  SENSORS_ACTIVATE(dht11_sensor);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, GPIO_HAL_PORT_A);
  dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, GPIO_HAL_PIN_0);

  while(1) {
    etimer_set(&timer, CLOCK_SECOND * 5);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read sensor data
    dht11_sensor.configure(SENSORS_ACTIVE, 1);
    int temp = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
    int humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

    printf("Temperature: %d C, Humidity: %d%%\n", temp, humidity);

    // Turn on the RGB LED
    rgb_led_on(RGB_LED_GREEN);
  }

  PROCESS_END();
}