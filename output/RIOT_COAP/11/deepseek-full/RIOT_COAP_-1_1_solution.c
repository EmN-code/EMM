
#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"
#include "xtimer.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_MESSAGE "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        puts("Request timed out");
    }
    else if (memo->state == GCOAP_MEMO_ERR) {
        puts("Request failed");
    }
    else {
        char *class_str = (coap_pkt_get_code_class(pdu) == COAP_CLASS_SUCCESS) ? "Success" : "Error";
        printf("Response Code: %s\n", class_str);
    }
}

int main(void)
{
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_SERVER_PORT };
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    // Initialize the remote endpoint with the server's IP address
    ipv4_addr_t server_addr;
    ipv4_addr_from_str(&server_addr, COAP_SERVER_IP);
    remote.addr.ipv4 = server_addr;

    // Initialize the CoAP packet
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    coap_pkt_set_payload(&pdu, (uint8_t *)COAP_MESSAGE, strlen(COAP_MESSAGE));

    // Send the CoAP request
    gcoap_req_send(&buf[0], coap_pkt_get_total_size(&pdu), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    // Keep the thread alive to handle the response
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
