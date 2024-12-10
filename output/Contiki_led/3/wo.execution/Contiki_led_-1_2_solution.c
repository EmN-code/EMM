#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/tcpip.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uip-udp-packet.h"
#include "sys/ctimer.h"
#include "sys/etimer.h"
#include "sys/rtimer.h"
#include "sys/timer.h"
#include "contiki-default-conf.h"
#include "contiki-lib.h"
#include "contiki-net.h"
#include "contiki.h"
#include "contiki-conf.h"
#include "dht11-sensor.h"
#include "dev/etc/rgb-led/rgb-led.h"

#include <stdio.h>

PROCESS(sensor_process, "DHT11 and LED control process");
AUTOSTART_PROCESSES(&sensor_process);

static struct etimer timer;

PROCESS_THREAD(sensor_process, ev, data)
{
    PROCESS_BEGIN();

    // Configure the DHT11 sensor
    SENSORS_ACTIVATE(dht11_sensor);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, /* your GPIO port */);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, /* your GPIO pin */);

    while (1) {
        // Set the timer
        etimer_set(&timer, CLOCK_SECOND * 5);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor
        int temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
        int humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

        if (dht11_sensor.status(SENSORS_READY) == DHT11_STATUS_OKAY) {
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);

            // Turn on the RGB LED
            rgb_led_on();
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }
    }

    PROCESS_END();
}