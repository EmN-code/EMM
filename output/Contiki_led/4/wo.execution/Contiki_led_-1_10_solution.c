#include "sys/process.h"
#include "dht11-sensor.h"
#include "dev/gpio-hal.h"
#include "dev/leds.h"
#include "contiki.h"

#define GPIO_HAL_PORT 0  // example port, please adjust as needed
#define GPIO_HAL_PIN 0   // example pin, please adjust as needed

PROCESS(dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_process);

void rgb_led_on(void) {
    // Assuming LEDs_RED is defined to control the RGB LED
    leds_set(LEDS_RED);
}

PROCESS_THREAD(dht11_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();
    
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, GPIO_HAL_PORT);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, GPIO_HAL_PIN);
    
    etimer_set(&timer, CLOCK_SECOND * 5);
    
    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        
        // Read the DHT11 sensor
        if (dht11_sensor.configure(SENSORS_ACTIVE, 1)) {
            int temp = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
            int humid = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
            printf("Temperature: %d, Humidity: %d\n", temp, humid);
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }

        // Control the RGB LED
        rgb_led_on();
        
        // Reset the timer to trigger again
        etimer_reset(&timer);
    }

    PROCESS_END();
}