#include <time_units.h>
#include <thread.h>
#include <string.h>
#include <paho_mqtt.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <msg.h>
#include <mutex.h>
#include <dht_params.h>
#include <dht.h>
#include <assert.h>
#include <arpa/inet.h>
#include <MQTTClient.h>
#include <esp_wifi.h>

#define REMOTE_ADDR "47.102.103.1"
#define REMOTE_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_RESP) {
        printf("Success: Received response\n");
    }
    else {
        printf("Error: Request not acknowledged\n");
    }
}

int main(void) {
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote;
    ssize_t bytes;

    remote.family = AF_INET;
    remote.port = REMOTE_PORT;
    inet_pton(AF_INET, REMOTE_ADDR, &remote.addr.ipv4);

    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);
    coap_hdr_set_type(&pdu, COAP_TYPE_CON);

    memcpy(pdu.payload, MESSAGE, strlen(MESSAGE));
    bytes = gcoap_req_send(buf, pdu.hdr_len + strlen(MESSAGE), &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);

    if (bytes <= 0) {
        printf("Error sending request\n");
    }
    
    return 0;
}