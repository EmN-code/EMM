#include <stdio.h>
#include <mqtt_client.h>
#include <net/af.h>
#include <net/sock/udp.h>
#include <ambient/sensors/ambient_sensors.h>
#include <osdependent/mqtt_thread.h>
#include <contiki.h>
#include <init.h>
#include BEACON_HEADER
#include "MQTTClient.h"

#define MQTT_HOST_IP "47.102.103.1"
#define MQTT_HOST_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_BUFFER_SIZE CONFIG_MQTT_MESSAGE_BUFFER_SIZE

static mqtt_client_t mqtt_client;
static char mqtt_buffer[MQTT_BUFFER_SIZE];
static const mqtt_connect_opt_t mqtt_options = {
    .keep_alive = CONFIG_EMQTT_KEEP_ALIVE,
    .clean_session = true
};

static emcute_topic_t state_topic;
static char timer_stack[TIMER_STACKSIZE_DEFAULT];
static kernel_pid_t timer_pid;

static void *publish_state(void *arg) {
    (void)arg;
    char *state = "work";
    while (1) {
        mqtt_msg_t message;
        memset(&message, 0, sizeof(message));
        message.data = (uint8_t *)state;
        message.data_len = strlen(state);
        if (mqtt_unsecure_transport_init()) {
            printf("MQTT publish failed\n");
        }
        mqtt_pub(MQTT_TOPIC, &message);
        xtimer_usleep(5 * US_PER_SEC);
    }
    return NULL;
}

int main(void) {
    printf("MQTT Publish State\n");
    network_init();
    mqtt_client_init(&mqtt_client, &mqtt_buffer, sizeof(mqtt_buffer), &mqtt_options);
    timer_pid = thread_create(timer_stack, sizeof(timer_stack), THREAD_PRIORITY_MAIN - 1, 0,
                              publish_state, NULL, "publish_state");
    if (mqtt_connect(MQTT_HOST_IP, MQTT_HOST_PORT) != MQTT_SUCCESS) {
        printf("Failed to connect to MQTT broker\n");
        return 1;
    }
    emcute_reg(&state_topic);
    return 0;
}
