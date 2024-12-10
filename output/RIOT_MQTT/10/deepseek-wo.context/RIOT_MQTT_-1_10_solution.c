
#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "net/ipv4/addr.h"
#include "net/sock/tcp.h"
#include "net/sock/util.h"
#include "net/mqtt.h"
#include "net/mqtt_client.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define CLIENT_ID "esp32_client"
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * 1000 * 1000U) /* 5 seconds in microseconds */

static sock_tcp_t sock;
static mqtt_client_t client;
static char sendbuf[1024];
static char readbuf[1024];

static int mqtt_read(void *arg, unsigned char *buf, int len, int timeout)
{
    (void)arg;
    return sock_tcp_read(&sock, buf, len, timeout);
}

static int mqtt_write(void *arg, unsigned char *buf, int len, int timeout)
{
    (void)arg;
    return sock_tcp_write(&sock, buf, len, timeout);
}

static void connect_to_broker(void)
{
    ipv4_addr_t server_addr;
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    inet_pton(AF_INET, SERVER_IP, &server_addr);
    remote.addr.ipv4 = server_addr;

    if (sock_tcp_connect(&sock, &remote, &local, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Error: could not connect to server");
        return;
    }

    mqtt_client_t client;
    mqtt_client_init(&client, &sock, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf), mqtt_read, mqtt_write);

    mqtt_connect_info_t conn_info = {
        .client_id = CLIENT_ID,
        .keep_alive = 60,
        .clean_session = 1,
    };

    if (mqtt_client_connect(&client, &conn_info) != MQTT_OK) {
        puts("Error: could not connect to MQTT broker");
        return;
    }

    puts("Connected to MQTT broker");
}

static void publish_state(void)
{
    mqtt_publish_info_t pub_info = {
        .qos = MQTT_QOS_0,
        .retain = 0,
        .topic = TOPIC,
        .payload = PAYLOAD,
        .payload_len = PAYLOAD_LEN,
    };

    if (mqtt_client_publish(&client, &pub_info) != MQTT_OK) {
        puts("Error: could not publish message");
    } else {
        puts("Message published");
    }
}

int main(void)
{
    connect_to_broker();

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
