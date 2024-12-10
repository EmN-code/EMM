#include "sys/etimer.h"
#include "sys/rtimer.h"
#include "sys/timer.h"
#include "sensors.h"
#include "dht11-sensor.h"
#include "contiki.h"
#define READ_INTERVAL (5 * CLOCK_SECOND)
#define DHT11_PIN (2)
PROCESS(dht11_reader_process, "DHT11 Reader Process");
AUTOSTART_PROCESSES(&dht11_reader_process);
PROCESS_THREAD(dht11_reader_process, ev, data) {
    static struct etimer timer;
    PROCESS_BEGIN();
    SENSORS_ACTIVATE(dht11_sensor);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_PIN);
    while (1) {
        etimer_set(&timer, READ_INTERVAL);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        int status = dht11_sensor.value(DHT11_STATUS_OKAY);
        if (status == DHT11_STATUS_OKAY) {
            int humidity_int = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
            int humidity_dec = dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL);
            int temp_int = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
            int temp_dec = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL);
            printf("Temperature: %d.%d C, Humidity: %d.%d %%\n", temp_int, temp_dec, humidity_int, humidity_dec);
        } else {
            printf("Failed to read from DHT11 sensor, status: %d\n", status);
        }
    }
    PROCESS_END();
}