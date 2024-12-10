
#include "contiki.h"
#include "dev/gpio.h"
#include "dev/leds.h"
#include "sys/etimer.h"
#include "dht11.h" // Ensure this header is available

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN  1

#define RGB_LED_RED     (1 << 0)
#define RGB_LED_GREEN   (1 << 1)
#define RGB_LED_BLUE    (1 << 2)

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

static struct etimer timer;

void read_dht11() {
    uint8_t humidity_integer, humidity_decimal, temperature_integer, temperature_decimal;
    int status = dht11_sensor.value(&dht11_sensor, DHT11_VALUE_HUMIDITY_INTEGER, &humidity_integer);
    if (status == DHT11_STATUS_OKAY) {
        dht11_sensor.value(&dht11_sensor, DHT11_VALUE_HUMIDITY_DECIMAL, &humidity_decimal);
        dht11_sensor.value(&dht11_sensor, DHT11_VALUE_TEMPERATURE_INTEGER, &temperature_integer);
        dht11_sensor.value(&dht11_sensor, DHT11_VALUE_TEMPERATURE_DECIMAL, &temperature_decimal);
        printf("Humidity: %d.%d %%, Temperature: %d.%d C\n", humidity_integer, humidity_decimal, temperature_integer, temperature_decimal);
        leds_set(RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE); // Turn on RGB LED
    } else {
        printf("Failed to read DHT11 sensor\n");
    }
}

PROCESS_THREAD(dht11_read_process, ev, data) {
    PROCESS_BEGIN();

    // Initialize DHT11 sensor
    dht11_sensor.configure(&dht11_sensor, DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PORT);
    dht11_sensor.configure(&dht11_sensor, DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);

    etimer_set(&timer, CLOCK_SECOND * 5); // Set timer for 5 seconds

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
        read_dht11();
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main() {
    // Initialize Contiki
    contiki_init();

    // Start the process
    process_start(&dht11_read_process, NULL);

    // Enter the Contiki main loop
    while (1) {
        process_run();
    }

    return 0;
}
