#include "contiki.h"
#include "dev/leds.h"
#include <stdio.h>

// Declare process for reading dht11 and controlling RGB LED
PROCESS(dht11_led_process, "DHT11 and LED Process");
AUTOSTART_PROCESSES(&dht11_led_process);

static struct etimer timer;

PROCESS_THREAD(dht11_led_process, ev, data) {
  PROCESS_BEGIN();

  // Initialize the timer for a 5-second interval
  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    // Wait for the timer event
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Simulate reading from DHT11 (In a real scenario, replace with actual sensor read code)
    printf("Reading DHT11 sensor\n");

    // Turn on the RGB LED (simulating as printf since exact library for RGB control on NRF52840 is not specified)
    leds_on(LEDS_GREEN);  // Assuming turning on green for the sake
    printf("Turning on RGB LED\n"); // Simulate LED control

    // Reset the timer to trigger again after 5 seconds
    etimer_reset(&timer);
  }

  PROCESS_END();
}