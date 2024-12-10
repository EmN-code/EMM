
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include <net/netif.h>
#include <net/ipv4/addr.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_MESSAGE "work"
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_KEEPALIVE 60
#define PUBLISH_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_tcp_t mqtt_sock;
static mqtt_client_t mqtt_client;
static char sendbuf[128];
static char readbuf[128];

void mqtt_connect(void) {
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    int res = sock_tcp_connect(&mqtt_sock, &remote, &local, 0);
    if (res < 0) {
        printf("Failed to connect to MQTT broker\n");
        return;
    }

    mqtt_client_init(&mqtt_client, &mqtt_sock, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf), NULL);

    mqtt_connect_info_t conn_info = {
        .client_id = MQTT_CLIENT_ID,
        .keep_alive = MQTT_KEEPALIVE,
        .clean_session = 1,
        .will_topic = NULL,
        .will_message = NULL,
        .will_qos = 0,
        .will_retain = 0,
        .username = NULL,
        .password = NULL
    };

    res = MQTTPublish(&mqtt_client, MQTT_TOPIC, (MQTTMessage*)&conn_info);
    if (res != 0) {
        printf("Failed to connect to MQTT broker\n");
        return;
    }

    printf("Connected to MQTT broker\n");
}

void mqtt_publish(void) {
    MQTTMessage message = {
        .qos = MQTT_QOS,
        .retained = MQTT_RETAIN,
        .dup = 0,
        .payload = (void*)MQTT_MESSAGE,
        .payloadlen = strlen(MQTT_MESSAGE)
    };

    int res = MQTTPublish(&mqtt_client, MQTT_TOPIC, &message);
    if (res != 0) {
        printf("Failed to publish message\n");
    } else {
        printf("Published message: %s\n", MQTT_MESSAGE);
    }
}

int main(void) {
    netif_t *netif = netif_iter(NULL);
    if (netif == NULL) {
        printf("No network interface found\n");
        return 1;
    }

    printf("Waiting for IP address...\n");
    while (netif_addr_from_str(netif, "0.0.0.0") == NULL) {
        xtimer_sleep(1);
    }

    printf("IP address acquired\n");

    mqtt_connect();

    while (1) {
        mqtt_publish();
        xtimer_usleep(PUBLISH_INTERVAL);
    }

    return 0;
}
