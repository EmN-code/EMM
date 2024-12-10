#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <timex.h> 
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h> 
#include <stddef.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <string.h>
#include <netinet/in.h>
#include <net/sock/util.h> 
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <msg.h>
#include <mutex.h>
#include <kernel_defines.h>
#include <arpa/inet.h>

#define COAP_URI_PATH "/coap/test"
#define EMQX_SERVER_IP "47.102.103.1"
#define PAYLOAD "Hello, COAP Cloud"

gcoap_request_memo_t memo;

sock_udp_ep_t remote = {
    .family = AF_INET,
    .port = 5683
};

static void _resp_handler(const gcoap_request_memo_t *memo,
                          coap_pkt_t *pdu,
                          const sock_udp_ep_t *remote)
{
    (void) remote;
    (void) memo;
    printf("Response received: %u\n", coap_get_code_class(pdu));
}

int main(void)
{
    int result;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    /* Convert the server IP address */
    result = inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);
    if (result <= 0) {
        printf("Error: unable to encode IP address.\n");
        return -1;
    }

    /* Initialize a CoAP pkt with the given payload */
    int len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    if (len < 0) {
        printf("Error initializing request\n");
        return -1;
    }

    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len += strlen(PAYLOAD);

    /* Send the CoAP request */
    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Error sending request\n");
        return -1;
    }

    printf("Request sent successfully\n");
    ztimer_sleep(ZTIMER_SEC, 5);

    return 0;
}