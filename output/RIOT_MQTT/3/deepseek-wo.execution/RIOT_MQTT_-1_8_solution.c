
#include <net/sock.h>
#include <net/sock/util.h>
#include <evtimer_mbox.h>
#include <mutex.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <thread.h>
#include <stdatomic.h>
#include <time_units.h>
#include <timex.h>
#include <kernel_defines.h>
#include <net/gcoap.h>
#include <evtimer_msg.h>
#include <msg.h>
#include <dht_params.h>
#include <arpa/inet.h>
#include <net/emcute.h>
#include <xtimer.h>
#include <dht.h>
#include <ztimer.h>
#include <stddef.h>
#include <net/sock/udp.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 1883
#define TOPIC "state"
#define PAYLOAD "work"
#define PAYLOAD_LEN 4
#define KEEP_ALIVE_INTERVAL 60
#define CLEAN_SESSION true
#define MQTT_VERSION 3

static sock_tcp_t _sock;

static int _connect(void)
{
    sock_udp_ep_t remote = SOCK_IPV4_EP_ANY;
    remote.port = SERVER_PORT;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    if (sock_tcp_connect(&_sock, &remote, SOCK_FLAGS_REUSE_EP) < 0) {
        printf("Error: could not connect to server\n");
        return -1;
    }

    return 0;
}

static int _publish(void)
{
    uint8_t buf[128];
    size_t len = 0;

    // Construct MQTT PUBLISH packet
    // Fixed header
    buf[len++] = 0x30; // MQTT Control Packet type (PUBLISH) and flags
    buf[len++] = 0x00; // Remaining length (to be filled later)

    // Variable header
    // Topic name
    const char *topic = TOPIC;
    uint16_t topic_len = strlen(topic);
    buf[len++] = (topic_len >> 8) & 0xFF;
    buf[len++] = topic_len & 0xFF;
    memcpy(&buf[len], topic, topic_len);
    len += topic_len;

    // Payload
    const char *payload = PAYLOAD;
    uint16_t payload_len = PAYLOAD_LEN;
    memcpy(&buf[len], payload, payload_len);
    len += payload_len;

    // Update remaining length
    buf[1] = len - 2;

    // Send the packet
    if (sock_tcp_send(&_sock, buf, len, 0) < 0) {
        printf("Error: could not send data\n");
        return -1;
    }

    return 0;
}

int main(void)
{
    if (_connect() < 0) {
        return -1;
    }

    while (1) {
        if (_publish() < 0) {
            break;
        }
        xtimer_sleep(5);
    }

    sock_tcp_close(&_sock);
    return 0;
}
