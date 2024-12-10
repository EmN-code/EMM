#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include "net/sock/udp.h"
#include "net/gcoap.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"

static ssize_t _resp_handler(coap_pkt_t *pdu, void *context)
{
    printf("Response received\n");
    return 0;
}

int main(void)
{
    printf("Starting CoAP client\n");

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote;
    coap_pkt_t pdu;
    int result;

    remote.family = AF_INET;
    remote.port = SERVER_PORT;
    result = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (result != 1) {
        printf("Error: unable to convert IP\n");
        return -1;
    }

    while (1) {
        result = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
        if (result < 0) {
            printf("Failed to initialize CoAP request\n");
            xtimer_sleep(1);
            continue;
        }

        size_t payload_len = snprintf((char *)pdu.payload, pdu.payload_len, "Hello, COAP Cloud");
        gcoap_req_send(buf, pdu.payload - buf + payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

        xtimer_sleep(10);
    }

    return 0;
}