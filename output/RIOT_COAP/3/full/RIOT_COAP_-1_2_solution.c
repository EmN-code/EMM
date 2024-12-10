#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include <stdio.h>
#include <string.h>

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

// Handler for the response
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu,
                          const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("gcoap: timeout" "\n");
        return;
    }

    char *class_str = (coap_get_code_class(pdu) == COAP_CLASS_SUCCESS) ?
                          "Success": "Error";
    printf("gcoap: response %s, code %1u.%02u\n", class_str,
           coap_get_code_class(pdu), coap_get_code_detail(pdu));
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET6, .netif = SOCK_ADDR_ANY_NETIF,
                             .port = SERVER_PORT };

    // Parse server address
    if (sock_udp_str2ep(&remote, SERVER_ADDR) < 0) {
        printf("Error: unable to parse destination address\n");
        return -1;
    }

    // Initialize CoAP request
    ssize_t len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
    if (len < 0) {
        printf("Error: unable to initialize CoAP request\n");
        return -1;
    }

    // Set payload
    if (!coap_hdr_set_payload(&pdu, (uint8_t *)MESSAGE, strlen(MESSAGE))) {
        printf("Error: unable to add payload\n");
        return -1;
    }

    // Send CoAP request
    len = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL,
                         GCOAP_SOCKET_TYPE_UDP);
    if (len <= 0) {
        printf("Error: unable to send CoAP request\n");
        return -1;
    }

    printf("CoAP message sent successfully\n");
    
    return 0;
}