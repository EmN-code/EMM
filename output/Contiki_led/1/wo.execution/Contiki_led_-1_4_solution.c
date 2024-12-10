#include "net/netstack.h"
#include "net/routing/routing.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-udp-packet.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/tcpip.h"
#include "net/ipv6/tcp-socket.h"
#include "net/ipv6/node-id.h"
#include "net/ipv6/psock.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-sr.h"
#include "dev/button-hal.h"
#include "dev/etc/rgb-led/rgb-led.h"
#include "dht11-sensor.h"
#include "coap-blocking-api.h"
#include "coap-log.h"
#include "coap-engine.h"
#include "coap-observe-client.h"
#include "coap-request-state.h"
#include "mqtt-prop.h"
#include "mqtt.h"
#include "random.h"

#define RGB_CYAN 5  // RGB LED Cyan color identifier

PROCESS_THREAD(example_dht11_process, ev, data) {
    static struct etimer timer;
    
    PROCESS_BEGIN();
    
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        etimer_reset(&timer);
       
        SENSORS_ACTIVATE(dht11_sensor);
        int temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
        int humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
        SENSORS_DEACTIVATE(dht11_sensor);

        if (temperature >= 0) {
            printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);

            rgb_led_set(RGB_CYAN); // Turn on RGB LED in Cyan color
        } else {
            printf("Failed to read DHT11 sensor\n");
        }
        
        etimer_set(&timer, CLOCK_SECOND * 5);
    }

    PROCESS_END();
}

void rgb_led_set(int color) {
    // This function should control the RGB LED hardware to set it to the specified color
    // For annotation purposes, we'll assume it outputs to the terminal
    printf("RGB LED set to color %d\n", color);
}

int main(void) {
    printf("Starting DHT11 and RGB LED example\n");
    process_start(&example_dht11_process, NULL);
    while(1) {
        // Main loop can perform other tasks
    }
    return 0;
}