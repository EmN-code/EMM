#include <string.h>
#include <xtimer.h>
#include <net/mqttsn.h>
#include "mqtt_client.h"

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define PUBLISH_INTERVAL (5 * US_PER_SEC)
#define MSG_BUFFER_SIZE (128)

static char mqtt_client_id[] = "esp32_client";
static char publish_topic[] = "device/state";
static char publish_message[] = "work";

static mqtt_client_t mqtt_client;
static mqtt_client_config_t mqtt_config;

static void on_message_received(const mqtt_message_data_t *md) {
    (void)md;
    printf("Message received\n");
}

static int mqtt_setup(void) {
    mqtt_config.client_id = mqtt_client_id;
    mqtt_config.timeout = CONFIG_MQTT_TIMEOUT_MS_DEFAULT;
    mqtt_config.keep_alive = 10;
    mqtt_config.clean_session = 1;
    mqtt_client_tls_enable(&mqtt_client, &mqtt_config);
    int res = mqttsn_init(&mqtt_client, MQTT_BROKER_IP, MQTT_BROKER_PORT,
                          publish_topic, on_message_received);
    return res;
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();
    if (mqtt_setup() != 0) {
        printf("MQTT setup failed\n");
        return 1;
    }

    while (1) {
        printf("Publishing message: %s\n", publish_message);

        mqttsn_publish(&mqtt_client, publish_topic, publish_message,
                       strlen(publish_message), MQTT_QoS_0);

        xtimer_periodic_wakeup(&last_wakeup, PUBLISH_INTERVAL);
    }

    return 0;
}