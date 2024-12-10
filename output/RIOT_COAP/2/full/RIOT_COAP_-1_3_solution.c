#include "ztimer.h"
#include "xtimer.h"
#include "thread.h"
#include "timex.h"
#include "time_units.h"
#include "string.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mma8x5x_regs.h"
#include "MQTTClient.h"
#include "dht_params.h"
#include "arpa/inet.h"

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_CONTENT "Hello, COAP Cloud"
#define COAP_BUF_SIZE 128

void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (memo->state == GCOAP_MEMO_RESP) {
        printf("Response received!\n");
    } else {
        printf("No response received.\n");
    }
}

int send_coap_request(void) {
    uint8_t buf[COAP_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = SERVER_PORT };

    if (inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("Error: unable to parse server address\n");
        return -1;
    }

    size_t len = gcoap_req_init(&pdu, buf, COAP_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    ssize_t bytes_written = coap_put_block(&pdu, (uint8_t *)COAP_CONTENT, strlen(COAP_CONTENT), NULL);
    if (bytes_written < 0) {
        printf("Error: unable to finish packet\n");
        return -1;
    }
    len += bytes_written;

    if (gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) <= 0) {
        printf("Error: unable to send packet\n");
        return -1;
    }

    return 0;
}

int main(void) {
    msg_init_queue(NULL, 0);
    ztimer_sleep(ZTIMER_MSEC, 1000);

    while (1) {
        if (send_coap_request() == 0) {
            printf("CoAP request sent successfully\n");
        } else {
            printf("Failed to send CoAP request\n");
        }

        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return 0;
}