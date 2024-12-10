#include "dht11-sensor.h"
#include "contiki.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-sr.h"
#include "coap-timer.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/uip-ds6.h"
#include "mqtt.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/tcpip.h"
#include "net/ipv6/tcp-socket.h"
#include "coap-request-state.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/routing/routing.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "contiki-net.h"
#include <stdio.h>
#include "coap-observe.h"
#include "net/ipv6/uip-udp-packet.h"
#include "contiki-default-conf.h"
#include "coap-observe-client.h"

#define RGB_LED_PIN 1  // Define the GPIO pin for the RGB LED

PROCESS(dht11_sensor_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_sensor_process);

static struct etimer timer;

PROCESS_THREAD(dht11_sensor_process, ev, data)
{
    static int humidity_integer, temperature_integer;
    PROCESS_BEGIN();

    // Configure the DHT11 sensor
    SENSORS_ACTIVATE(dht11_sensor);

    while(1) {
        // Set the timer for 5 seconds
        etimer_set(&timer, CLOCK_SECOND * 5);

        // Wait for the timer event
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the DHT11 sensor values
        humidity_integer = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
        temperature_integer = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

        if (humidity_integer != -1 && temperature_integer != -1) {
            // Print the sensor values
            printf("Humidity: %d%%, Temperature: %dC\n", humidity_integer, temperature_integer);
            
            // Turn on the RGB LED (simplified, assuming a function turn_on_rgb_led exists)
            // Here we could use GPIO operations depending on the platform to turn on the LED
            printf("Turning on the RGB LED\n");
            // Example pseudo function: gpio_set(RGB_LED_PIN, 1);
        } else {
            printf("Failed to read from the DHT11 sensor\n");
        }
    }

    PROCESS_END();
}

int main() {
    // Initialize the Contiki process
    process_init();
    process_start(&dht11_sensor_process, NULL);
    while (1) {
        process_run();
        etimer_request_poll();
    }
}
