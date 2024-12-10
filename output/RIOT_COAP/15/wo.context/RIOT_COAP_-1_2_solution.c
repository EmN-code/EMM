#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/ipv6/addr.h>

#define COAP_SERVER_IP      "47.102.103.1"
#define COAP_SERVER_PORT    5683
#define COAP_URI_PATH       "/coap/test"
#define WAIT_INTERVAL       (1 * US_PER_SEC)

static ssize_t _resp_handler(coap_pkt_t* pdu, uint8_t *buf, size_t len) {
    printf("Response received\n");
    if(pdu->hdr->code == COAP_CODE_205) {
        printf("Response Payload: %s\n", (char *)pdu->payload);
    }
    return 0;
}

int gcoap_post_request(const char* addr_str, uint16_t port, const char* path) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, path);
    uint8_t *payload = (uint8_t *)"Hello, COAP Cloud";
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    memcpy(pdu.payload, payload, strlen((char *)payload));
    pdu.payload_len = strlen((char *)payload);
    sock_udp_ep_t remote = { .family = AF_INET6, .netif = SOCK_ADDR_ANY_NETIF };
    inet_pton(AF_INET, addr_str, &remote.addr.ipv4);
    remote.port = port;

    ssize_t result;
    while (1) {
        result = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (result < 0) {
            printf("Failed to send request, retrying...\n");
            xtimer_usleep(WAIT_INTERVAL);
            continue;
        }
        else {
            printf("Request sent successfully\n");
            break;
        }
    }

    return result;
}

int main(void) {
    puts("CoAP Client with error handling and reconnection logic");
    gcoap_post_request(COAP_SERVER_IP, COAP_SERVER_PORT, COAP_URI_PATH);
    return 0;
}