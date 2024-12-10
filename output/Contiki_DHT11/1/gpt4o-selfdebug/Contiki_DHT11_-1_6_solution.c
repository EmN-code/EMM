#include "contiki.h"
#include "sys/etimer.h"
#include "dev/gpio-hal.h"
#include "dev/gpio.h"

PROCESS(read_dht11_process, "DHT11 Reader Process");
AUTOSTART_PROCESSES(&read_dht11_process);

PROCESS_THREAD(read_dht11_process, ev, data)
{
  static struct etimer timer;
  static int value;
  
  PROCESS_BEGIN();

  // Initialize the timer to fire every 5 seconds
  etimer_set(&timer, CLOCK_SECOND * 5);

  // Setup the GPIO pin for the DHT11
  gpio_hal_arch_pin_set_input(2);
  
  while (1) {
    // Wait for the timer event
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    // Simulate reading the DHT11 sensor
    // In actual implementation, you would invoke the DHT11 library functions here
    value = (int)(gpio_hal_arch_pin_read(2));
    printf("DHT11 reading: %d\n", value);

    // Reset the timer
    etimer_reset(&timer);
  }

  PROCESS_END();
}
