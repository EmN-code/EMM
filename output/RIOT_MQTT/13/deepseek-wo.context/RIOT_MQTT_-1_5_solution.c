
#include <stdio.h>
#include <xtimer.h>
#include <net/sock/tcp.h>
#include <net/mqtt.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define INTERVAL (5 * 1000000U)  // 5 seconds in microseconds

static void connect_to_broker(sock_tcp_t *sock) {
    sock_tcp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = BROKER_PORT };
    inet_addr_from_str(remote.addr.ipv4, BROKER_IP);

    if (sock_tcp_connect(sock, &remote, &local, 0) < 0) {
        puts("Error: could not connect to broker");
        return;
    }
    puts("Connected to broker");
}

static void publish_message(sock_tcp_t *sock) {
    mqtt_publish_t pubmsg = {
        .topic = TOPIC,
        .payload = PAYLOAD,
        .payload_len = PAYLOAD_LEN,
        .qos = MQTT_QOS_0,
        .retain = 0,
        .dup = 0
    };

    if (mqtt_publish(sock, &pubmsg) < 0) {
        puts("Error: could not publish message");
    } else {
        puts("Message published");
    }
}

int main(void) {
    sock_tcp_t sock;
    sock_tcp_init(&sock);

    connect_to_broker(&sock);

    while (1) {
        publish_message(&sock);
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
