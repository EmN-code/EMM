#include "coap-blocking-api.h"
#include "contiki-net.h"
#include "contiki.h"
#include "dev/button-hal.h"
#include "dev/gpio-hal.h"
#include "dht11-sensor.h"
#include "net/linkaddr.h"
#include "net/netstack.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-icmp6.h"
#include "sys/etimer.h"
#include "udp-socket.h"

// Define the process to read from DHT11 sensor
PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data) {
    static struct etimer timer;
    static int temperature;
    static int humidity;

    PROCESS_BEGIN();

    // Configure DHT11 GPIO Port and Pin
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, 0);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, 2);

    while (1) {
        etimer_set(&timer, CLOCK_SECOND * 5);

        // Activate sensor and read values
        dht11_sensor.configure(SENSORS_ACTIVE, 1);
        temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
        humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

        // Print the values
        printf("Temperature: %d°C\n", temperature);
        printf("Humidity: %d%%\n", humidity);

        // Wait for the timer event
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    }

    PROCESS_END();
}