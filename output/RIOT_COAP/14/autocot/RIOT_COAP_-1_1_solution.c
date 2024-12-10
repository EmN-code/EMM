#include <net/nanocoap.h>
#include <net/gcoap.h>
#include <stdio.h>
#include <string.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT CONFIG_GCOAP_PORT
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

static const sock_udp_ep_t server_endpoint = {
    .family = AF_INET,
    .port = SERVER_PORT,
    .addr.ipv6 = { 0 },  // This is a placeholder, use ipv6 address conversion if needed
};

gcoap_req_init(coap_pkt_t *pdu, uint8_t *buf, size_t len, unsigned code, const char *path);
void gcoap_req_send(uint8_t *buf, size_t len, const sock_udp_ep_t *remote, gcoap_req_handler_t resp_handler, void *resp_arg, unsigned flags);

void send_coap_request(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, MESSAGE, strlen(MESSAGE));
    pdu.payload_len = strlen(MESSAGE);

    if (gcoap_req_send(buf, gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH), &server_endpoint, NULL, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        puts("Error sending CoAP request");
    } else {
        puts("CoAP request sent successfully");
    }
}

int main(void) {
    puts("RIOT CoAP client started");
    send_coap_request();
    return 0;
}