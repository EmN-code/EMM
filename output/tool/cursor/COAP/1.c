#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "net/ipv4/addr.h"
#include "net/coap.h"
#include "xtimer.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static sock_udp_ep_t remote = {
    .family = AF_INET,
    .port = SERVER_PORT
};

int main(void)
{
    printf("RIOT CoAP client example\n");

    // Resolve the server address
    if (ipv4_addr_from_str((ipv4_addr_t *)&remote.addr.ipv4, SERVER_ADDR) == NULL) {
        printf("Error: unable to parse destination address\n");
        return 1;
    }

    // Create a UDP socket
    sock_udp_t sock;
    if (sock_udp_create(&sock, NULL, &remote, 0) < 0) {
        printf("Error: unable to create UDP socket\n");
        return 1;
    }

    // Prepare CoAP packet
    uint8_t buf[128];
    coap_pkt_t pdu;
    ssize_t len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    if (len < 0) {
        printf("Error: unable to initialize CoAP request\n");
        return 1;
    }

    // Add payload
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len += coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    // Send CoAP request
    if (sock_udp_send(&sock, buf, len, &remote) < 0) {
        printf("Error: unable to send CoAP request\n");
        return 1;
    }

    printf("CoAP request sent\n");

    // Close the socket
    sock_udp_close(&sock);

    return 0;
}