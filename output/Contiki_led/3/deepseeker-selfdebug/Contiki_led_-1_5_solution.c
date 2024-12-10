
#include "contiki.h"
#include "dev/gpio.h"
#include "dev/leds.h"
#include <stdio.h>

// Define the GPIO pins for the DHT11 sensor and RGB LED
#define DHT11_PIN 2
#define RGB_LED_RED_PIN 22
#define RGB_LED_GREEN_PIN 23
#define RGB_LED_BLUE_PIN 24

// DHT11 sensor data structure
typedef struct {
  uint8_t humidity_integer;
  uint8_t humidity_decimal;
  uint8_t temperature_integer;
  uint8_t temperature_decimal;
  uint8_t checksum;
} dht11_data_t;

// Function to read DHT11 sensor data
static int dht11_read(dht11_data_t *data) {
  // Implement the DHT11 reading logic here
  // This is a placeholder for the actual implementation
  return 0; // Return 0 on success, non-zero on failure
}

// Function to initialize the GPIO pins
static void gpio_init(void) {
  gpio_set_input(DHT11_PIN);
  gpio_set_output(RGB_LED_RED_PIN);
  gpio_set_output(RGB_LED_GREEN_PIN);
  gpio_set_output(RGB_LED_BLUE_PIN);
}

// Function to control the RGB LED
static void rgb_led_control(uint8_t red, uint8_t green, uint8_t blue) {
  gpio_set(RGB_LED_RED_PIN, red);
  gpio_set(RGB_LED_GREEN_PIN, green);
  gpio_set(RGB_LED_BLUE_PIN, blue);
}

// Main process
PROCESS(dht11_rgb_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_process);

PROCESS_THREAD(dht11_rgb_process, ev, data) {
  static struct etimer timer;
  static dht11_data_t dht11_data;

  PROCESS_BEGIN();

  gpio_init();

  while(1) {
    etimer_set(&timer, CLOCK_SECOND * 5); // Set timer for 5 seconds

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    if(dht11_read(&dht11_data) == 0) {
      printf("Temperature: %d.%d C, Humidity: %d.%d %%\n",
             dht11_data.temperature_integer, dht11_data.temperature_decimal,
             dht11_data.humidity_integer, dht11_data.humidity_decimal);

      // Simple logic to control RGB LED based on temperature
      if(dht11_data.temperature_integer < 20) {
        rgb_led_control(0, 255, 0); // Green for cool temperature
      } else if(dht11_data.temperature_integer < 30) {
        rgb_led_control(255, 255, 0); // Yellow for moderate temperature
      } else {
        rgb_led_control(255, 0, 0); // Red for hot temperature
      }
    } else {
      printf("Failed to read DHT11 sensor\n");
    }
  }

  PROCESS_END();
}
