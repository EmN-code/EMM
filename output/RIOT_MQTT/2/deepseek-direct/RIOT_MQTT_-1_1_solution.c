
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
#define MQTT_WILL_TOPIC NULL
#define MQTT_WILL_MESSAGE NULL
#define MQTT_WILL_QOS 0
#define MQTT_WILL_RETAIN 0
#define MQTT_RETAIN 0

#define PUBLISH_INTERVAL (5 * 1000 * 1000U) /* 5 seconds in microseconds */

static void _mqtt_connect(sock_tcp_t *sock)
{
    mqtt_connect_t conn;
    memset(&conn, 0, sizeof(conn));
    conn.client_id = MQTT_CLIENT_ID;
    conn.username = MQTT_USERNAME;
    conn.password = MQTT_PASSWORD;
    conn.keep_alive = MQTT_KEEPALIVE;
    conn.will_topic = MQTT_WILL_TOPIC;
    conn.will_message = MQTT_WILL_MESSAGE;
    conn.will_qos = MQTT_WILL_QOS;
    conn.will_retain = MQTT_WILL_RETAIN;

    mqtt_connect(sock, &conn);
}

static void _mqtt_publish(sock_tcp_t *sock, const char *topic, const char *message)
{
    mqtt_publish_t pub;
    memset(&pub, 0, sizeof(pub));
    pub.topic = topic;
    pub.message = message;
    pub.qos = MQTT_QOS;
    pub.retain = MQTT_RETAIN;

    mqtt_publish(sock, &pub);
}

int main(void)
{
    sock_tcp_t sock;
    sock_tcp_ep_t remote;
    xtimer_ticks32_t last_wakeup = xtimer_now();

    /* Initialize network interface */
    gnrc_netif_t *netif = gnrc_netif_iter(NULL);
    if (!netif) {
        puts("No network interface found");
        return 1;
    }

    /* Set up remote endpoint */
    remote.family = AF_INET;
    remote.port = MQTT_BROKER_PORT;
    if (ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, MQTT_BROKER_IP) == NULL) {
        puts("Error parsing broker IP address");
        return 1;
    }

    /* Connect to MQTT broker */
    if (sock_tcp_connect(&sock, &remote, SOCK_FLAGS_REUSE_EP) < 0) {
        puts("Error connecting to MQTT broker");
        return 1;
    }

    /* Connect to MQTT broker */
    _mqtt_connect(&sock);

    while (1) {
        /* Publish state */
        _mqtt_publish(&sock, MQTT_TOPIC, "work");

        /* Wait for 5 seconds */
        xtimer_periodic_wakeup(&last_wakeup, PUBLISH_INTERVAL);
    }

    /* Close the socket */
    sock_tcp_close(&sock);

    return 0;
}
