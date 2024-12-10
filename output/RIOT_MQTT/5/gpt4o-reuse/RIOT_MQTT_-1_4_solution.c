
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/ipv6/addr.h"
#include "thread.h"
#include "xtimer.h"

#define ADDRESS     "47.102.103.1"
#define PORT        1883
#define CLIENTID    "mqtt_riot_client"
#define TOPIC       "state"
#define PAYLOAD     "work"
#define QOS         1
#define TIMEOUT     10000L

static sock_udp_t sock;
static sock_udp_ep_t remote;

void publish_message(void)
{
    uint8_t buf[GCOAP_PDU_BUF_SIZE];
    gcoap_req_init(&pdu, buf, GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, TOPIC);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);

    ssize_t len = gcoap_req_send(&buf[0], pdu.payload_len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (len < 0) {
        printf("Failed to send message\n");
    } else {
        printf("Message sent to topic %s\n", TOPIC);
    }
}

int main(void)
{
    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
    local.port = GCOAP_PORT;
    remote.family = AF_INET;
    remote.port = PORT;
    inet_pton(AF_INET, ADDRESS, &remote.addr.ipv4);

    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        printf("Failed to create UDP socket\n");
        return 1;
    }

    while (1) {
        publish_message();
        xtimer_sleep(5);
    }

    sock_udp_close(&sock);
    return 0;
}
