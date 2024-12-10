#include <xtimer.h>
#include <stdio.h>
#include <net/sock/udp.h>
#include <net/mqttsn.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID "mqtt_client"
#define KEEPALIVE_SEC 60
#define STATE_TOPIC "state"

static sock_udp_ep_t gw = { .family = AF_INET, .port = SERVER_PORT };
static mqtt_client_t client;
static mqtt_client_config_t client_conf = {
    .client_id = CLIENT_ID,
    .will_topic = NULL,
    .will_message = NULL,
    .username = NULL,
    .password = NULL,
    .keep_alive = KEEPALIVE_SEC,
    .clean_session = 1
};

void *publish_state(void *arg) {
    char message[] = "work";

    while(1) {
        mqtt_msg_t msg;
        msg.payload = message;
        msg.payload_len = strlen(message);
        msg.qos = QOS_0;
        msg.retain = false;

        mqtt_sn_publish(&client, &msg, STATE_TOPIC);

        xtimer_sleep(5);

        // Change state
        if (strcmp(message, "work") == 0) {
            strcpy(message, "not work");
        } else {
            strcpy(message, "work");
        }
    }
    return NULL;
}

int main(void) {
    inet_pton(AF_INET, SERVER_IP, &gw.addr.ipv4);

    mqtt_client_init(&client, &client_conf, NULL);
    mqtt_sn_connect(&client, &gw, KEEPALIVE_SEC);

    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, publish_state, NULL, "pub thread");

    return 0;
}