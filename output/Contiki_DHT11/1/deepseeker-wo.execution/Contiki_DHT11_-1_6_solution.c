
#include "contiki-default-conf.h"
#include "sys/clock.h"
#include "sys/etimer.h"
#include "dev/gpio-hal.h"

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN 2

#define DHT11_VALUE_HUMIDITY_INTEGER    (0)
#define DHT11_VALUE_HUMIDITY_DECIMAL    (1)
#define DHT11_VALUE_TEMPERATURE_INTEGER (2)
#define DHT11_VALUE_TEMPERATURE_DECIMAL (3)

#define DHT11_STATUS_OKAY               (0)
#define DHT11_STATUS_TIMEOUT            (1)
#define DHT11_STATUS_CHECKSUM_FAILED    (2)

#define DHT11_SAMPLING_RATE_SECONDS     (5)

typedef struct {
  gpio_hal_port_t port;
  gpio_hal_pin_t pin;
  uint8_t status;
  clock_time_t last_read;
  uint8_t data[5];
} dht_t;

static dht_t dht;

static int8_t dht_signal_duration(uint8_t active, uint32_t max_duration);
static uint8_t dht_verify_checksum(void);
static uint8_t dht_read(void);

static int value(int type);
static int status(int type);
static int configure(int type, int c);

static int8_t dht_signal_duration(uint8_t active, uint32_t max_duration) {
  rtimer_clock_t elapsed_ticks;
  rtimer_clock_t max_wait_ticks = US_TO_RTIMERTICKS(max_duration) + 1;
  rtimer_clock_t start_ticks = RTIMER_NOW();

  RTIMER_BUSYWAIT_UNTIL(gpio_hal_arch_read_pin(dht.port, dht.pin) != active, max_wait_ticks);

  elapsed_ticks = RTIMER_NOW() - start_ticks;

  if(elapsed_ticks > max_wait_ticks) {
    return -1;
  }

  return RTIMERTICKS_TO_US(elapsed_ticks);
}

static uint8_t dht_verify_checksum(void) {
  return ((dht.data[0] + dht.data[1] + dht.data[2] + dht.data[3]) & 0xFF) == dht.data[4];
}

static uint8_t dht_read(void) {
  uint8_t j, i;
  int8_t data_signal_duration[40];

  gpio_hal_arch_pin_set_output(dht.port, dht.pin);
  gpio_hal_arch_clear_pin(dht.port, dht.pin);
  RTIMER_BUSYWAIT(US_TO_RTIMERTICKS(18000UL));
  gpio_hal_arch_set_pin(dht.port, dht.pin);
  gpio_hal_arch_pin_set_input(dht.port, dht.pin);

  if(dht_signal_duration(1, 40) == -1) {
    return DHT11_STATUS_TIMEOUT;
  }

  if(dht_signal_duration(0, 80) == -1) {
    return DHT11_STATUS_TIMEOUT;
  }

  if(dht_signal_duration(1, 80) == -1) {
    return DHT11_STATUS_TIMEOUT;
  }

  for(i = 0; i < 40; i++) {
    if(dht_signal_duration(0, 50) == -1) {
      return DHT11_STATUS_TIMEOUT;
    }

    data_signal_duration[i] = dht_signal_duration(1, 98);
    if(data_signal_duration[i] == -1) {
      return DHT11_STATUS_TIMEOUT;
    }
  }

  memset(dht.data, 0, sizeof(uint8_t) * 5);
  for(j = 0, i = 0; i < 40; i++) {
    if(data_signal_duration[i] >= 28 + 1) {
      dht.data[j] = (dht.data[j] << 1) | 1;
    } else {
      dht.data[j] = dht.data[j] << 1;
    }

    if(i % 8 == 7U) {
      j++;
    }
  }

  if(!dht_verify_checksum()) {
    return DHT11_STATUS_CHECKSUM_FAILED;
  } else {
    return DHT11_STATUS_OKAY;
  }
}

static int value(int type) {
  switch(type) {
  case DHT11_VALUE_HUMIDITY_INTEGER:
    return dht.data[0];
  case DHT11_VALUE_HUMIDITY_DECIMAL:
    return dht.data[1];
  case DHT11_VALUE_TEMPERATURE_INTEGER:
    return dht.data[2];
  case DHT11_VALUE_TEMPERATURE_DECIMAL:
    return dht.data[3];
  }

  return 0;
}

static int status(int type) {
  (void)type;
  return dht.status;
}

static int configure(int type, int c) {
  switch(type) {
  case DHT11_CONFIGURE_GPIO_PORT:
    dht.port = c;
    break;
  case DHT11_CONFIGURE_GPIO_PIN:
    dht.pin = c;
    break;
  case SENSORS_HW_INIT:
    dht.last_read = 0;
  case SENSORS_ACTIVE:
    if(c == 1) {
      clock_time_t now;

      now = clock_seconds();
      if(now - dht.last_read < DHT11_SAMPLING_RATE_SECONDS) {
        return 0;
      }
      dht.last_read = now;
      dht.status = dht_read();
    }
  case SENSORS_READY:
    break;
  default:
    return 0;
  }

  return 1;
}

int main() {
  struct etimer timer;
  int humidity, temperature;

  configure(DHT11_CONFIGURE_GPIO_PORT, DHT11_GPIO_PORT);
  configure(DHT11_CONFIGURE_GPIO_PIN, DHT11_GPIO_PIN);

  while(1) {
    etimer_set(&timer, CLOCK_SECOND * DHT11_SAMPLING_RATE_SECONDS);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    configure(SENSORS_ACTIVE, 1);

    if(status(0) == DHT11_STATUS_OKAY) {
      humidity = value(DHT11_VALUE_HUMIDITY_INTEGER);
      temperature = value(DHT11_VALUE_TEMPERATURE_INTEGER);
    }

    etimer_reset(&timer);
  }

  return 0;
}
