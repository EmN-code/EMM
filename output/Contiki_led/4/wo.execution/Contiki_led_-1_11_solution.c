#include "sys/autostart.h"
#include "sys/clock.h"
#include "sys/ctimer.h"
#include "sys/log.h"
#include "sys/process.h"
#include "sys/pt.h"
#include "sys/rtimer.h"
#include "sys/timer.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/uip-udp-packet.h"
#include "dev/gpio-hal.h"
#include "dht11-sensor.h"
#include "contiki.h"

PROCESS_THREAD(example_process, ev, data) {
  static struct etimer timer;
  etimer_set(&timer, CLOCK_SECOND * 5);

  PROCESS_BEGIN();
  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    int temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
    int humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

    printf("Temperature: %d, Humidity: %d\n", temperature, humidity);

    rgb_led_on(); // Example function to turn on an RGB LED

    etimer_reset(&timer);
  }
  PROCESS_END();
}

int main(void) {
  process_start(&example_process, NULL);
  autostart_start(autostart_processes);
  return 0;
}