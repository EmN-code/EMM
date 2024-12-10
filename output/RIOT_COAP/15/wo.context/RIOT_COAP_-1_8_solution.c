#include "xtimer.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include <stdio.h>
#include <string.h>

#define SERVER_IP     "47.102.103.1"
#define SERVER_PORT   5683
#define URI_PATH      "/coap/test"
#define PAYLOAD       "Hello, COAP Cloud"

static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
static ssize_t _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void) memo; // To avoid unused parameter warning
    (void) remote;
    
    if (pdu->hdr->code == COAP_CODE_204)
    {
        printf("Request acknowledged by the server\n");
    }
    else
    {
        printf("Server error\n");
    }
    return 0;
}

int main(void)
{
    ssize_t res;
    uint8_t payload[] = PAYLOAD;
    sock_udp_ep_t remote = { .family = AF_INET6, .port = SERVER_PORT };
    coap_pkt_t pdu;

    // Convert IP string to byte-represented IP address
    if (inet_pton(AF_INET6, SERVER_IP, &remote.addr.ipv6) != 1) {
        printf("Error: invalid IP address\n");
        return -1;
    }

    // Initialize CoAP message
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, URI_PATH);
    memcpy(pdu.payload, payload, sizeof(payload) - 1);
    gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    // Attempt network connection if required
    while (1) {
        res = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (res >= 0) {
            printf("Connected successfully\n");
            break; // Exit loop if connection is successful
        } else {
            printf("Connection failed, retrying in 1 second\n");
            xtimer_sleep(1);
        }
    }
    return 0;
}