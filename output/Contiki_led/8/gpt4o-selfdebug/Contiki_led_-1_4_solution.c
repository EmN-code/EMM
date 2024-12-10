#include <sys/rtimer.h>
#include <sys/etimer.h>
#include <sys/ctimer.h>
#include <sys/timer.h>
#include <nrf_gpio.h>
#include <net/netstack.h>
#include <dev/leds.h>

#define DHT11_PIN 2 // Define the pin connected to DHT11

PROCESS(dht11_process, "DHT11 Reader");
PROCESS(rgb_led_process, "RGB LED Controller");
AUTOSTART_PROCESSES(&dht11_process, &rgb_led_process);

static struct etimer et;

PROCESS_THREAD(dht11_process, ev, data)
{
    PROCESS_BEGIN();
    
    while(1) {
        etimer_set(&et, CLOCK_SECOND * 5);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        // Code to read DHT11 sensor
        // dht11_read(DHT11_PIN);

        // Use the sensor data to control the LED if needed

        printf("Read DHT11
");
    }
    
    PROCESS_END();
}

PROCESS_THREAD(rgb_led_process, ev, data)
{
    PROCESS_BEGIN();

    while(1) {
        // Assuming the DHT11 has been read and based on that, control LED
        etimer_set(&et, CLOCK_SECOND); // Delay for LED state change
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        // Toggle LED color/state
        leds_toggle(RGB_LED_RED);
        
        printf("Toggling RGB LED
");
    }
    
    PROCESS_END();
}