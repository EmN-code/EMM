
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "sys/etimer.h"
#include "sys/timer.h"
#include "sys/ctimer.h"
#include "sys/rtimer.h"
#include "contiki-net.h"

#define DHT11_PIN 2
#define READ_INTERVAL (5 * CLOCK_SECOND)

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

static void dht11_init(void) {
    // Initialize the DHT11 sensor connected to GPIO pin 2
    gpio_hal_set_input(DHT11_PIN);
}

static int dht11_read(void) {
    // Simulate reading the DHT11 sensor
    // In a real implementation, you would read the actual data from the sensor
    return 25; // Simulated temperature value
}

PROCESS_THREAD(dht11_read_process, ev, data) {
    static struct etimer timer;
    static int temperature;

    PROCESS_BEGIN();

    dht11_init();

    etimer_set(&timer, READ_INTERVAL);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        temperature = dht11_read();
        printf("Temperature: %d C\n", temperature);

        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void) {
    // Initialize Contiki
    contiki_init();

    // Start the Contiki system
    contiki_start();

    return 0;
}
