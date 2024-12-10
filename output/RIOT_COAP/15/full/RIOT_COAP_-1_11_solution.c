#include <ztimer.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <assert.h>
#include <err.h>
#include <sys/socket.h>
#include <unistd.h>
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/gcoap.h"

#define COAP_SERVER_IP      "47.102.103.1"
#define COAP_SERVER_PORT    5683
#define COAP_URI_PATH       "/coap/test"
#define COAP_MESSAGE        "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    // Handle the response here. For simplicity, just print that a response was received.
    (void)memo;
    (void)pdu;
    (void)remote;
    printf("Response received\n");
}

int main(void)
{
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    ssize_t result;

    inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = COAP_SERVER_PORT;

    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, COAP_MESSAGE, sizeof(COAP_MESSAGE));
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    while (1) {
        result = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

        if (result < 0) {
            printf("Message sending failed, retrying...\n");
            ztimer_sleep(ZTIMER_MSEC, 1000); // Sleep for 1 second before retrying
            continue;
        } else {
            printf("Message sent successfully\n");
            break;
        }
    }

    return 0;
}