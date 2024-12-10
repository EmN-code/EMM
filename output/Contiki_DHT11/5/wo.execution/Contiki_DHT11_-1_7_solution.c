#include "contiki.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/uip-sr.h"
#include "sys/autostart.h"
#include "sys/rtimer.h"
#include "sys/timer.h"
#include "sensors.h"
#include "random.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "coap-observe-client.h"
#include "contiki-default-conf.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-icmp6.h"
#include "coap-log.h"
#include "dht11-sensor.h"
#include "dev/etc/rgb-led/rgb-led.h"
#include "mqtt-prop.h"
#include "mqtt.h"
#include "dev/button-hal.h"
#include "sys/etimer.h"
#include "sys/log.h"
#include "sys/pt.h"

#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

static struct etimer timer;

PROCESS_THREAD(dht11_read_process, ev, data) {
    PROCESS_BEGIN();

    // Set GPIO port and pin
    SENSORS_ACTIVATE(dht11_sensor);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, 0);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, 2);

    while (1) {
        etimer_set(&timer, CLOCK_SECOND * 5);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        if (dht11_sensor.status(SENSORS_READY)) {
            dht11_sensor.configure(SENSORS_ACTIVE, 1);
            int temperature_integer = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
            int temperature_decimal = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL);
            int humidity_integer = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
            int humidity_decimal = dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL);

            LOG_INFO("Temperature: %d.%d C", temperature_integer, temperature_decimal);
            LOG_INFO("Humidity: %d.%d %%", humidity_integer, humidity_decimal);
        } else {
            LOG_WARN("DHT11 sensor not ready");
        }
    }

    PROCESS_END();
}