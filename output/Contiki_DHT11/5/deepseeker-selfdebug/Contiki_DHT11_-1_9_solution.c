
#include "contiki.h"
#include "dht11-sensor.h"
#include <stdio.h>

PROCESS(dht11_read_process, "DHT11 Read Process");
AUTOSTART_PROCESSES(&dht11_read_process);

/* Function prototypes */
int configure(int type, int c);
int value(int type);
void contiki_init(void);

PROCESS_THREAD(dht11_read_process, ev, data)
{
    static struct etimer timer;
    static int humidity_integer, humidity_decimal, temperature_integer, temperature_decimal;

    PROCESS_BEGIN();

    /* Configure the DHT11 sensor */
    configure(DHT11_CONFIGURE_GPIO_PORT, 0);
    configure(DHT11_CONFIGURE_GPIO_PIN, 2);
    configure(SENSORS_HW_INIT, 0);

    /* Set the timer to read the sensor every 5 seconds */
    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        /* Read the sensor values */
        humidity_integer = value(DHT11_VALUE_HUMIDITY_INTEGER);
        humidity_decimal = value(DHT11_VALUE_HUMIDITY_DECIMAL);
        temperature_integer = value(DHT11_VALUE_TEMPERATURE_INTEGER);
        temperature_decimal = value(DHT11_VALUE_TEMPERATURE_DECIMAL);

        /* Print the values */
        printf("Humidity: %d.%d %%, Temperature: %d.%d C\n",
               humidity_integer, humidity_decimal,
               temperature_integer, temperature_decimal);

        /* Reset the timer */
        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void)
{
    contiki_init();
    return 0;
}
