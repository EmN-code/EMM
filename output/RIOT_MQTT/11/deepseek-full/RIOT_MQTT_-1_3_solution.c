
#include "net/emcute.h"
#include "net/sock/udp.h"
#include "net/af.h"
#include "net/sock/util.h"
#include "net/gcoap.h"
#include "netinet/in.h"
#include "paho_mqtt.h"
#include "sys/socket.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define QOS 0
#define RETAIN 0

static sock_udp_t sock;
static sock_udp_ep_t server = SOCK_IPV4_EP_ANY;

static void _on_publish(void *arg, err_t err)
{
    (void)arg;
    if (err != ERR_OK) {
        puts("Error publishing message");
    } else {
        puts("Message published");
    }
}

int main(void)
{
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    local.port = 12345;

    server.port = SERVER_PORT;
    server.family = AF_INET;
    server.netif = SOCK_ADDR_ANY_NETIF;

    if (sock_udp_create(&sock, &local, &server, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    struct mqtt_client client;
    uint8_t sendbuf[1024];
    uint8_t recvbuf[1024];
    mqtt_client_config_t config = {
        .host = SERVER_ADDR,
        .port = SERVER_PORT,
        .client_id = "riot_client",
        .username = NULL,
        .password = NULL,
        .keep_alive = 60,
        .clean_session = 1,
        .sendbuf = sendbuf,
        .recvbuf = recvbuf,
        .sendbuf_size = sizeof(sendbuf),
        .recvbuf_size = sizeof(recvbuf),
    };

    mqtt_client_init(&client, &config);

    if (mqtt_client_connect(&client) != ERR_OK) {
        puts("Error connecting to MQTT broker");
        return 1;
    }

    while (1) {
        mqtt_client_publish(&client, TOPIC, PAYLOAD, sizeof(PAYLOAD) - 1, QOS, RETAIN, _on_publish, NULL);
        xtimer_sleep(5);
    }

    mqtt_client_disconnect(&client);
    mqtt_client_destroy(&client);
    sock_udp_close(&sock);

    return 0;
}
