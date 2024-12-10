#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip-udp-packet.h"
#include "random.h"
#include "sys/autostart.h"
#include "sys/rtimer.h"
#include "contiki.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/tcp-socket.h"
#include "sys/timer.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uip-icmp6.h"
#include "mqtt-prop.h"
#include "dev/etc/rgb-led/rgb-led.h"
#include "contiki.h"
#include "mqtt.h"
#include "dev/button-hal.h"
#include "contiki-conf.h"
#include "sys/pt.h"
#include "sys/clock.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/uiplib.h"
#include "sensors.h"
#include "net/ipv6/uip.h"
#include "contiki-net.h"
#include "contiki-lib.h"
#include "coap-observe-client.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "coap-log.h"
#include "contiki-default-conf.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/routing/routing.h"
#include "dht11-sensor.h"
#include "net/ipv6/uip-ds6.h"
#include "coap-blocking-api.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/mac/framer/framer.h"
#include "sys/log.h"
#include "sys/etimer.h"

#define LOG_MODULE "DHT11 Example"
#define LOG_LEVEL LOG_LEVEL_INFO

static struct etimer timer;

PROCESS(dht11_example_process, "DHT11 Example Process");
AUTOSTART_PROCESSES(&dht11_example_process);

// DHT11 sensor reading process
PROCESS_THREAD(dht11_example_process, ev, data)
{
    PROCESS_BEGIN();

    // Configure and activate DHT11 sensor
    sensors_dht11.configure(DHT11_CONFIGURE_GPIO_PORT, GPIO_PORT(2));
    sensors_dht11.configure(DHT11_CONFIGURE_GPIO_PIN, 2);
    SENSORS_ACTIVATE(sensors_dht11);

    while(1) {
        etimer_set(&timer, CLOCK_SECOND * 5);

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        if (sensors_dht11.status(0) == DHT11_STATUS_OKAY) {
            int temp_int = sensors_dht11.value(DHT11_VALUE_TEMPERATURE_INTEGER);
            int temp_dec = sensors_dht11.value(DHT11_VALUE_TEMPERATURE_DECIMAL);
            int hum_int = sensors_dht11.value(DHT11_VALUE_HUMIDITY_INTEGER);
            int hum_dec = sensors_dht11.value(DHT11_VALUE_HUMIDITY_DECIMAL);

            LOG_INFO("Temperature: %d.%d C", temp_int, temp_dec);
            LOG_INFO("Humidity: %d.%d %%", hum_int, hum_dec);
        } else {
            LOG_WARN("Failed to read from sensor");
        }
    }

    PROCESS_END();
}

int main(void) {
    // Start Contiki
    process_start(&dht11_example_process, NULL);
    // Enter Contiki-main loop
    while (1) {
        process_run();
        etimer_request_poll();
    }
}