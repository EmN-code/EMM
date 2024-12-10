#include <contiki.h>
#include <dev/dht11.h>
#include <sys/rtimer.h>
#include <sys/etimer.h>

PROCESS(read_dht11_and_control_led, "DHT11 and LED control process");
AUTOSTART_PROCESSES(&read_dht11_and_control_led);

PROCESS_THREAD(read_dht11_and_control_led, ev, data)
{
    static struct etimer timer;
    static int temperature, humidity;
    
    PROCESS_BEGIN();

    // Initialize the etimer module
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        
        // Read values from the DHT11 sensor
        if(dht11_read(&temperature, &humidity) == DHT11_SUCCESS) {
            printf("Temperature: %d, Humidity: %d\n", temperature, humidity);
            // Turn on the RGB LED based on readings
            // Assuming some function set_rgb_led_color()
            set_rgb_led_color(RGB_LED_CYAN);  // Turn LED to cyan
        } else {
            printf("Error reading DHT11 sensor\n");
        }
        
        // Reset the timer for the next read
        etimer_reset(&timer);
    }

    PROCESS_END();
}

void set_rgb_led_color(int color) {
    // Dummy function to simulate RGB LED functionality
    // Implement actual hardware interaction here
}