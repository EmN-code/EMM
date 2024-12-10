
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "sys/etimer.h"
#include "sys/timer.h"
#include "sys/ctimer.h"
#include "sys/rtimer.h"
#include "contiki-default-conf.h"

#define DHT11_PIN 2
#define READ_INTERVAL (5 * CLOCK_SECOND)

static struct etimer read_timer;

void dht11_read(void) {
    // Simulate reading from DHT11 connected to GPIO 2
    gpio_hal_set(DHT11_PIN);
    clock_delay(18000); // Delay for 18ms
    gpio_hal_clear(DHT11_PIN);
    clock_delay(40); // Delay for 40us

    // Read the response from DHT11
    if (gpio_hal_read(DHT11_PIN) == 0) {
        // DHT11 is ready to send data
        // Implement the data reading logic here
        // For simplicity, we just print a message
        printf("DHT11 data read successfully\n");
    } else {
        printf("DHT11 not responding\n");
    }
}

PROCESS(dht11_process, "DHT11 Reading Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data) {
    PROCESS_BEGIN();

    etimer_set(&read_timer, READ_INTERVAL);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&read_timer));
        dht11_read();
        etimer_reset(&read_timer);
    }

    PROCESS_END();
}

int main(void) {
    // Initialize the Contiki system
    contiki_init();

    // Start the DHT11 reading process
    process_start(&dht11_process, NULL);

    // Enter the Contiki main loop
    while (1) {
        process_run();
    }

    return 0;
}
