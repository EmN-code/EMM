#include "net/routing/routing.h"
#include "sys/etimer.h"
#include "dht11-sensor.h"
#include "contiki.h"
#include "random.h"
#include "net/ipv6/uip-ds6.h"
#include "dev/etc/rgb-led/rgb-led.h"
#include "coap-engine.h"

#define DHT11_READ_INTERVAL (CLOCK_SECOND * 5)

static struct etimer read_timer;

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
    PROCESS_BEGIN();

    SENSORS_ACTIVATE(dht11_sensor);
    etimer_set(&read_timer, random_rand() % CLOCK_SECOND);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER && data == &read_timer);

        int status = dht11_sensor.configure(SENSORS_ACTIVE, 1);
        if (status == DHT11_STATUS_OKAY) {
            int temp_int = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
            int temp_dec = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL);
            int hum_int = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
            int hum_dec = dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL);

            printf("Temperature: %d.%d°C, Humidity: %d.%d%%\n", temp_int, temp_dec, hum_int, hum_dec);

            rgb_led_on();  // Make sure rgb_led_on() function is available and correctly linked
        } else {
            printf("Failed to read DHT11 sensor\n");
        }

        etimer_reset(&read_timer);
    }

    PROCESS_END();
}