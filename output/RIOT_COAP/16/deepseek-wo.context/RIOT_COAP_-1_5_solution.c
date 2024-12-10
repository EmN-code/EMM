
#include <stdio.h>
#include <string.h>
#include "net/gcoap.h"
#include "net/ipv4/addr.h"
#include "net/sock/udp.h"
#include "thread.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static char coap_stack[THREAD_STACKSIZE_DEFAULT];
static msg_t coap_msg_queue[8];

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote)
{
    (void)memo;
    (void)remote;

    if (pdu->payload_len) {
        printf("Received CoAP response: %.*s\n", (int)pdu->payload_len, (char *)pdu->payload);
    }
}

static void *coap_thread(void *arg)
{
    (void)arg;

    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;

    while (1) {
        gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);
        memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
        pdu.payload_len = strlen(PAYLOAD);

        gcoap_req_send(buf, pdu.payload - buf, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

        thread_sleep(1000000); // Sleep for 1 second
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(coap_msg_queue, sizeof(coap_msg_queue) / sizeof(coap_msg_queue[0]));

    thread_create(coap_stack, sizeof(coap_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, coap_thread, NULL, "coap_thread");

    return 0;
}
