#include "contiki.h"
#include "contiki-net.h"
#include "contiki-conf.h"
#include "contiki-default-conf.h"

// RGB LED define
#define RGB_LED_RED   1
#define RGB_LED_GREEN 2
#define RGB_LED_BLUE  4
#define RGB_LED_WHITE (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)

// DHT11 defines
#define DHT11_VALUE_TEMPERATURE_INTEGER 0
#define DHT11_VALUE_HUMIDITY_INTEGER    1

PROCESS(dht11_process, "DHT11 Reading Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data) {
  static struct etimer timer;
  static int temperature = 0;
  static int humidity = 0;

  PROCESS_BEGIN();

  // Set the timer to 5 seconds
  etimer_set(&timer, CLOCK_SECOND * 5);

  while (1) {
    // Wait for timer event
    PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);

    // Read the DHT11 sensor values
    printf("Reading DHT11 sensor data...\n");
    // Simulated read from DHT11
    temperature = DHT11_VALUE_TEMPERATURE_INTEGER;  // Replace with actual sensor read function
    humidity = DHT11_VALUE_HUMIDITY_INTEGER;        // Replace with actual sensor read function

    // Perform action based on readings
    printf("Temperature: %d, Humidity: %d\n", temperature, humidity);

    // Turn on the RGB LED
    int rgb_led_color = RGB_LED_WHITE;
    printf("RGB LED turned on with color: %d\n", rgb_led_color);

    // Reset the timer
    etimer_reset(&timer);
  }

  PROCESS_END();
}
