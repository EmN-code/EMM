#include "udp-socket.h"
#include "contiki-lib.h"
#include "coap-request-state.h"
#include "coap-engine.h"
#include "sys/pt.h"
#include "sys/ctimer.h"
#include "net/ipv6/uip-ds6-route.h"
#include "dev/etc/rgb-led/rgb-led.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/simple-udp.h"
#include "contiki.h"
#include "sys/clock.h"
#include "contiki-net.h"
#include "dht11-sensor.h"

PROCESS(read_dht11_and_control_rgb_led_process, "Read DHT11 and control RGB LED");
AUTOSTART_PROCESSES(&read_dht11_and_control_rgb_led_process);

PROCESS_THREAD(read_dht11_and_control_rgb_led_process, ev, data)
{
    static struct etimer timer;
    PROCESS_BEGIN();

    // Configure the DHT11 sensor
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, YOUR_GPIO_PORT);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, YOUR_GPIO_PIN);

    while(1) {
        // Set the timer for 5 seconds
        etimer_set(&timer, CLOCK_SECOND * 5);

        // Read sensor data
        dht11_sensor.configure(SENSORS_ACTIVE, 1);

        // Check if the sensor status is okay
        if(dht11_sensor.status(0) == DHT11_STATUS_OKAY) {
            int temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
            int humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);

            // Turn on the RGB LED
            rgb_led_set(0xFF, 0xFF, 0xFF); // Assuming this function exists
        }
        
        // Wait for the timer to expire
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    }

    PROCESS_END();
}