#include "net/ipv6/udp-socket.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/psock.h"
#include "coap-log.h"
#include "dht11-sensor.h"
#include "coap-blocking-api.h"
#include "dev/button-hal.h"
#include "net/ipv6/uiplib.h"
#include "net/routing/routing.h"
#include "mqtt-prop.h"
#include "net/netstack.h"
#include "coap-engine.h"
#include "rgb-led.h"
#include "sys/etimer.h"
#include "mqtt.h"
#include "contiki.h"

#define DHT11_READ_INTERVAL (CLOCK_SECOND * 5)

static struct etimer read_timer;

PROCESS(dht11_read_process, "DHT11 Sensor Read and LED Control Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
    PROCESS_BEGIN();
    etimer_set(&read_timer, DHT11_READ_INTERVAL);
    
    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&read_timer));
        
        SENSORS_ACTIVATE(dht11_sensor);

        int temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
        int humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

        if (temperature == -1 || humidity == -1) {
            printf("Failed to read from DHT11 sensor\n");
        } else {
            printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
        }

        rgb_led_set(RGB_CYAN);
        
        SENSORS_DEACTIVATE(dht11_sensor);
        
        etimer_reset(&read_timer);
    }

    PROCESS_END();
}
