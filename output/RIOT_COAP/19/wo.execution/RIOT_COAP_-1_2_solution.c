#include "netinet/in.h"
#include "sys/socket.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "arpa/inet.h"
#include "net/emcute.h"

int main(void) {
    // Setting up destination endpoint
    sock_udp_ep_t remote;
    ipv6_addr_from_str((ipv6_addr_t *)&remote, "47.102.103.1");
    remote.port = 5683;

    // Buffer for creating CoAP requests
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    ssize_t bytes;

    // Initialize CoAP request with POST method
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");
    size_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    // Add payload "Hello, COAP Cloud"
    memcpy(pdu.payload, "Hello, COAP Cloud", 16);
    len += 16;
    coap_opt_set_length(&pdu, len);

    // Sending Coap Request
    bytes = gcoap_req_send(buf, len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);

    if (bytes <= 0) {
        // Error handling for failed send
        puts("Failed to send CoAP request");
    } else {
        puts("CoAP request sent");
    }
    return 0;
}