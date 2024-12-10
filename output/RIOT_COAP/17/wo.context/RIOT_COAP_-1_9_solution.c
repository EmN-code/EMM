#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <net/ipv6/addr.h>
#include <xtimer.h>
#include <gcoap.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE (128)

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    const char *server_ip = "47.102.103.1";
    const char *uri_path = "/coap/test";
    const char *payload = "Hello, COAP Cloud";

    // Initialize remote endpoint with server address
    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = CONFIG_GCOAP_PORT;
    if (inet_pton(AF_INET, server_ip, &remote.addr.ipv4) != 1) {
        printf("Error setting up server IP address\n");
        return 1;
    }

    // Initialize CoAP request
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, uri_path);
    memcpy(pdu.payload, payload, strlen(payload));

    // Send CoAP request
    int res = gcoap_req_send(buf, pdu.hdr_len + strlen(payload), &remote,
                             NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        printf("Error sending CoAP request\n");
        return 1;
    }

    printf("CoAP POST request sent to %s\n", server_ip);
    return 0;
}