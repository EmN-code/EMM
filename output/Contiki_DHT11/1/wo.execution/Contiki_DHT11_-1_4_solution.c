#include "mqtt.h"
#include "random.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "dev/etc/rgb-led/rgb-led.h"
#include "dev/button-hal.h"
#include "dht11-sensor.h"
#include "coap-engine.h"
#include "coap-blocking-api.h"
#include "coap-log.h"
#include "sys/rtimer.h"
#include "sys/timer.h"
#include "sys/etimer.h"
#include "sys/ctimer.h"
#include "net/netstack.h"
#include "net/routing/routing.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uiplib.h"
#include "net/mac/framer/framer.h"
#include "contiki-net.h"
#include "contiki-lib.h"
#include "contiki-default-conf.h"
#include "contiki-conf.h"
#include "contiki.h"

PROCESS(dht11_process, "DHT11 process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data) {
    static struct etimer timer;
    static int temperature, humidity;

    PROCESS_BEGIN();

    /* Configure GPIO for DHT11 */
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, 0);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, 2);

    /* Set a timer for 5 seconds */
    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        /* Activate the sensor to read data */
        SENSORS_ACTIVATE(dht11_sensor);

        /* Read temperature and humidity */
        temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
        humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

        /* Print out the values */
        printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);

        /* Deactivate the sensor after reading */
        SENSORS_DEACTIVATE(dht11_sensor);

        /* Reset the timer */
        etimer_reset(&timer);
    }

    PROCESS_END();
}