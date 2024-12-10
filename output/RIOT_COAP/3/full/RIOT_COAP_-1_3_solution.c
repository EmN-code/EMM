#include "thread.h"
#include "string.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "msg.h"
#include "esp_wifi.h"
#include "dht_params.h"
#include "arpa/inet.h"
#include "MQTTClient.h"
#include "mma8x5x_regs.h"
#include "net/gcoap.h"

#define SERVER_ADDR "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define BUFFER_SIZE 128

static ssize_t _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;
    (void)remote;
    if (pdu != NULL) {
        /* Response received */
        printf("Received response: %u\n", coap_get_code(pdu));
    }
    else {
        /* Timeout or error */
        printf("No response received\n");
    }
    return 0;
}

int main(void) {
    uint8_t buf[BUFFER_SIZE];
    coap_pkt_t pdu;
    ssize_t res;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };
    inet_pton(AF_INET, SERVER_ADDR, &remote.addr.ipv4);

    gcoap_req_init(&pdu, buf, BUFFER_SIZE, COAP_METHOD_POST, URI_PATH);
    const char *payload = "Hello, COAP Cloud";
    size_t paylen = strlen(payload);
    coap_hdr_set_payload(&pdu, (uint8_t *)payload, paylen);

    res = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        printf("Failed to send request\n");
        return 1;
    }

    /* Wait for response */
    msg_t msg;
    msg_receive(&msg);

    return 0;
}