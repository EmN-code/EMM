#include "udp-socket.h"
#include "contiki-net.h"
#include "contiki.h"
#include "dht11-sensor.h"
#include "dev/etc/rgb-led/rgb-led.h"
#include <stdio.h>

PROCESS(read_dht11_process, "DHT11 Sensor Read Process");
AUTOSTART_PROCESSES(&read_dht11_process);

static struct etimer timer;

PROCESS_THREAD(read_dht11_process, ev, data)
{
    int temperature, humidity;

    PROCESS_BEGIN();
    
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        SENSORS_ACTIVATE(dht11_sensor);
        
        temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
        humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
        
        printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);

        if(dht11_sensor.status(0) == DHT11_STATUS_OKAY) {
            rgb_led_set(RGB_LED_RED);
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }

        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void) {
    process_start(&read_dht11_process, NULL);
    process_run();
    return 0;
}
