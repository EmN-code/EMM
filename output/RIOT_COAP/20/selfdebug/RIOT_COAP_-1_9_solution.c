#include <string.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include "shell.h"

// Define constants for our CoAP request
#define COAP_SERVER_IP        "47.102.103.1"
#define COAP_SERVER_PORT      5683
#define COAP_URI_PATH         "/coap/test"
#define PAYLOAD               "Hello, COAP Cloud"

uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];  // Buffer for CoAP PDU

gcoap_req_init(coap_pkt_t *pdu, uint8_t *buf, size_t len, unsigned code, const char *path);

int main(void) {
    // Initialize endpoint
    sock_udp_ep_t remote = { .port = COAP_SERVER_PORT };
    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);

    // Initialize CoAP request
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    ssize_t len = coap_pkt_build_pdu(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, PAYLOAD, strlen(PAYLOAD));

    // Send the request
    gcoap_req_send(buf, len, &remote, NULL, NULL, GCOAP_SOCKET_TYPE_UDP);

    // Run the shell for command execution
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}