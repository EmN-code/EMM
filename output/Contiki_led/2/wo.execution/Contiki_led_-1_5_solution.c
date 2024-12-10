#include "sys/pt.h"
#include "sys/ctimer.h"
#include "dht11-sensor.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/udp-socket.h"
#include "net/ipv6/simple-udp.h"
#include "contiki-net.h"
#include "contiki-default-conf.h"
#include "contiki.h"
#include "dev/etc/rgb-led/rgb-led.h"

#define YOUR_GPIO_PORT 0 // replace with actual GPIO port for your setup
#define YOUR_GPIO_PIN  1 // replace with actual GPIO pin for your setup

PROCESS(read_dht11_process, "DHT11 Sensor Reader Process");
AUTOSTART_PROCESSES(&read_dht11_process);

static struct ctimer dht11_timer;

static void read_dht11_callback(void *ptr) {
    // Configure DHT11 sensor GPIO 
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, YOUR_GPIO_PORT);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, YOUR_GPIO_PIN);

    // Read the sensor values
    dht11_sensor.configure(SENSORS_ACTIVE, 1);

    // Print the sensor values for debugging purposes
    int temp_int = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
    int temp_dec = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_DECIMAL);
    int hum_int = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
    int hum_dec = dht11_sensor.value(DHT11_VALUE_HUMIDITY_DECIMAL);

    printf("Temperature: %d.%d C, Humidity: %d.%d %%, Status: %d\n", temp_int, temp_dec, hum_int, hum_dec, dht11_sensor.status(SENSORS_READY));

    // If the temperature exceeds a certain threshold, turn on the RGB LED
    if (temp_int >= 25) { // just an example threshold
        rgb_led_set(1, 255, 0, 0); // Turn on the RGB LED with red color
    } else {
        rgb_led_set(1, 0, 0, 0); // Turn off the RGB LED
    }

    // Reset the timer
    ctimer_reset(&dht11_timer);
}

PROCESS_THREAD(read_dht11_process, ev, data) {
    PROCESS_BEGIN();

    // Initialize timer to read DHT11 sensor every 5 seconds
    ctimer_set(&dht11_timer, CLOCK_SECOND * 5, read_dht11_callback, NULL);

    PROCESS_END();
}