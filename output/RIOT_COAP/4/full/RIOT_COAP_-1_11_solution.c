#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "string.h"
#include "paho_mqtt.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "msg.h"
#include "mutex.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "dht.h"
#include "assert.h"
#include "arpa/inet.h"
#include "ztimer.h"
#include "xtimer.h"
#include "MQTTClient.h"

#define COAP_REQUEST_METHOD COAP_METHOD_POST
#define COAP_PAYLOAD "Hello, COAP Cloud"
#define COAP_URI_PATH "/coap/test"
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_BUF_SIZE 128

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    if (pdu != NULL) {
        puts("Response received successfully");
    } else {
        puts("No response from server\n");
    }
}

int main(void) {
    uint8_t buf[COAP_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    
    memset(&remote, 0, sizeof(remote));
    remote.family = AF_INET;
    remote.port = SERVER_PORT;

    if (!inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4)) {
        puts("Error: unable to parse server IP");
        return -1;
    }

    ssize_t res = gcoap_req_init(&pdu, buf, COAP_BUF_SIZE, COAP_REQUEST_METHOD, COAP_URI_PATH);
    if (res < 0) {
        printf("Error in request initialization: %d\n", (int)res);
        return -1;
    }

    memcpy(pdu.payload, COAP_PAYLOAD, strlen(COAP_PAYLOAD));
    coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);

    res = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (res <= 0) {
        printf("Error sending request: %d\n", (int)res);
        return -1;
    }

    puts("CoAP request sent\n");
    return 0;
}