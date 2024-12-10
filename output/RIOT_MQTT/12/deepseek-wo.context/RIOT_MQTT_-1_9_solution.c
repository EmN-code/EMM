
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_DUP 0
#define MQTT_ID 1
#define MQTT_KEEPALIVE 60
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL

static void mqtt_connect(mqtt_client_t *client, const char *host, uint16_t port) {
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = port };
    inet_addr_from_str(host, &remote.addr.ipv4);

    int res = sock_tcp_connect(&client->sock, &remote, &local, SOCK_FLAGS_REUSE_EP);
    if (res < 0) {
        printf("Error connecting to MQTT broker: %d\n", res);
        return;
    }

    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
    options.clientID.cstring = "RIOT_MQTT_CLIENT";
    options.keepAliveInterval = MQTT_KEEPALIVE;
    options.cleansession = 1;
    options.username.cstring = MQTT_USERNAME;
    options.password.cstring = MQTT_PASSWORD;

    res = MQTTConnect(client, &options);
    if (res != 0) {
        printf("Error connecting to MQTT broker: %d\n", res);
        return;
    }

    printf("Connected to MQTT broker\n");
}

static void mqtt_publish(mqtt_client_t *client, const char *topic, const char *message) {
    MQTTMessage msg = {
        .qos = MQTT_QOS,
        .retained = MQTT_RETAIN,
        .dup = MQTT_DUP,
        .id = MQTT_ID,
        .payload = (void *)message,
        .payloadlen = strlen(message)
    };

    int res = MQTTPublish(client, topic, &msg);
    if (res != 0) {
        printf("Error publishing message: %d\n", res);
    } else {
        printf("Message published: %s\n", message);
    }
}

int main(void) {
    mqtt_client_t client;
    mqtt_connect(&client, MQTT_BROKER_IP, MQTT_BROKER_PORT);

    while (1) {
        mqtt_publish(&client, MQTT_TOPIC, MQTT_MESSAGE);
        xtimer_sleep(5);
    }

    return 0;
}
