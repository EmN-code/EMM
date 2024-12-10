
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "dev/dht11-sensor.h"
#include "dev/leds.h"
#include <stdio.h>

// Define GPIO pins for DHT11 and RGB LED
#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN  1
#define RGB_LED_RED     2
#define RGB_LED_GREEN   3
#define RGB_LED_BLUE    4

// Define RGB LED colors
#define RGB_LED_OFF     0
#define RGB_LED_RED_ON  (1 << RGB_LED_RED)
#define RGB_LED_GREEN_ON (1 << RGB_LED_GREEN)
#define RGB_LED_BLUE_ON (1 << RGB_LED_BLUE)

// Function to read DHT11 sensor
static int read_dht11(void) {
  int status = dht11_sensor.status;
  if (status == DHT11_STATUS_OKAY) {
    int humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
    int temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
    printf("Humidity: %d %%, Temperature: %d C\n", humidity, temperature);
    return 1;
  } else {
    printf("DHT11 read failed: %d\n", status);
    return 0;
  }
}

// Function to control RGB LED
static void control_rgb_led(int status) {
  if (status) {
    leds_set(RGB_LED_RED_ON | RGB_LED_GREEN_ON | RGB_LED_BLUE_ON);
  } else {
    leds_set(RGB_LED_OFF);
  }
}

// Process for reading DHT11 and controlling RGB LED
PROCESS(dht11_rgb_process, "DHT11 and RGB LED Process");
AUTOSTART_PROCESSES(&dht11_rgb_process);

PROCESS_THREAD(dht11_rgb_process, ev, data) {
  static struct etimer timer;

  PROCESS_BEGIN();

  // Initialize GPIO for DHT11 and RGB LED
  gpio_hal_arch_port_pin_cfg_output(DHT11_GPIO_PORT, DHT11_GPIO_PIN);
  gpio_hal_arch_port_pin_cfg_output(DHT11_GPIO_PORT, RGB_LED_RED);
  gpio_hal_arch_port_pin_cfg_output(DHT11_GPIO_PORT, RGB_LED_GREEN);
  gpio_hal_arch_port_pin_cfg_output(DHT11_GPIO_PORT, RGB_LED_BLUE);

  // Set initial state of RGB LED
  leds_set(RGB_LED_OFF);

  while (1) {
    // Set timer for 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    // Wait for timer to expire
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read DHT11 sensor
    int status = read_dht11();

    // Control RGB LED based on sensor status
    control_rgb_led(status);
  }

  PROCESS_END();
}

int main(void) {
  // Initialize Contiki
  contiki_init();

  // Start the process
  process_start(&dht11_rgb_process, NULL);

  return 0;
}
