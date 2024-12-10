#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/simple-udp.h"
#include "contiki-net.h"
#include "contiki-default-conf.h"

extern const struct sensors_sensor dht11_sensor;

void activate_led(void) {
    // Placeholder function to activate the RGB LED.
    // Actual GPIO pin numbers and configuration will depend on your hardware setup.
    printf("RGB LED is turned ON\n");
}

PROCESS(dht11_led_process, "DHT11 and LED process");
AUTOSTART_PROCESSES(&dht11_led_process);

PROCESS_THREAD(dht11_led_process, ev, data)
{
    static struct etimer timer;
    PROCESS_BEGIN();

    // Set the timer to five seconds
    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        // Wait for the timer event
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);

        // Read the DHT11 sensor
        SENSORS_ACTIVATE(dht11_sensor);
        int value = dht11_sensor.value(0);  // Reads the temperature value
        printf("DHT11 sensor value: %d\n", value);
        SENSORS_DEACTIVATE(dht11_sensor);

        // Turn on RGB LED
        activate_led();

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}