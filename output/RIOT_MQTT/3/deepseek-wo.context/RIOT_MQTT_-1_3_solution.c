
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static sock_tcp_t sock;
static mqtt_client_t client;
static uint8_t sendbuf[1024];
static uint8_t readbuf[1024];

static void network_init(void)
{
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    inet_addr_from_str(remote.addr.ipv4, BROKER_IP);

    int res = sock_tcp_create(&sock, &local, &remote, 0);
    if (res < 0) {
        puts("Error creating TCP socket");
        return;
    }
}

static void mqtt_init(void)
{
    mqtt_client_init(&client);
    mqtt_connect_params_t connect_params = {
        .keep_alive = 60,
        .client_id = "riot_client",
        .clean_session = true,
        .will_topic = NULL,
        .will_message = NULL,
        .will_qos = 0,
        .will_retain = false,
        .username = NULL,
        .password = NULL,
    };

    int res = mqtt_connect(&client, &sock, &connect_params, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    if (res != MQTT_OK) {
        puts("Error connecting to MQTT broker");
        return;
    }
}

static void publish_state(void)
{
    mqtt_publish_params_t pub_params = {
        .topic = TOPIC,
        .message = PAYLOAD,
        .msg_size = strlen(PAYLOAD),
        .qos = 0,
        .retain = false,
    };

    int res = mqtt_publish(&client, &pub_params, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));
    if (res != MQTT_OK) {
        puts("Error publishing message");
    } else {
        puts("Message published successfully");
    }
}

int main(void)
{
    puts("Starting MQTT client example");

    network_init();
    mqtt_init();

    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
