#include "contiki-net.h"
#include "contiki-lib.h"
#include "contiki-conf.h"
#include "contiki-default-conf.h"
#include "dev/dht11.h"
#include "dev/rgb-leds.h"
#include "clock.h"

PROCESS(dht11_read_process, "DHT11 Read and RGB LED Control Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data)
{
    static struct etimer timer;
    static int temperature, humidity;
    
    PROCESS_BEGIN();

    // Configure the DHT11 sensor and RGB LED
    SENSORS_ACTIVATE(dht11_sensor);
    rgb_led_init();

    // Set the timer to fire every 5 seconds 
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read values from the DHT11 sensor
        temperature = dht11_sensor.value(DHT11_READ_TEMP);
        humidity = dht11_sensor.value(DHT11_READ_HUM);

        // Check if reading is successful
        if(temperature != -1 && humidity != -1){
            printf("Temperature: %d C, Humidity: %d %%\n", temperature, humidity);
            
            // Turn on the RGB LED based on the temperature
            if (temperature > 25) {
                rgb_led_on(RGB_LED_RED);
            } else {
                rgb_led_on(RGB_LED_GREEN);
            }
        } else {
            printf("Error reading DHT11 sensor\n");
            rgb_led_on(RGB_LED_BLUE); // Error indication
        }

        // Reset the timer
        etimer_reset(&timer);
    }

    PROCESS_END();
}