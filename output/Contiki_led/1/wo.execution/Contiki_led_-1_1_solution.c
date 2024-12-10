#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uip-ds6-route.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/ipv6/simple-udp.h"
#include "mqtt_prop_decode_input_props.h"
#include "mqtt_prop_print_input_props.h"
#include "etimer.h"
#include "dht11-sensor.h"

#include <stdio.h>

// Define LED pins (assume these definitions are correct for your board)
#define LED_PIN_RED   22
#define LED_PIN_GREEN 23
#define LED_PIN_BLUE  24

// Function prototypes
static void read_dht11_and_control_led(void);

// Main process
PROCESS(dht11_process, "DHT11 Sensor and LED Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    // Initialize hardware components
    dht11_sensor.configure(SENSORS_ACTIVE, DHT11_SENSOR);

    // Set LED pins as output
    nrf_gpio_cfg_output(LED_PIN_RED);
    nrf_gpio_cfg_output(LED_PIN_GREEN);
    nrf_gpio_cfg_output(LED_PIN_BLUE);

    while(1) {
        // Set the timer to fire every 5 seconds
        etimer_set(&timer, CLOCK_SECOND * 5);

        // Block until the timer expires
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        // Read sensor and control LED
        read_dht11_and_control_led();
    }

    PROCESS_END();
}

// Function to read DHT11 sensor and control the RGB LED
static void read_dht11_and_control_led(void) {
    int temp, hum;
    SENSORS_ACTIVATE(dht11_sensor);

    // Check if the sensor read was successful
    if (dht11_sensor.value(DHT11_SENSOR_TEMP) != -1) {
        temp = dht11_sensor.value(DHT11_SENSOR_TEMP) / 10;
        hum = dht11_sensor.value(DHT11_SENSOR_HUMIDITY) / 10;
        printf("Temperature: %02d C, Humidity: %02d %%\n", temp, hum);

        // Example control of the RGB LED based on temperature
        if (temp > 25) {
            nrf_gpio_pin_set(LED_PIN_RED);   // Turn on red
            nrf_gpio_pin_clear(LED_PIN_GREEN);
            nrf_gpio_pin_clear(LED_PIN_BLUE);
        } else {
            nrf_gpio_pin_set(LED_PIN_GREEN); // Turn on green
            nrf_gpio_pin_clear(LED_PIN_RED);
            nrf_gpio_pin_clear(LED_PIN_BLUE);
        }
    } else {
        printf("DHT11 sensor read error\n");
    }

    SENSORS_DEACTIVATE(dht11_sensor);
}