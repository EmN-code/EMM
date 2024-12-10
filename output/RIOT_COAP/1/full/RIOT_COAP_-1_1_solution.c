#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/sock/udp.h"

int main(void) {
    coap_pkt_t pdu;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    const char *payload = "Hello, COAP Cloud";
    sock_udp_ep_t remote = { .family = AF_INET6,
                             .port = CONFIG_GCOAP_PORT };
    
    // Set the server address
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr, "47.102.103.1");

    if (gcoap_req_init(&pdu, &buf[0], CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test") < 0) {
        puts("Failed to initialize CoAP request.");
        return 1;
    }

    // Copy payload into the request
    memcpy(pdu.payload, payload, strlen(payload));
    pdu.payload_len = strlen(payload);

    if (gcoap_req_send(&buf[0], pdu.payload_len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        puts("Failed to send CoAP request.");
        return 1;
    }

    puts("CoAP request sent successfully.");
    return 0;
}
