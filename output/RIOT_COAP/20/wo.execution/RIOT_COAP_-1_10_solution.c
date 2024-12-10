#include <netinet/in.h>
#include <net/sock/udp.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <sched.h>
#include <stdlib.h>
#include <net/gcoap.h>
#include <net/sock/util.h>
#include <sys/socket.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

#define COAP_PORT 5683

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu,
                          const sock_udp_ep_t *remote) {
    /* Handle the incoming response here */
    (void)memo;
    (void)pdu;
    (void)remote;

    printf("Response received\n");
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_PORT };
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    const char *payload = "Hello, COAP Cloud";

    if (inet_pton(AF_INET, "47.102.103.1", &remote.addr.ipv4) != 1) {
        printf("Error: Could not convert IP address\n");
        return 1;
    }

    ssize_t len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, "/coap/test");
    if (len < 0) {
        printf("Error: could not initialize pdu\n");
        return 1;
    }

    memcpy(pdu.payload, payload, strlen(payload));
    gcoap_req_send(buf, len + strlen(payload), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    printf("CoAP request sent\n");
    return 0;
}