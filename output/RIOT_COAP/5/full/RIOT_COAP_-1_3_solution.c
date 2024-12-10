#include "thread.h"
#include "string.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "msg.h"
#include "MQTTClient.h"
#include "mma8x5x_regs.h"
#include "dht_params.h"
#include "arpa/inet.h"
#include "net/emcute.h"

#define EMQX_SERVER_IP      "47.102.103.1"
#define EMQX_PORT          5683
#define EMQX_URI_PATH      "/coap/test"
#define PAYLOAD            "Hello, COAP Cloud"
msg_t _main_msg_queue[8];

void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)pdu;
    (void)remote;
    printf("Response received\n");
}

int main(void) {
    msg_init_queue(_main_msg_queue, ARRAY_SIZE(_main_msg_queue));
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote = { .family = AF_INET, .port = EMQX_PORT };
    coap_pkt_t pdu;

    // Setting up remote address using inet_pton
    inet_pton(AF_INET, EMQX_SERVER_IP, &remote.addr.ipv4);

    // Initialize the CoAP request
    int len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);

    // Set the payload for the request
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    len += strlen(PAYLOAD);

    // Send the CoAP request
    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        puts("Error sending CoAP request");
        return 1;
    }

    puts("CoAP request sent successfully");
    return 0;
}