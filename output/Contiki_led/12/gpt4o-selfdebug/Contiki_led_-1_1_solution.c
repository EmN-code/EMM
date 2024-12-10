#include "sensors.h"
#include "dev/rgb_led.h"
#include "dev/dht11.h"
#include "sys/etimer.h"
#include "contiki.h"

PROCESS(dht11_read_process, "DHT11 Read and LED Control Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
  static struct etimer timer;
  static int temperature;
  static int humidity;

  PROCESS_BEGIN();

  etimer_set(&timer, CLOCK_SECOND * 5);

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    if (dht11_read(&temperature, &humidity) == DHT11_SUCCESS) {
      printf("Temperature: %d C, Humidity: %d%%\n", temperature, humidity);
      rgb_led_on(RED);
    } else {
      printf("Failed to read from DHT11 sensor\n");
      rgb_led_on(BLUE);
    }

    etimer_reset(&timer);
  }

  PROCESS_END();
}