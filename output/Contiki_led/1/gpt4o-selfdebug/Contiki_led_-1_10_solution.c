#include "sensors.h"
#include "dev/dht11_sensor.h"

process_thread(dht11_process, ev, data) {
    static struct etimer timer;
    uint8_t humidity, temperature;

    PROCESS_BEGIN();

    while (1) {
        etimer_set(&timer, CLOCK_SECOND * 5);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        SENSORS_ACTIVATE(dht11_sensor);

        if (dht11_sensor.status == DHT11_STATUS_OKAY) {
            humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
            temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
            printf("Humidity: %d%, Temperature: %dC\n", humidity, temperature);
            rgb_led_set(RGB_LED_RED); // Assuming red LED is part of the RGB and indicates successful reading
        } else {
            printf("Failed to read from DHT11 sensor\n");
        }

        SENSORS_DEACTIVATE(dht11_sensor);
    }

    PROCESS_END();
}

PROCESS_THREAD(rgb_process, ev, data) {
    PROCESS_BEGIN();

    while (1) {
        // Logic to change RGB LED can be implemented here
        PROCESS_PAUSE(); // For future expansion
    }

    PROCESS_END();
}

AUTOSTART_PROCESSES(&dht11_process, &rgb_process);