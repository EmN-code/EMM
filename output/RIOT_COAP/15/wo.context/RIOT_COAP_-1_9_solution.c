#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"
#define BUFFER_SIZE CONFIG_GCOAP_PDU_BUF_SIZE

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_RESP) {
        puts("Response received successfully!");
    }
    else {
        puts("No response received");
    }
}

int main(void) {
    uint8_t buf[BUFFER_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;

    remote.family = AF_INET;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    remote.port = SERVER_PORT;

    int rc;

    while (1) {
        rc = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
        if (rc != 1) {
            puts("Failed to convert IP address. Retrying...");
            sleep(1);
            continue;
        } else {
            puts("IP address converted successfully");
            break;
        }
    }

    while (1) {
        rc = gcoap_req_init(&pdu, buf, BUFFER_SIZE, COAP_METHOD_POST, URI_PATH);
        if (rc == 0) {
            puts("Failed to initialize CoAP request. Retrying...");
            sleep(1);
            continue;
        } else {
            puts("CoAP request initialized successfully");
            break;
        }
    }

    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);

    while (1) {
        rc = gcoap_req_send(buf, pdu.payload_len + pdu.hdr_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
        if (rc <= 0) {
            printf("Failed to send CoAP request. Retrying...\n");
            sleep(1);
            continue;
        } else {
            printf("CoAP request sent successfully\n");
            break;
        }
    }

    return 0;
}