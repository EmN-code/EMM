#include "contiki-lib.h"
#include "contiki-net.h"
#include "contiki-default-conf.h"
#include "contiki.h"
#include "coap-observe-client.h"
#include "sys/etimer.h"
#include "net/mac/framer/framer.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6.h"
#include "contiki-conf.h"

#include "sensors.h"

#define GPIO_PORT 0
#define GPIO_PIN 2

PROCESS(dht11_process, "DHT11 Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data)
{
    static struct etimer timer;
    PROCESS_BEGIN();

    // Initial configuration, normally done in dht11_init()
    sensors_dht11.configure(DHT11_CONFIGURE_GPIO_PORT, GPIO_PORT);
    sensors_dht11.configure(DHT11_CONFIGURE_GPIO_PIN, GPIO_PIN);

    // Set a timer to trigger every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);

        // Read temperature and humidity data
        int temp_integer = sensors_dht11.value(DHT11_VALUE_TEMPERATURE_INTEGER);
        int temp_decimal = sensors_dht11.value(DHT11_VALUE_TEMPERATURE_DECIMAL);
        int hum_integer = sensors_dht11.value(DHT11_VALUE_HUMIDITY_INTEGER);
        int hum_decimal = sensors_dht11.value(DHT11_VALUE_HUMIDITY_DECIMAL);

        // Handle data depending on the sensor status
        if (temp_integer != -1 && hum_integer != -1) {
            printf("Temperature: %d.%d C, Humidity: %d.%d %%\n", temp_integer, temp_decimal, hum_integer, hum_decimal);
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}