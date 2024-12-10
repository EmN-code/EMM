
#include "contiki.h"
#include "dev/gpio-hal.h"
#include "sys/etimer.h"
#include "sys/timer.h"
#include "sys/rtimer.h"

#define DHT11_GPIO_PORT (0)
#define DHT11_GPIO_PIN  (2)

PROCESS(dht11_reading_process, "DHT11 Reading Process");
AUTOSTART_PROCESSES(&dht11_reading_process);

static void dht11_init(void) {
    gpio_hal_set_input(DHT11_GPIO_PORT, DHT11_GPIO_PIN);
}

static void dht11_read(void) {
    // Placeholder for DHT11 reading logic
    // This function should contain the logic to read from the DHT11 sensor
    // connected to GPIO 2 on the NRF52840.
    printf("DHT11 reading placeholder\n");
}

PROCESS_THREAD(dht11_reading_process, ev, data) {
    static struct etimer timer;

    PROCESS_BEGIN();

    dht11_init();

    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        dht11_read();

        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void) {
    contiki_init();
    return 0;
}
