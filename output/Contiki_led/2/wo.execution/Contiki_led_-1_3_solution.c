#include "sys/pt.h"
#include "sys/rtimer.h"
#include "sys/process.h"
#include "sys/etimer.h"
#include "sys/clock.h"
#include "net/ipv6/uip-sr.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uiplib.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/tcpip.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "dev/etc/rgb-led/rgb-led.h"
#include "dev/button-hal.h"
#include "coap-request-state.h"
#include "coap-observe-client.h"
#include "coap-observe.h"
#include "coap-engine.h"
#include "coap-blocking-api.h"
#include "coap-log.h"
#include "net/netstack.h"
#include "net/routing/routing.h"
#include "mqtt.h"
#include "mqtt-prop.h"
#include "sys/timer.h"
#include "sys/ctimer.h"
#include "contiki-lib.h"
#include "contiki.h"
#include "contiki-default-conf.h"
#include "contiki-conf.h"
#include "contiki-net.h"
#include "dev/etc/rgb-led/rgb-led.h"
#include "dev/button-hal.h"

#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

static struct etimer timer;

PROCESS(sensor_process, "DHT11 and LED Process");
AUTOSTART_PROCESSES(&sensor_process);

PROCESS_THREAD(sensor_process, ev, data){
    PROCESS_BEGIN();

    // RGB LED Initialization
    LOG_INFO("Initializing RGB LED...\n");
    rgb_led_init();

    // Set a timer to trigger every 5 seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        
        // Simulate reading DHT11 data
        LOG_INFO("Reading DHT11 Data...\n");
        int humidity = 65;  // Placeholder for humidity data
        int temperature = 22; // Placeholder for temperature data
        LOG_INFO("Humidity: %d Temperature: %d\n", humidity, temperature);

        // Activate RGB LED upon reading the data
        rgb_led_set(RGB_LED_RED);
        LOG_INFO("RGB LED turned on.\n");

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}
