#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <timex.h>
#include <ztimer.h>
#include <net/mqtt.h>
#include <net/emcute.h>

#define SERVER_ADDR   "47.102.103.1"
#define SERVER_PORT   1883
#define CLIENT_ID     "esp32_client"
#define TOPIC         "device/state"
#define INTERVAL      (5U * US_PER_SEC)  // 5 seconds

static mqtt_client_t mqtt_client;
static mqtt_connect_params_t mqtt_params;
static mqtt_callbacks_t mqtt_callbacks;
static char buffer[128];

static ztimer_t timer;

static void on_timer(void *arg) {
    mqtt_message_t msg;
    char *state = (rand() % 2 == 0) ? "work" : "not work";

    printf("Publishing state: %s\n", state);

    msg.payload = state;
    msg.payload_len = strlen(state);

    mqtt_publish(&mqtt_client, TOPIC, &msg);

    ztimer_set(&timer, INTERVAL);
}

static void mqtt_on_event(enum mqtt_event ev, void *arg) {
    switch (ev) {
        case MQTT_EVENT_CONNECTED:
            printf("Connected to broker\n");
            ztimer_set(&timer, INTERVAL);
            break;

        case MQTT_EVENT_DISCONNECTED:
            printf("Disconnected from broker, attempting reconnect\n");
            mqtt_connect(&mqtt_client, &mqtt_params);
            break;

        default:
            break;
    }
}

int main(void) {
    printf("Starting MQTT client\n");

    mqtt_params.client_id = CLIENT_ID;
    mqtt_params.addr = SERVER_ADDR;
    mqtt_params.port = SERVER_PORT;

    mqtt_callbacks.on_event = mqtt_on_event;

    mqtt_init(&mqtt_client, &mqtt_params, mqtt_callbacks);
    mqtt_connect(&mqtt_client, &mqtt_params);

    while (1) {
        mqtt_loop(&mqtt_client);
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    return 0;
}