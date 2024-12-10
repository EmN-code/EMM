#include <stdio.h>
#include <arpa/inet.h>
#include <net/sock/udp.h>
#include <string.h>
#include <xtimer.h>
#include <net/gcoap.h>

#define COAP_SERVER_IP       "47.102.103.1"
#define COAP_SERVER_PORT     5683
#define COAP_URI_PATH        "/coap/test"
#define COAP_PAYLOAD         "Hello, COAP Cloud"

static char _resp_handler(coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)pdu;
    (void)remote;
    printf("Received response\n");
    return 0;
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = COAP_SERVER_PORT;

    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("inet_pton failed\n");
        return -1;
    }

    int len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, COAP_PAYLOAD, strlen(COAP_PAYLOAD));
    pdu.payload_len = strlen(COAP_PAYLOAD);

    ssize_t result;
    while (1) {
        result = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (result <= 0) {
            printf("gcoap_req_send failed, retrying\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("gcoap_req_send successfully\n");
            break;
        }
    }

    return 0;
}