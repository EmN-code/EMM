#include <string.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>

#define COAP_METHOD_POST 0.02    /* CoAP POST method code */
#define EMQX_URI_PATH "/coap/test"

/* Length of the buffer used for CoAP PDU */
#define CONFIG_GCOAP_PDU_BUF_SIZE 128

/* Define the address of the CoAP server */
#define COAP_SERVER_IP "47.102.103.1"

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET6, .port = CONFIG_GCOAP_PORT };
    coap_pkt_t pdu;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

    /* Convert IPv4 address into network byte order and store in remote endpoint */
    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);

    /* Initialize the CoAP PDU */
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, EMQX_URI_PATH);

    /* Set the payload to "Hello, COAP Cloud" */
    memcpy(pdu.payload, "Hello, COAP Cloud", 17);
    pdu.payload_len = 17;

    /* Send the CoAP request */
    gcoap_req_send(&buf[0], pdu.payload_len + coap_hdr_len(pdu), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    return 0;
}

void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    /* Handle the response from the server, if needed */
    if (pdu == NULL) {
        puts("No response from server");
    } else {
        /* Print response payload */
        printf("Response: %.*s\n", (int)pdu->payload_len, (char *)pdu->payload);
    }
}