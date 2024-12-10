
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_tcp_t tcp_sock;
static mqtt_client_t mqtt_client;
static char sendbuf[128];
static char readbuf[128];

void mqtt_connect(void) {
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_addr_from_str(remote.addr.ipv4, MQTT_BROKER_IP);

    int res = sock_tcp_create(&tcp_sock, &local, &remote, 0);
    if (res < 0) {
        printf("Failed to create TCP socket: %d\n", res);
        return;
    }

    mqtt_client_init(&mqtt_client, &tcp_sock, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    mqtt_connect_info_t conn_info = {
        .client_id = MQTT_CLIENT_ID,
        .keep_alive = 60,
        .clean_session = 1,
    };

    res = MQTTPublish(&mqtt_client, MQTT_TOPIC, &(mqtt_message_t){
        .payload = MQTT_MESSAGE,
        .payload_len = strlen(MQTT_MESSAGE),
        .qos = MQTT_QOS,
        .retain = MQTT_RETAIN,
    });

    if (res != 0) {
        printf("Failed to publish message: %d\n", res);
    }
}

void mqtt_publish_state(void) {
    static const char *states[] = { "work", "not work" };
    static int state_index = 0;

    const char *state = states[state_index];
    state_index = (state_index + 1) % 2;

    int res = MQTTPublish(&mqtt_client, MQTT_TOPIC, &(mqtt_message_t){
        .payload = state,
        .payload_len = strlen(state),
        .qos = MQTT_QOS,
        .retain = MQTT_RETAIN,
    });

    if (res != 0) {
        printf("Failed to publish message: %d\n", res);
    }
}

int main(void) {
    mqtt_connect();

    while (1) {
        mqtt_publish_state();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
