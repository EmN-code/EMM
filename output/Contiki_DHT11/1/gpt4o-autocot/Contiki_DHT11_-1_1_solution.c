#include <contiki.h>
#include <dev/dht11-sensor.h>
#include <stdio.h>
#include <sys/etimer.h>

#define GPIO_PIN  2
#define READ_INTERVAL (CLOCK_SECOND * 5)

PROCESS(dht11_reader_process, "DHT11 Reader Process");
AUTOSTART_PROCESSES(&dht11_reader_process);

PROCESS_THREAD(dht11_reader_process, ev, data)
{
    static struct etimer timer;
    int temperature, humidity;

    PROCESS_BEGIN();

    etimer_set(&timer, READ_INTERVAL);

    while(1) {
        PROCESS_YIELD();
        
        if(etimer_expired(&timer)) {
            dht11_sensor.configure(SENSORS_ACTIVE, GPIO_PIN);
            temperature = dht11_sensor.value(DHT11_SENSOR_TEMP);
            humidity = dht11_sensor.value(DHT11_SENSOR_HUMIDITY);
            printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);
            etimer_reset(&timer);
        }
    }

    PROCESS_END();
}

int main(void)
{
    process_start(&dht11_reader_process, NULL);
    while(1) {
        // Main loop of Contiki; let the OS do its thing.
        process_run();
    }

    return 0;
}