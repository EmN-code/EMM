
#include <stdio.h>
#include <string.h>
#include <xtimer.h>
#include <net/sock.h>
#include <net/mqtt.h>
#include <net/netif.h>
#include <net/ipv4/addr.h>

#define BROKER_IP "47.102.103.1"
#define BROKER_PORT 1883
#define TOPIC "state"
#define PAYLOAD_WORK "work"
#define PAYLOAD_NOT_WORK "not work"
#define INTERVAL (5 * 1000000U) // 5 seconds in microseconds

static sock_udp_t sock;
static mqtt_client_t client;
static char sendbuf[128];
static char readbuf[128];

static void publish_state(void)
{
    static bool state = false;
    const char *payload = state ? PAYLOAD_WORK : PAYLOAD_NOT_WORK;
    state = !state;

    mqtt_publish(&client, TOPIC, payload, strlen(payload), 0, 0);
}

int main(void)
{
    // Initialize network interface
    netif_t *netif = netif_iter(NULL);
    if (netif == NULL) {
        puts("No network interface found");
        return 1;
    }

    // Initialize MQTT client
    mqtt_client_init(&client, &sock, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

    // Connect to MQTT broker
    ipv4_addr_t broker_addr;
    inet_pton(AF_INET, BROKER_IP, &broker_addr);
    sock_udp_ep_t local = SOCK_IPV4_EP_ANY;
    sock_udp_ep_t remote = SOCK_IPV4_EP(broker_addr, BROKER_PORT);
    sock_udp_create(&sock, &local, &remote, 0);

    mqtt_connect(&client, "RIOT_MQTT_CLIENT", 60);

    // Publish state every 5 seconds
    while (1) {
        publish_state();
        xtimer_usleep(INTERVAL);
    }

    return 0;
}
