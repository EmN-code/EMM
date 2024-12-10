#include <stdio.h>
#include <shell.h>
#include <net/gcoap.h>
#include <net/gnrc.h>
#include <xtimer.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT "5683"
#define URI_PATH "/coap/test"

static gcoap_req_t coap_req;

static void send_coap_post(void) {
    char *uri = URI_PATH;
    coap_pkt_t pdu;
    uint8_t buf[128];
    sock_udp_ep_t remote = {
        .family = AF_INET6,
        .addr.ipv6 = {0},
        .port = atoi(SERVER_PORT)
    };
    
    // Parse the server address
    ipv6_addr_from_str((ipv6_addr_t *)remote.addr.ipv6, "::FFFF:" SERVER_ADDR);

    // Initialize the CoAP PDU
    gcoap_req_init(&coap_req);
    coap_pkt_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, uri);

    // Set the payload
    const char* payload = "Hello, COAP Cloud";
    coap_pkt_append_payload_marker(&pdu);
    coap_pkt_append_payload(&pdu, (uint8_t*)payload, strlen(payload));

    // Send the CoAP POST request
    gcoap_req_send(&coap_req, &pdu, &remote);
    printf("CoAP POST request sent to %s on port %s with URI path %s\n", SERVER_ADDR, SERVER_PORT, URI_PATH);
}

int main(void) {
    send_coap_post();
    return 0;
}