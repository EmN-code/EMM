#include <stdlib.h>
#include <sys/timer.h>
#include <sys/pt.h>
#include <sys/log.h>
#include <sys/ctimer.h>
#include <sys/etimer.h>
#include <sys/clock.h>
#include <sensors.h>
#include <net/routing/routing.h>
#include <net/netstack.h>
#include <net/mac/framer/framer.h>
#include <lib/random.h>
#include <dev/button-hal.h>
#include <dht11-sensor.h>
#include <coap-blocking-api.h>
#include <coap-log.h>
#include <coap-engine.h>
#include <coap-request-state.h>
#include <contiki-net.h>
#include <contiki-lib.h>
#include <contiki-default-conf.h>
#include <contiki-conf.h>
#include <contiki.h>

// Log configuration
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_APP

PROCESS(dht11_reader_process, "DHT11 Reader Process");
AUTOSTART_PROCESSES(&dht11_reader_process);

static struct etimer timer;

static void read_dht11_sensor() {
    int temperature = DHT11_VALUE_TEMPERATURE_INTEGER;  // Replace with proper function to get temperature
    int humidity = DHT11_VALUE_HUMIDITY_INTEGER;        // Replace with proper function to get humidity

    LOG_INFO("Temperature: %d, Humidity: %d\n", temperature, humidity);
}

PROCESS_THREAD(dht11_reader_process, ev, data) {
    PROCESS_BEGIN();

    // Set the GPIO port and pin
    SENSORS_ACTIVATE(dht11_sensor);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, 0);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, 2);

    while(1) {
        // Set the event timer
        etimer_set(&timer, CLOCK_SECOND * 5);

        // Wait for the timer to expire
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        read_dht11_sensor();
    }

    SENSORS_DEACTIVATE(dht11_sensor);

    PROCESS_END();
}