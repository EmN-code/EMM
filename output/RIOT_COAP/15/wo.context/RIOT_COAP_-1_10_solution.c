#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <string.h>
#include <stdio.h>

#define CONFIG_GCOAP_PDU_BUF_SIZE 128

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

static uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (pdu != NULL) {
        printf("Response received: %d\n", coap_get_code(pdu));
    }
    else {
        printf("No response received\n");
    }
}

int main(void) {
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_SERVER_PORT };
    int result = inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);

    while (result != 1) {
        printf("inet_pton failed\n");
        xtimer_sleep(1);
        result = inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4);
    }
    printf("inet_pton succeeded\n");

    coap_pkt_t pdu;
    ssize_t bytes;
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    size_t len = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    const char *data = "Hello, COAP Cloud";
    memcpy(pdu.payload, data, strlen(data));
    pdu.payload_len = strlen(data);

    while (1) {
        bytes = gcoap_req_send(buf, len + pdu.payload_len, &remote, _resp_handler, NULL);

        if (bytes <= 0) {
            printf("Failed to send request\n");
            xtimer_sleep(1);
            continue;
        }
        else {
            printf("CoAP request sent successfully\n");
            break;
        }
    }

    return 0;
}