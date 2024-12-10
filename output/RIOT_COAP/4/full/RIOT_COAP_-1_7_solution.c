#include <ztimer.h>
#include <xtimer.h>
#include <thread.h>
#include <timex.h>
#include <string.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <msg.h>
#include <mutex.h>
#include <arpa/inet.h>
#include <assert.h>
#include <dht_params.h>
#include <mma8x5x.h>
#include <paho_mqtt.h>
#include <dht.h>
#include <mqttclient.h>
#include <net/emcute.h>
#include <mma8x5x_regs.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static sock_udp_ep_t remote;
static char _buf[CONFIG_GCOAP_PDU_BUF_SIZE];
static ssize_t _resp_handler(void *context, const gcoap_pkt_t *pkt, coap_reply_status_t status, uint32_t tok){
    (void)context;
    if (status == GCOAP_MEMO_COMPLETE || status == GCOAP_MEMO_TIMEOUT) {
        printf("Response received or timeout occurred\n");
    }
    return 0;
}

static void send_coap_request(void) {
    coap_pkt_t pdu;
    inet_pton(AF_INET, SERVER_IP, &remote.addr.ipv4);
    remote.family = AF_INET;
    remote.port = SERVER_PORT;

    size_t len = gcoap_req_init(&pdu, (uint8_t *)_buf, CONFIG_GCOAP_PDU_BUF_SIZE, COAP_METHOD_POST, COAP_URI_PATH);
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    coap_opt_finish(&pdu, len, COAP_OPT_FINISH_NONE);

    if (gcoap_req_send((uint8_t *)_buf, strlen(PAYLOAD), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP) < 0) {
        printf("Failed to send request\n");
    }
}

int main(void) {
    msg_init_queue(my_stack, sizeof(my_stack));
    send_coap_request();
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return 0;
}