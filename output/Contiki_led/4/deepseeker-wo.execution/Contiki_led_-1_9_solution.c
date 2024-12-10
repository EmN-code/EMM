
#include "contiki.h"
#include "sys/etimer.h"
#include "dev/gpio-hal.h"
#include "dev/leds.h"
#include "dev/dht11-sensor.h"

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN 1
#define LED_GPIO_PORT 0
#define LED_GPIO_PIN 2

#define READ_INTERVAL (5 * CLOCK_SECOND)

static struct etimer read_timer;

void read_dht11(void) {
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PORT);
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);
    dht11_sensor.configure(SENSORS_ACTIVE, 1);

    if (dht11_sensor.status(0) == DHT11_STATUS_OKAY) {
        leds_on(LEDS_RED);
    } else {
        leds_off(LEDS_RED);
    }
}

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

PROCESS_THREAD(dht11_read_process, ev, data) {
    PROCESS_BEGIN();

    etimer_set(&read_timer, READ_INTERVAL);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&read_timer));
        read_dht11();
        etimer_reset(&read_timer);
    }

    PROCESS_END();
}

int main(void) {
    gpio_hal_pin_set_output(LED_GPIO_PORT, LED_GPIO_PIN);
    gpio_hal_clear_pin(LED_GPIO_PORT, LED_GPIO_PIN);

    process_start(&dht11_read_process, NULL);

    while (1) {
        process_run();
    }

    return 0;
}
