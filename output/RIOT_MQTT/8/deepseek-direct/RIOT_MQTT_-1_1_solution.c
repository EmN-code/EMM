
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include <net/ipv6/addr.h>
#include <net/gnrc/netif.h>
#include <net/gnrc/netapi.h>
#include <net/gnrc/netreg.h>
#include <net/gnrc/ipv6.h>
#include <net/gnrc/udp.h>
#include <net/gnrc/tcp.h>

#define MQTT_BROKER_IP "47.102.103.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_TOPIC "device/state"
#define MQTT_CLIENT_ID "esp32_client"
#define MQTT_USERNAME NULL
#define MQTT_PASSWORD NULL
#define MQTT_QOS 0
#define MQTT_KEEPALIVE 60
#define MQTT_MESSAGE_INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static void publish_state(void) {
    static int state = 0;
    const char *states[] = {"not work", "work"};

    // Create a socket
    sock_tcp_t sock;
    sock_tcp_ep_t remote = { .family = AF_INET, .port = MQTT_BROKER_PORT };
    inet_pton(AF_INET, MQTT_BROKER_IP, &remote.addr.ipv4);

    // Connect to the MQTT broker
    if (sock_tcp_connect(&sock, &remote, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Error: could not connect to MQTT broker");
        return;
    }

    // MQTT connect packet
    mqtt_connect_t connect;
    connect.client_id = MQTT_CLIENT_ID;
    connect.username = MQTT_USERNAME;
    connect.password = MQTT_PASSWORD;
    connect.keep_alive = MQTT_KEEPALIVE;
    connect.clean_session = 1;

    // Send MQTT connect packet
    mqtt_connect(&sock, &connect);

    // MQTT publish packet
    mqtt_publish_t publish;
    publish.topic = MQTT_TOPIC;
    publish.payload = states[state];
    publish.payload_len = strlen(states[state]);
    publish.qos = MQTT_QOS;
    publish.retain = 0;

    // Send MQTT publish packet
    mqtt_publish(&sock, &publish);

    // Toggle state
    state = !state;

    // Close the socket
    sock_tcp_close(&sock);
}

int main(void) {
    // Initialize network interface
    gnrc_netif_t *netif = gnrc_netif_iter(NULL);
    if (netif == NULL) {
        puts("Error: no network interface found");
        return 1;
    }

    // Set up IP address
    ipv6_addr_t addr = IPV6_ADDR_UNSPECIFIED;
    gnrc_netapi_set(netif->pid, NETOPT_IPV6_ADDR, 0, &addr, sizeof(addr));

    // Main loop
    while (1) {
        publish_state();
        xtimer_usleep(MQTT_MESSAGE_INTERVAL);
    }

    return 0;
}
