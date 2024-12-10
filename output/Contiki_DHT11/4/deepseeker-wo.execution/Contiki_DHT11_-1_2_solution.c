
#include "contiki-net.h"
#include "contiki-lib.h"
#include "contiki-conf.h"
#include "contiki.h"
#include "contiki-default-conf.h"
#include "net/ipv6/simple-udp.h"
#include "coap-observe-client.h"
#include "net/ipv6/uip-nameserver.h"
#include "net/ipv6/uiplib.h"

#include "nrf_gpio.h"
#include "nrf_delay.h"

#define DHT11_PIN 2

static uint8_t dht11_read_byte(void) {
    uint8_t i, byte = 0;
    for(i = 0; i < 8; i++) {
        while(!nrf_gpio_pin_read(DHT11_PIN));
        nrf_delay_us(40);
        if(nrf_gpio_pin_read(DHT11_PIN)) {
            byte |= (1 << (7 - i));
        }
        while(nrf_gpio_pin_read(DHT11_PIN));
    }
    return byte;
}

static void dht11_read(uint8_t *humidity, uint8_t *temperature) {
    uint8_t checksum, data[5];

    nrf_gpio_cfg_output(DHT11_PIN);
    nrf_gpio_pin_clear(DHT11_PIN);
    nrf_delay_ms(18);
    nrf_gpio_pin_set(DHT11_PIN);
    nrf_delay_us(40);
    nrf_gpio_cfg_input(DHT11_PIN, NRF_GPIO_PIN_PULLUP);
    nrf_delay_us(160);

    if(nrf_gpio_pin_read(DHT11_PIN) == 0) {
        while(!nrf_gpio_pin_read(DHT11_PIN));
        while(nrf_gpio_pin_read(DHT11_PIN));

        data[0] = dht11_read_byte();
        data[1] = dht11_read_byte();
        data[2] = dht11_read_byte();
        data[3] = dht11_read_byte();
        data[4] = dht11_read_byte();

        checksum = data[0] + data[1] + data[2] + data[3];
        if(checksum == data[4]) {
            *humidity = data[0];
            *temperature = data[2];
        }
    }
}

PROCESS(dht11_process, "DHT11 Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data)
{
    static struct etimer timer;
    static uint8_t humidity, temperature;

    PROCESS_BEGIN();

    etimer_set(&timer, CLOCK_SECOND * 5);

    while(1) {
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

        dht11_read(&humidity, &temperature);

        etimer_reset(&timer);
    }

    PROCESS_END();
}

int main(void)
{
    contiki_init();
    return 0;
}
