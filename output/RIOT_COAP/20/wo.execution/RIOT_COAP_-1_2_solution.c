#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "arpa/inet.h"
#include "mma8x5x_regs.h"
#include "net/gcoap.h"

int main(void) {
    const char *addr_str = "47.102.103.1";
    uint16_t port = 5683;
    const char *uri_path = "/coap/test";
    const char *payload = "Hello, COAP Cloud";

    // Create a UDP endpoint
    sock_udp_ep_t remote = { .family = AF_INET, .port = port };
    inet_pton(AF_INET, addr_str, &remote.addr.ipv4);

    // CoAP PDU initialization
    coap_pkt_t pdu;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, uri_path);

    // Set CoAP Payload
    pdu.payload_len = strlen(payload);
    memcpy(pdu.payload, payload, pdu.payload_len);

    // Send CoAP request
    ssize_t res = gcoap_req_send(buf, pdu.payload_len + pdu.hdr_len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        return 1; // Error
    }

    return 0; // Success
}