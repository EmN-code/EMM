#include "sys/pt.h"
#include "sys/ctimer.h"
#include "sensors.h"
#include "dev/dht11-sensor.h"

#define RGB_LED_RED     (1<<0)
#define RGB_LED_GREEN   (1<<1)
#define RGB_LED_BLUE    (1<<2)

static struct ctimer dht_timer;

PROCESS(dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_process);

void activate_leds(uint8_t color) {
    // Simulate the turning on of RGB LEDs based on the color value
    // Hardware specific API calls would be needed to control real LEDs
    if (color & RGB_LED_RED) {
        printf("Turning on RED LED\n");
    }
    if (color & RGB_LED_GREEN) {
        printf("Turning on GREEN LED\n");
    }
    if (color & RGB_LED_BLUE) {
        printf("Turning on BLUE LED\n");
    }
}

static void read_dht11(void *ptr) {
    int temp = 0;
    int humidity = 0;
    
    SENSORS_ACTIVATE(dht11_sensor);
    
    temp = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
    humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

    printf("Temperature: %d C\n", temp);
    printf("Humidity: %d%%\n", humidity);

    activate_leds(RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE); // White color for indication

    SENSORS_DEACTIVATE(dht11_sensor);
    
    ctimer_reset(&dht_timer);
}

PROCESS_THREAD(dht11_process, ev, data) {
    PROCESS_BEGIN();
    
    ctimer_set(&dht_timer, 5 * CLOCK_SECOND, read_dht11, NULL);
    
    while (1) {
        PROCESS_YIELD();
    }
    
    PROCESS_END();
}