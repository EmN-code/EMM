
#include "contiki-default-conf.h"
#include "sys/etimer.h"
#include "contiki.h"
#include "contiki-net.h"
#include "dht11-sensor.h"
#include "random.h"

#define DHT11_GPIO_PIN 2
#define READ_INTERVAL (5 * CLOCK_SECOND)

static struct etimer read_timer;

void read_dht11(void) {
    static int last_read_time = 0;
    int now = clock_seconds();

    if (now - last_read_time < 5) {
        return;
    }

    last_read_time = now;

    dht11_sensor.configure(SENSORS_ACTIVE, 1);
    int humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);
    int temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);

    // Print or process the humidity and temperature values here
    // For example:
    // printf("Humidity: %d, Temperature: %d\n", humidity, temperature);
}

int main(void) {
    dht11_sensor.configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);
    dht11_sensor.configure(SENSORS_HW_INIT, 0);

    etimer_set(&read_timer, READ_INTERVAL);

    while (1) {
        if (etimer_expired(&read_timer)) {
            read_dht11();
            etimer_reset(&read_timer);
        }

        // Other processing can be done here
    }

    return 0;
}
