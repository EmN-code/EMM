
#include "contiki.h"
#include "sys/rtimer.h"
#include "dev/gpio-hal.h"
#include "contiki-default-conf.h"
#include "dht11-sensor.h"
#include "net/routing/routing.h"
#include "coap-engine.h"
#include "net/ipv6/uip-nameserver.h"
#include "tcp-socket.h"
#include "mqtt-prop.h"
#include "mqtt.h"
#include "contiki.h"
#include "dev/leds.h"
#include "sys/ctimer.h"
#include "sys/rtimer.h"
#include "random.h"
#include "net/ipv6/simple-udp.h"
#include "dev/etc/rgb-led/rgb-led.h"
#include "net/ipv6/uip-udp-packet.h"
#include "lib/random.h"
#include "net/netstack.h"
#include "sys/etimer.h"
#include "net/ipv6/uiplib.h"
#include "net/mac/framer/framer.h"
#include "sys/autostart.h"
#include "net/ipv6/psock.h"
#include "coap-timer.h"
#include "dev/leds.h"
#include "lib/random.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "sys/pt.h"
#include "dev/gpio-hal.h"
#include "udp-socket.h"
#include "sys/etimer.h"
#include "net/ipv6/uip.h"
#include "sys/process.h"
#include "net/ipv6/uip-ds6.h"
#include "dev/radio.h"
#include "net/ipv6/uip-icmp6.h"
#include "coap-log.h"
#include "net/routing/routing.h"
#include "contiki-conf.h"
#include "sys/clock.h"
#include "dev/button-hal.h"
#include "net/ipv6/tcpip.h"
#include "net/ipv6/tcp-socket.h"
#include "contiki-lib.h"
#include "coap-transactions.h"
#include "sys/timer.h"
#include "net/ipv6/uip-ds6-route.h"
#include "coap-blocking-api.h"

#define INTERVAL_SECONDS 5

PROCESS(dht11_reader_process, "DHT11 Reader Process");
AUTOSTART_PROCESSES(&dht11_reader_process);

PROCESS_THREAD(dht11_reader_process, ev, data)
{
    static struct etimer et;
    static uint8_t humidity_integer, temperature_integer;
    static uint8_t humidity_decimal, temperature_decimal;
    static uint8_t checksum;

    PROCESS_BEGIN();

    etimer_set(&et, CLOCK_SECOND * INTERVAL_SECONDS);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        // Read DHT11 sensor values
        humidity_integer = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
        humidity_decimal = dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL);
        temperature_integer = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
        temperature_decimal = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL);
        checksum = dht11_sensor.value(DHT11_VALUE_CHECKSUM);

        // Check if the checksum is valid
        if((humidity_integer + humidity_decimal + temperature_integer + temperature_decimal) != checksum) {
            // Handle checksum error
            continue;
        }

        // Turn on the RGB LED
        rgb_led_set(RGB_LED_COLOR_RED);

        // Reset the timer
        etimer_reset(&et);
    }

    PROCESS_END();
}

int main(void)
{
    process_start(&dht11_reader_process, NULL);
    return 0;
}
