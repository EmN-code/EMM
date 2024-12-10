
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt_client.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "state"
#define MQTT_MESSAGE "work"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_KEEPALIVE 60
#define MQTT_QOS 0
#define MQTT_RETAIN 0
#define MQTT_CLEAN_SESSION 1
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL

static sock_tcp_t sock;
static mqtt_client_t client;
static char sendbuf[1024];
static char readbuf[1024];

void mqtt_connect(void) {
    mqtt_client_init(&client);
    mqtt_client_set_server(&client, MQTT_BROKER_IP, MQTT_BROKER_PORT);
    mqtt_client_set_client_id(&client, MQTT_CLIENT_ID);
    mqtt_client_set_keep_alive(&client, MQTT_KEEPALIVE);
    mqtt_client_set_clean_session(&client, MQTT_CLEAN_SESSION);
    mqtt_client_set_username_password(&client, MQTT_USERNAME, MQTT_PASSWORD);

    int res = mqtt_client_connect(&client);
    if (res != 0) {
        printf("MQTT connection failed\n");
        return;
    }
    printf("MQTT connected\n");
}

void mqtt_publish(void) {
    mqtt_client_publish(&client, MQTT_TOPIC, MQTT_MESSAGE, strlen(MQTT_MESSAGE), MQTT_QOS, MQTT_RETAIN);
    printf("Published: %s\n", MQTT_MESSAGE);
}

int main(void) {
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    int res = sock_tcp_create(&sock, &local, &remote, SOCK_FLAGS_REUSE_EP);
    if (res < 0) {
        printf("Failed to create socket\n");
        return 1;
    }

    mqtt_connect();

    while (1) {
        mqtt_publish();
        xtimer_sleep(5);
    }

    sock_tcp_close(&sock);
    return 0;
}
