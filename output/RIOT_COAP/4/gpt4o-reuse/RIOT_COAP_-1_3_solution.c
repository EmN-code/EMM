
#include <stdio.h>
#include <netinet/in.h>
#include "gcoap.h"
#include "net/sock/udp.h"

/*
 * The response handler
 */ 
static void
message_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) 
{
    unsigned char* data;
    size_t data_len;

    if (coap_get_data(pdu, &data_len, &data)) {
        printf("Received: %s\n", data);
    }
}

int main(void)
{
    sock_udp_ep_t local = SOCK_UDP_EP_ANY;
    sock_udp_ep_t remote = { .family = AF_INET, .port = 5683, .addr = { .ipv4 = { .u8 = { 47, 102, 103, 1 } } } };
    sock_udp_t sock;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    /* Initialize the CoAP socket */
    if (sock_udp_create(&sock, &local, &remote, 0) < 0) {
        puts("Error creating UDP socket");
        return 1;
    }

    /* Prepare the request */
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, "/coap/test");
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    coap_add_data(&pdu, 15, (uint8_t *)"Hello, COAP Cloud");

    /* Send the request */
    ssize_t bytes_sent = sock_udp_send(&sock, pdu.hdr, pdu.payload - pdu.hdr, &remote);
    if (bytes_sent < 0) {
        puts("Error sending CoAP request");
        return 1;
    }

    /* Wait for the response */
    while (1) {
        sock_udp_ep_t remote_ep;
        ssize_t bytes_received = sock_udp_recv(&sock, buf, sizeof(buf), SOCK_NO_TIMEOUT, &remote_ep);
        if (bytes_received < 0) {
            puts("Error receiving CoAP response");
            return 1;
        }
        if (bytes_received > 0) {
            coap_parse(&pdu, buf, bytes_received);
            message_handler(NULL, &pdu, &remote_ep);
            break;
        }
    }

    /* Close the socket */
    sock_udp_close(&sock);

    return 0;
}
