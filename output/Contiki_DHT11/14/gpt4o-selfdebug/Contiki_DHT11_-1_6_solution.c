#include "contiki.h"
#include "contiki-default-conf.h"
#include "contiki-conf.h"
#include "sys/etimer.h"

PROCESS(dht11_sensor_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_sensor_process);

PROCESS_THREAD(dht11_sensor_process, ev, data)
{
  static struct etimer timer;
  
  PROCESS_BEGIN();

  // Initialize timer to fire every 5 seconds
  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    // Wait for the timer event to occur
    PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);

    // Simulate reading temperature and humidity from the DHT11 sensor connected to GPIO 2
    int temperature = 25; // Suppose we've read this value
    int humidity = 60;    // Suppose we've read this value

    printf("Temperature: %dC, Humidity: %d%%\n", temperature, humidity);

    // Reset the timer
    etimer_reset(&timer);
  }

  PROCESS_END();
}