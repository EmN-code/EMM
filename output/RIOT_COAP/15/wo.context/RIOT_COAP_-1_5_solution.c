#include <xtimer.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/sock.h>
#include <net/ipv4/addr.h>

#define COAP_PORT 5683
#define COAP_SERVER "47.102.103.1"
#define COAP_URI_PATH "/coap/test"
#define COAP_MSG "Hello, COAP Cloud"
#define COAP_BUFFER_SIZE CONFIG_GCOAP_PDU_BUF_SIZE

static void _response_handler(unsigned req_state, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (req_state == GCOAP_MEMO_RETRANSMIT) {
        printf("Retransmit PDU\n");
    }
    else {
        printf("Response received\n");
    }
}

int main(void) {
    uint8_t buf[COAP_BUFFER_SIZE];
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_PORT };
    coap_pkt_t pdu;
    int res;

    res = inet_pton(AF_INET, COAP_SERVER, &remote.addr.ipv4);
    while (res != 1) {
        puts("Invalid server address format, retrying...");
        xtimer_sleep(1);
        res = inet_pton(AF_INET, COAP_SERVER, &remote.addr.ipv4);
    }

    printf("Server address valid\n");

    gcoap_req_init(&pdu, buf, COAP_BUFFER_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, COAP_MSG, strlen(COAP_MSG));
    pdu.payload_len = strlen(COAP_MSG);

    while (gcoap_req_send(buf, pdu.payload_len, &remote, _response_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        puts("Failed to send request, retrying...");
        xtimer_sleep(1);
    }

    puts("CoAP POST request sent successfully");
    return 0;
}