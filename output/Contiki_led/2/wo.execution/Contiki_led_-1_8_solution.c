#include "string.h"
#include "net/routing/routing.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip.h"
#include "net/mac/mac.h"
#include "random.h"
#include "mqtt.h"
#include "mqtt-prop.h"
#include "dev/etc/rgb-led/rgb-led.h"
#include "dev/button-hal.h"
#include "dev/gpio-hal.h"
#include "dht11-sensor.h"
#include "coap-request-state.h"
#include "coap-observe.h"
#include "coap-log.h"
#include "coap-engine.h"
#include "coap-blocking-api.h"
#include "sys/timer.h"
#include "sys/rtimer.h"
#include "sys/process.h"
#include "sys/pt.h"
#include "contiki-net.h"
#include "contiki-lib.h"
#include "contiki-default-conf.h"
#include "contiki-conf.h"
#include "contiki.h"

PROCESS(dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data)
{
    static struct etimer timer;
    PROCESS_BEGIN();

    // Initialize the GPIO pin for DHT11
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, GPIO_HAL_PORT_A);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, GPIO_HAL_PIN_4);

    while(1) {
        // Set the timer to trigger every 5 seconds
        etimer_set(&timer, CLOCK_SECOND * 5);

        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Activate the sensor and read the values
        SENSORS_ACTIVATE(dht11_sensor);

        int temp_int = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
        int temp_dec = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL);
        int hum_int = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
        int hum_dec = dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL);

        printf("Temperature: %d.%d C, Humidity: %d.%d%%\n", temp_int, temp_dec, hum_int, hum_dec);

        // Turn on the RGB LED. For instance, the function could be rgb_led_on();
        rgb_led_on();

        SENSORS_DEACTIVATE(dht11_sensor);
    }

    PROCESS_END();
}