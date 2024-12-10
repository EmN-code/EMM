
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_KEEPALIVE 60
#define MQTT_QOS 0

static mqtt_client_t client;
static sock_udp_ep_t broker = { .family = AF_INET };
static char sendbuf[128];
static char readbuf[128];

void mqtt_publish_state(void) {
    static int state = 0;
    const char *states[] = { "not work", "work" };

    mqtt_publish_params_t pub_params = {
        .topic = MQTT_TOPIC,
        .message = states[state],
        .qos = MQTT_QOS,
        .retain = 0,
        .message_length = strlen(states[state])
    };

    mqtt_publish(&client, &pub_params);
    state = !state;
}

int main(void) {
    sock_udp_ep_t local = { .family = AF_INET };
    sock_udp_t sock;

    inet_addr_from_str(broker.addr.ipv4, MQTT_BROKER_IP);
    broker.port = MQTT_BROKER_PORT;

    if (sock_udp_create(&sock, &local, &broker, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    mqtt_client_config_t config = {
        .sendbuf = sendbuf,
        .sendbuf_size = sizeof(sendbuf),
        .recvbuf = readbuf,
        .recvbuf_size = sizeof(readbuf),
        .client_id = MQTT_CLIENT_ID,
        .keep_alive = MQTT_KEEPALIVE
    };

    mqtt_client_init(&client, &config);

    if (mqtt_connect(&client, &broker) != 0) {
        puts("Error connecting to MQTT broker");
        return 1;
    }

    while (1) {
        mqtt_publish_state();
        xtimer_sleep(5);
    }

    mqtt_disconnect(&client);
    sock_udp_close(&sock);

    return 0;
}
