#include "net/ipv6/udp-socket.h"
#include "dev/button-hal.h"
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "dht11-sensor.h"
#include "sys/etimer.h"
#include "dev/etc/rgb-led/rgb-led.h"
#include <string.h>

static struct etimer timer;

PROCESS(dht11_led_process, "DHT11 LED Process");
AUTOSTART_PROCESSES(&dht11_led_process);

PROCESS_THREAD(dht11_led_process, ev, data)
{
    PROCESS_BEGIN();

    // Configure DHT11 sensor
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, GPIO_HAL_PORT_A);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, GPIO_HAL_PIN_4);

    // Initialize RGB LED
    rgb_led_init();

    while(1) {
        // Read sensor every 5 seconds
        etimer_set(&timer, CLOCK_SECOND * 5);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Trigger a sensor read
        dht11_sensor.configure(SENSORS_ACTIVE, 1);
        int temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

        // Turn on LED based on certain condition
        if (temperature > 25) {  // Example condition
            rgb_led_set(255, 0, 0);  // Turn on LED (red color)
        }
        else {
            rgb_led_off();
        }
    }

    PROCESS_END();
}