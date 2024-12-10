
#include "contiki.h"
#include "sys/etimer.h"
#include "dev/gpio-hal.h"
#include "sys/clock.h"

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN 2

#define SENSORS_HW_INIT 0
#define SENSORS_ACTIVE 1
#define SENSORS_READY 2

#define DHT11_CONFIGURE_GPIO_PORT 0
#define DHT11_CONFIGURE_GPIO_PIN 1

#define DHT11_VALUE_HUMIDITY_INTEGER 0
#define DHT11_VALUE_HUMIDITY_DECIMAL 1
#define DHT11_VALUE_TEMPERATURE_INTEGER 2
#define DHT11_VALUE_TEMPERATURE_DECIMAL 3

#define DHT11_STATUS_OKAY 0
#define DHT11_STATUS_TIMEOUT 1
#define DHT11_STATUS_CHECKSUM_FAILED 2

#define DHT11_SAMPLING_RATE_SECONDS 5

static struct etimer et;

static int dht11_read(void);
static int dht11_value(int type);
static int dht11_configure(int type, int c);
static int dht11_status(int type);

SENSORS_SENSOR(dht11_sensor, "dht11", dht11_value, dht11_configure, dht11_status);

static int dht11_value(int type) {
  switch(type) {
    case DHT11_VALUE_HUMIDITY_INTEGER:
    case DHT11_VALUE_HUMIDITY_DECIMAL:
    case DHT11_VALUE_TEMPERATURE_INTEGER:
    case DHT11_VALUE_TEMPERATURE_DECIMAL:
      return dht11_read();
  }
  return 0;
}

static int dht11_configure(int type, int c) {
  switch(type) {
    case DHT11_CONFIGURE_GPIO_PORT:
    case DHT11_CONFIGURE_GPIO_PIN:
    case SENSORS_HW_INIT:
    case SENSORS_ACTIVE:
    case SENSORS_READY:
      return 1;
  }
  return 0;
}

static int dht11_status(int type) {
  (void)type;
  return DHT11_STATUS_OKAY;
}

static int dht11_read(void) {
  // Dummy implementation for the sake of example
  return 0;
}

PROCESS(dht11_process, "DHT11 Sensor Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data) {
  static int humidity, temperature;

  PROCESS_BEGIN();

  dht11_configure(DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PORT);
  dht11_configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);
  dht11_configure(SENSORS_HW_INIT, 0);

  etimer_set(&et, CLOCK_SECOND * DHT11_SAMPLING_RATE_SECONDS);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    dht11_configure(SENSORS_ACTIVE, 1);
    humidity = dht11_value(DHT11_VALUE_HUMIDITY_INTEGER);
    temperature = dht11_value(DHT11_VALUE_TEMPERATURE_INTEGER);
    dht11_configure(SENSORS_READY, 0);

    etimer_reset(&et);
  }

  PROCESS_END();
}

int main(void) {
  contiki_init();
  return 0;
}
