#include "contiki.h"
#include "sys/etimer.h"
#include "dev/gpio.h"
#include "dev/dht11.h"

PROCESS(read_dht11_process, "DHT11 Reader Process");
AUTOSTART_PROCESSES(&read_dht11_process);

PROCESS_THREAD(read_dht11_process, ev, data)
{
  static struct etimer timer;
  static int temperature, humidity;

  PROCESS_BEGIN();

  // Initialize the GPIO pin for DHT11
  gpio_set_output(GPIO_PORT_NUM, 2);
  dht11_init(GPIO_PORT_NUM, 2);

  // Set the timer to trigger every 5 seconds
  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);

    // Read values from DHT11
    if(dht11_read(&temperature, &humidity) == 0) {
      printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
    } else {
      printf("Failed to read from DHT11 sensor\n");
    }

    // Reset the timer
    etimer_reset(&timer);
  }

  PROCESS_END();
}