
#include "contiki.h"
#include "dev/dht11.h"  // Include the DHT11 sensor header file
#include "dev/leds.h"   // Include the LEDs header file for controlling the RGB LED

#include <stdio.h>

// Define the RGB LED colors
#define RGB_LED_RED    LED1
#define RGB_LED_GREEN  LED2
#define RGB_LED_BLUE   LED3

// Define the RGB LED color combinations
#define RGB_LED_WHITE   (RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_CYAN    (RGB_LED_GREEN | RGB_LED_BLUE)
#define RGB_LED_MAGENTA (RGB_LED_RED | RGB_LED_BLUE)

// Process to read DHT11 sensor and control RGB LED
PROCESS(dht11_rgb_led_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_led_process);

PROCESS_THREAD(dht11_rgb_led_process, ev, data)
{
  static struct etimer timer;
  static int temperature, humidity;

  PROCESS_BEGIN();

  // Initialize the DHT11 sensor
  dht11_init(GPIO_PIN(0, 15));  // Replace GPIO_PIN(0, 15) with the actual pin connected to the DHT11 sensor

  // Set the timer to trigger every 5 seconds
  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read the DHT11 sensor
    if(dht11_read(&temperature, &humidity) == 0) {
      printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);

      // Turn on the RGB LED based on the temperature
      if(temperature < 20) {
        leds_set(RGB_LED_CYAN);
      } else if(temperature >= 20 && temperature < 30) {
        leds_set(RGB_LED_GREEN);
      } else {
        leds_set(RGB_LED_RED);
      }
    } else {
      printf("Failed to read DHT11 sensor\n");
    }

    // Reset the timer
    etimer_reset(&timer);
  }

  PROCESS_END();
}
