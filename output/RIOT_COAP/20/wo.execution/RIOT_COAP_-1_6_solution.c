// The code implements a CoAP client on an ESP32 using the RIOT OS.
// It sends a POST request with the payload "Hello, COAP Cloud" to a CoAP server.
// The server is configured at the IP "47.102.103.1" and port 5683 with a URI path "/coap/test".

#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/socket.h>
#include <mutex.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <arpa/inet.h>
#include <event/thread.h>
#include <net/sock/udp.h>
#include <net/emcute.h>

#define EMQX_URI_PATH "/coap/test"
#define EMQX_SERVER_IP "47.102.103.1"

// Correctly define the buffer size
#ifndef CONFIG_GCOAP_PDU_BUF_SIZE
#define CONFIG_GCOAP_PDU_BUF_SIZE 256
#endif

void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    // Example response handler
    if (memo->state == GCOAP_MEMO_FIN) {
        printf("Response received\n");
    }
    else {
        printf("No response\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = 5683 };
    int result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);

    if (result != 1) {
        printf("Failed to convert IP address\n");
        return -1;
    }

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);
    const char *payload = "Hello, COAP Cloud";
    memcpy(pdu.payload, payload, strlen(payload));

    ssize_t len = pdu.payload - buf + strlen(payload);

    if (gcoap_req_send(&buf[0], len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        printf("Error sending CoAP request\n");
    }

    return 0;
}