#include "contiki.h"
#include "dev/gpio-hal.h"
#include "dht11-sensor.h"

#define GPIO_PORT 0
#define DHT11_GPIO_PIN 2

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data) {
    static struct etimer timer;
    
    PROCESS_BEGIN();
    
    // Configure the DHT11 sensor's GPIO port and pin
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, GPIO_PORT);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);

    // Activate the sensor
    SENSORS_ACTIVATE(dht11_sensor);

    while(1) {
         // Set the timer to expire in 5 seconds
        etimer_set(&timer, CLOCK_SECOND * 5);
        
        // Wait for the timer to expire
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read the values from the DHT11 sensor
        if(dht11_sensor.status(SENSORS_READY)) {
            int temp_int = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
            int temp_dec = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL);
            int hum_int = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
            int hum_dec = dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL);
            
            printf("Temperature: %d.%d C, Humidity: %d.%d %%, Status: %d\n", temp_int, temp_dec, hum_int, hum_dec, dht11_sensor.status(SENSORS_READY));
        } else {
            printf("DHT11 sensor is not ready\n");
        }
    }
    
    SENSORS_DEACTIVATE(dht11_sensor);
    PROCESS_END();
}