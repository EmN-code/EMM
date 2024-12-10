#include "contiki.h"
#include "contiki-conf.h"
#include "contiki-lib.h"
#include "contiki-net.h"
#include "contiki-default-conf.h"
#include "net/linkaddr.h"
#include "net/netstack.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-nameserver.h"
#include "tcp-socket.h"
#include "udp-socket.h"

#include "dht11_sensor.h"
#include "dev/gpio-hal.h"

#define DHT11_GPIO_PORT (0)
#define DHT11_GPIO_PIN (2)

static struct etimer timer;

PROCESS(dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data)
{
    PROCESS_BEGIN();

    gpio_hal_arch_pin_set_output(DHT11_GPIO_PORT, DHT11_GPIO_PIN);

    etimer_set(&timer, CLOCK_SECOND * 5);

    while (1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        int temperature = dht11_sensor.value(DHT11_VALUE_TEMPERATURE_INTEGER);
        int humidity = dht11_sensor.value(DHT11_VALUE_HUMIDITY_INTEGER);

        /* Print temperature and humidity */
        printf("Temperature: %d°C, Humidity: %d%%\n", temperature, humidity);

        etimer_reset(&timer);
    }

    PROCESS_END();
}
