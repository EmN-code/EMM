
#include "contiki.h"
#include "sys/etimer.h"
#include "dev/gpio-hal.h"
#include "dev/leds.h"

#define DHT11_GPIO_PORT 0
#define DHT11_GPIO_PIN 1
#define LED_GPIO_PORT 0
#define LED_GPIO_PIN 2

#define DHT11_SIGNAL_HIGH 1
#define DHT11_SIGNAL_LOW 0
#define DHT11_SIGNAL_START_PHASE1_DURATION 40
#define DHT11_SIGNAL_START_PHASE2_DURATION 80
#define DHT11_SIGNAL_START_PHASE3_DURATION 80
#define DHT11_SIGNAL_RESPONSE_PHASE1_DURATION 50
#define DHT11_SIGNAL_RESPONSE_BIT_0_DURATION 28
#define DHT11_SIGNAL_RESPONSE_BIT_1_DURATION 70
#define DHT11_TICKS_GUARD 1
#define DHT11_US_GUARD RTIMERTICKS_TO_US(1)
#define DHT11_DATA_SAMPLES 40
#define DHT11_DATA_SIZE 5
#define DHT11_SAMPLING_RATE_SECONDS 1

typedef struct {
  gpio_hal_port_t port;
  gpio_hal_pin_t pin;
  uint8_t status;
  clock_time_t last_read;
  uint8_t data[DHT11_DATA_SIZE];
} dht_t;

static dht_t dht;

static int8_t dht_signal_duration(uint8_t active, uint32_t max_duration) {
  rtimer_clock_t elapsed_ticks;
  rtimer_clock_t max_wait_ticks = US_TO_RTIMERTICKS(max_duration) + DHT11_TICKS_GUARD;
  rtimer_clock_t start_ticks = RTIMER_NOW();

  RTIMER_BUSYWAIT_UNTIL(gpio_hal_arch_read_pin(dht.port, dht.pin) != active, max_wait_ticks);

  elapsed_ticks = RTIMER_NOW() - start_ticks;

  if(elapsed_ticks > max_wait_ticks) {
    return -1;
  }

  return RTIMERTICKS_TO_US(elapsed_ticks);
}

static int8_t dht_signal_transition(uint8_t active, uint32_t max_duration) {
  return dht_signal_duration(active, max_duration);
}

static uint8_t dht_verify_checksum(void) {
  return ((dht.data[0] + dht.data[1] + dht.data[2] + dht.data[3]) & 0xFF) == dht.data[4];
}

static uint8_t dht_read(void) {
  uint8_t j, i;
  int8_t data_signal_duration[DHT11_DATA_SAMPLES];

  gpio_hal_arch_pin_set_output(dht.port, dht.pin);
  gpio_hal_arch_clear_pin(dht.port, dht.pin);
  RTIMER_BUSYWAIT(US_TO_RTIMERTICKS(18000UL));
  gpio_hal_arch_set_pin(dht.port, dht.pin);
  gpio_hal_arch_pin_set_input(dht.port, dht.pin);

  if(dht_signal_transition(DHT11_SIGNAL_HIGH, DHT11_SIGNAL_START_PHASE1_DURATION) == -1) {
    return 1;
  }

  if(dht_signal_transition(DHT11_SIGNAL_LOW, DHT11_SIGNAL_START_PHASE2_DURATION) == -1) {
    return 1;
  }

  if(dht_signal_transition(DHT11_SIGNAL_HIGH, DHT11_SIGNAL_START_PHASE3_DURATION) == -1) {
    return 1;
  }

  for(i = 0; i < DHT11_DATA_SAMPLES; i++) {
    if(dht_signal_transition(DHT11_SIGNAL_LOW, DHT11_SIGNAL_RESPONSE_PHASE1_DURATION) == -1) {
      return 1;
    }

    data_signal_duration[i] = dht_signal_duration(DHT11_SIGNAL_HIGH,
                                                  DHT11_SIGNAL_RESPONSE_BIT_0_DURATION
                                                  + DHT11_SIGNAL_RESPONSE_BIT_1_DURATION);
    if(data_signal_duration[i] == -1) {
      return 1;
    }
  }

  memset(dht.data, 0, sizeof(uint8_t) * DHT11_DATA_SIZE);
  for(j = 0, i = 0; i < DHT11_DATA_SAMPLES; i++) {
    if(data_signal_duration[i] >= DHT11_SIGNAL_RESPONSE_BIT_0_DURATION + DHT11_US_GUARD) {
      dht.data[j] = (dht.data[j] << 1) | 1;
    } else {
      dht.data[j] = dht.data[j] << 1;
    }

    if(i % 8 == 7U) {
      j++;
    }
  }

  if(!dht_verify_checksum()) {
    return 2;
  } else {
    return 0;
  }
}

PROCESS(dht11_process, "DHT11 Process");
AUTOSTART_PROCESSES(&dht11_process);

PROCESS_THREAD(dht11_process, ev, data) {
  static struct etimer et;

  PROCESS_BEGIN();

  dht.port = DHT11_GPIO_PORT;
  dht.pin = DHT11_GPIO_PIN;
  dht.last_read = 0;

  gpio_hal_arch_pin_set_output(LED_GPIO_PORT, LED_GPIO_PIN);
  gpio_hal_arch_clear_pin(LED_GPIO_PORT, LED_GPIO_PIN);

  etimer_set(&et, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    dht.status = dht_read();

    if(dht.status == 0) {
      gpio_hal_arch_set_pin(LED_GPIO_PORT, LED_GPIO_PIN);
    } else {
      gpio_hal_arch_clear_pin(LED_GPIO_PORT, LED_GPIO_PIN);
    }

    etimer_reset(&et);
  }

  PROCESS_END();
}

int main(void) {
  contiki_init();
  return 0;
}
