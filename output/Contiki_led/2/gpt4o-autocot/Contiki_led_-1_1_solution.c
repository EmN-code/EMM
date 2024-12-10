#include "sensors.h"
#include "contiki-default-conf.h"
#include <stdio.h>

// Function prototypes
extern const struct sensors_sensor dht11_sensor;
void read_dht11_sensor(void);
void turn_on_rgb_led(void);

PROCESS(dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_process);

// Function to read the DHT11 sensor
void read_dht11_sensor(void) {
  int temperature = SENSORS_ACTIVATE(dht11_sensor);
  // Usually temperature and humidity are returned and can be accessed via sensor.value()
  printf("Temperature: %d\n", temperature);
}

// Mock function to simulate turning on the RGB LED
void turn_on_rgb_led(void) {
  printf("RGB LED turned on\n");
}

PROCESS_THREAD(dht11_process, ev, data) {
  static struct etimer timer;
  PROCESS_BEGIN();

  // Set the timer to the desired interval, e.g., 5 seconds
  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read the sensor values
    read_dht11_sensor();

    // Turn on the RGB LED
    turn_on_rgb_led();

    // Reset the timer
    etimer_reset(&timer);
  }

  PROCESS_END();
}

int main(void) {
  // Initialize Contiki and the process manager
  process_init();

  // Start the Contiki processes
  process_start(&dht11_process, NULL);
  
  // Main loop to run processes
  while(1) {
    int r;
    do {
      r = process_run();
    } while(r > 0);
  }

  return 0;
}