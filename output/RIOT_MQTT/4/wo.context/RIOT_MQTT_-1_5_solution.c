#include "xtimer.h"
#include "net/mqtt.h"
#include "net/emcute.h"
#include "MQTTClient.h"

#define BROKER_IP   "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC_NAME  "state/status"
#define CLIENT_ID   "ESP32_MQTT_Client"

static char mqtt_state[16];

static void *emcute_thread(void *args) {
    (void)args;
    mqtt_client_t client;
    mqtt_client_config_t client_config;
    mqtt_message_t message;
    mqtt_broker_handle_t *broker = mqtt_connect(CLIENT_ID, BROKER_IP, BROKER_PORT);
    int ret;

    if (broker == NULL) {
        printf("Error: Could not connect to MQTT broker\n");
        return NULL;
    }

    while (1) {
        snprintf(mqtt_state, sizeof(mqtt_state), "work"); // set state
        message.payload = mqtt_state;
        message.payloadlen = strlen(mqtt_state);
        message.qos = QOS0;
        ret = mqtt_publish(broker, TOPIC_NAME, &message);

        if (ret < 0) {
            printf("Error: Could not publish message\n");
            continue;
        }

        printf("Published message: %s\n", mqtt_state);
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    pthread_create(&emcute_thread_pid, NULL, emcute_thread, NULL);
    pthread_join(emcute_thread_pid, NULL);
    return 0;
}
