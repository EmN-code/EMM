#include "sensors.h"
#include "dht11.h"  // Assume this header provides the dht11_sensor external declaration
#include "contiki.h"
#include <stdio.h>

PROCESS(dht11_reader_process, "DHT11 Reader Process");
AUTOSTART_PROCESSES(&dht11_reader_process);

static struct etimer timer;

PROCESS_THREAD(dht11_reader_process, ev, data) {
  PROCESS_BEGIN();

  printf("Starting DHT11 reader process\n");

  while(1) {
    // Set the etimer for 5 seconds
    etimer_set(&timer, 5 * CLOCK_SECOND);

    // Wait for the etimer event (5 seconds)
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Read data from the DHT11 sensor
    int value = dht11_sensor.value(SENSORS_ACTIVE);
    if(value == -1) {
      printf("Failed to read from DHT11 sensor\n");
    } else {
      printf("DHT11 Sensor Value: %d\n", value);
    }
  }

  PROCESS_END();
}