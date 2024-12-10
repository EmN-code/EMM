#include "sensors.h"
#include "pt.h"
#include "process.h"
#include "etimer.h"
#include "core/net/netstack.h"
#include "dht11_sensor.h"

PROCESS(dht11_process, "DHT11 Process");
AUTOSTART_PROCESSES(&dht11_process);

static struct etimer timer;

PROCESS_THREAD(dht11_process, ev, data)
{
    static int temp_integer, temp_decimal, hum_integer, hum_decimal;

    PROCESS_BEGIN();

    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);

        if (dht11_sensor.status == DHT11_STATUS_OKAY) {
            temp_integer = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
            temp_decimal = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL);
            hum_integer = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
            hum_decimal = dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL);

            // RGB LED logic
            if(temp_integer > 25) {
                rgb_led_set(RGB_LED_RED);
            } else if(hum_integer > 50) {
                rgb_led_set(RGB_LED_BLUE);
            } else {
                rgb_led_set(RGB_LED_GREEN);
            }
        }

        etimer_reset(&timer);
    }

    PROCESS_END();
}
