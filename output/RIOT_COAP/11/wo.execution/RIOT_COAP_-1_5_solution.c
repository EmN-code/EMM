#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "sys/socket.h"
#include "unistd.h"
#include "time_units.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "mutex.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "arpa/inet.h"

#define SERVER_IP       "47.102.103.1"
#define SERVER_PORT     5683
#define URI_PATH        "/coap/test"
#define BUFFER_SIZE     128

static void _resp_handler(unsigned req_state, coap_pkt_t* pdu, uint8_t *buf, size_t len) {
    if (req_state == GCOAP_MEMO_TIMEOUT) {
        printf("Error: request timed out\n");
        return;
    }

    printf("Response received\n");
    printf("%.*s\n", (int)len, (char *)buf);
}

int main(void) {
    uint8_t buf[BUFFER_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    size_t len;

    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = SERVER_PORT;

    int res = inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    if (res != 1) {
        printf("Error: unable to parse IP address\n");
        return -1;
    }

    len = gcoap_req_init(&pdu, buf, BUFFER_SIZE, COAP_METHOD_POST, URI_PATH);
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    strcpy((char *)pdu.payload, "Hello, COAP Cloud");
    pdu.payload_len = strlen("Hello, COAP Cloud");

    ssize_t bytes_sent = gcoap_req_send(buf, len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (bytes_sent <= 0) {
        printf("Error: message not sent\n");
        return -1;
    }

    return 0;
}