#include "dev/leds.h"
#include "dev/dht11-sensor.h"
#include "contiki.h"

PROCESS(dht11_led_process, "DHT11 Sensor and LED control process");
AUTOSTART_PROCESSES(&dht11_led_process);

#define LED_PIN_RED    18  // Dummy GPIO pin for Red LED
#define LED_PIN_GREEN  19  // Dummy GPIO pin for Green LED
#define LED_PIN_BLUE   20  // Dummy GPIO pin for Blue LED

static struct etimer et;

PROCESS_THREAD(dht11_led_process, ev, data)
{
  PROCESS_BEGIN();

  // Initialize the sensor
  SENSORS_ACTIVATE(dht11_sensor);

  // Configure LED pins as outputs
  GPIO_SET_OUTPUT(GPIO_PORT_TO_BASE(GPIO_C_NUM), GPIO_PIN_MASK(LED_PIN_RED));
  GPIO_SET_OUTPUT(GPIO_PORT_TO_BASE(GPIO_C_NUM), GPIO_PIN_MASK(LED_PIN_GREEN));
  GPIO_SET_OUTPUT(GPIO_PORT_TO_BASE(GPIO_C_NUM), GPIO_PIN_MASK(LED_PIN_BLUE));

  while(1) {
    // Set timer to trigger every 5 seconds
    etimer_set(&et, CLOCK_SECOND * 5);

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    // Read the DHT11 sensor
    int temperature = dht11_sensor.value(DHT11_SENSOR_TEMP);
    int humidity = dht11_sensor.value(DHT11_SENSOR_HUMIDITY);

    // Print sensor values for debugging
    printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);

    // Turn on the RGB LED
    GPIO_WRITE_PIN(GPIO_PORT_TO_BASE(GPIO_C_NUM), GPIO_PIN_MASK(LED_PIN_RED));
    GPIO_WRITE_PIN(GPIO_PORT_TO_BASE(GPIO_C_NUM), GPIO_PIN_MASK(LED_PIN_GREEN));
    GPIO_WRITE_PIN(GPIO_PORT_TO_BASE(GPIO_C_NUM), GPIO_PIN_MASK(LED_PIN_BLUE));
  }

  PROCESS_END();
}