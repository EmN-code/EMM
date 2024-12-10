#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/udp-socket.h"
#include "mqtt.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/tcpip.h"
#include "sys/etimer.h"
#include "dev/etc/rgb-led/rgb-led.h"
#include "dev/button-hal.h"
#include "contiki.h"
#include "dht11-sensor.h"

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

static struct etimer timer;

PROCESS_THREAD(dht11_read_process, ev, data)
{
    PROCESS_BEGIN();

    // Initialize the sensor
    SENSORS_ACTIVATE(dht11_sensor);
    
    while(1) {
        // Set the timer to 5 seconds
        etimer_set(&timer, CLOCK_SECOND * 5);

        // Wait for the timer event
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read values from DHT11 sensor
        int temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
        int humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

        // Print them
        printf("Temperature: %d, Humidity: %d\n", temperature, humidity);

        // Turn on the RGB LED assuming a function rgb_led_on() to turn it on
        rgb_led_on();
    }

    PROCESS_END();
}
