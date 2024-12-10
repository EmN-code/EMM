#include <unistd.h>
#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "thread_config.h"
#include "thread.h"
#include "time_units.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "paho_mqtt.h"
#include "msg.h"
#include "mbox.h"
#include "MQTTClient.h"
#include "event/timeout.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "dht_params.h"
#include "dht.h"

static kernel_pid_t thread_pid;
#define DHT11_PIN GPIO_PIN(0, 2)  // Assuming GPIO 2 is on PORT 0
#define DHT11_TYPE DHT11
#define MQTT_BUFFER_SIZE 128

static char _thread_stack[THREAD_STACKSIZE_MAIN];
char mqtt_buffer[MQTT_BUFFER_SIZE];

void *_dht_thread(void *arg) {
    (void)arg;
    gpio_init(DHT11_PIN, GPIO_IN);

    while (1) {
        int16_t temp, hum;
        if (dht_read(&dht_params[DHT11_TYPE], &temp, &hum) == DHT_OK) {
            printf("Temperature: %d.%d°C
", temp / 10, temp % 10);
            printf("Humidity: %d.%d%%
", hum / 10, hum % 10);
        } else {
            printf("Failed to read from sensor\n");
        }
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return NULL;
}

void _mqtt_setup(void) {
    MQTTClient client;
    Network network;
    int rc;

    MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
    char *address = "tcp://broker.hivemq.com:1883";

    NewNetwork(&network);
    ConnectNetwork(&network, address);
    MQTTClientInit(&client, &network, 1000, mqtt_buffer, MQTT_BUFFER_SIZE, mqtt_buffer, MQTT_BUFFER_SIZE);

    connectData.MQTTVersion = 3;
    connectData.clientID.cstring = "RIOT-ESP32-client";

    if ((rc = MQTTConnect(&client, &connectData)) != 0) {
        printf("Failed to connect, return code %d\n", rc);
        return;
    }

    printf("Connected to MQTT broker\n");
}

int main(void) {
    // Create thread for DHT11 sensor
    thread_pid = kernel_pid_t thread_create(_thread_stack, sizeof(_thread_stack), THREAD_PRIORITY_MAIN - 1,
                 THREAD_CREATE_STACKTEST, _dht_thread, NULL, "dht_thread");

    // Setup MQTT client
    _mqtt_setup();

    // Application loop
    while (1) {
        // Perform other tasks
        ztimer_sleep(ZTIMER_SEC, 10);
    }
    
    return 0;
}