
#include "sys/rtimer.h"
#include "sys/etimer.h"
#include "sys/timer.h"
#include "sys/ctimer.h"
#include "sensors.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/tcpip.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/icmp6.h"
#include "net/ipv6/framer/framer.h"
#include "net/mac/framer/framer.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/tcpip.h"
#include "net/ipv6/psock.h"
#include "net/mac/framer/framer.h"
#include "dht11-sensor.h"
#include "contiki.h"
#include "contiki-net.h"
#include "contiki-lib.h"
#include "coap-timer.h"
#include "coap-request-state.h"

PROCESS(nrf52840_dht11_process, "NRF52840 DHT11 process");
AUTOSTART_PROCESSES(&nrf52840_dht11_process);

PROCESS_THREAD(nrf52840_dht11_process, ev, data)
{
    static struct etimer timer;
    static uint8_t humidity_integer;
    static uint8_t temperature_integer;

    PROCESS_BEGIN();

    // Initialize the DHT11 sensor
    if(SENSORS_ACTIVATE(dht11_sensor) != DHT11_STATUS_OKAY) {
        printf("Failed to activate DHT11 sensor\n");
        PROCESS_EXIT();
    }

    while(1) {
        // Set a timer for 5 seconds
        etimer_set(&timer, CLOCK_SECOND * 5);

        // Wait for an event
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);

        // Read humidity and temperature from the sensor
        humidity_integer = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
        temperature_integer = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

        printf("Humidity: %d\% Temperature: %d\n", humidity_integer, temperature_integer);

        // Implement LED activation logic here based on sensor readings
        // For example, the hypothetical command to turn on LED might be:
        // rgb_led_on();
    }

    // Deactivate the sensor (if needed)
    SENSORS_DEACTIVATE(dht11_sensor);

    PROCESS_END();
}