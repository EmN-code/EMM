#include "string.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "arpa/inet.h"
#include "mutex.h"
#include "msg.h"
#include "thread.h"
#include "esp_wifi.h"
#include "dht_params.h"
#include "mma8x5x.h"
#include "mma8x5x_regs.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"

// Variables and constants
#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define MESSAGE "Hello, COAP Cloud"

static sock_udp_ep_t server_sock = { .family = AF_INET, .port = SERVER_PORT };

static char buf[128];

// Handler for response
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)remote;
    if (memo->state == GCOAP_MEMO_FINAL) {
        printf("Response received: code class: %u code detail: %u\n", coap_get_code_class(pdu), coap_get_code_detail(pdu));
    }
}

// Main program
int main(void) {
    // Configure IP address of server
    inet_pton(AF_INET, SERVER_IP, &server_sock.addr.ipv4);

    // Initialize CoAP packet
    coap_pkt_t pdu;
    gcoap_req_init(&pdu, (uint8_t *)buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);

    // Set the payload
    size_t payload_len = strlen(MESSAGE);
    coap_hdr_set_payload(&pdu, &buf[0] + pdu.hdr_len, sizeof(buf) - pdu.hdr_len);
    memcpy(&buf[pdu.hdr_len], MESSAGE, payload_len);

    // Send request
    ssize_t res = gcoap_req_send((const uint8_t *)buf, pdu.hdr_len + payload_len, &server_sock, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if(res <= 0) {
        printf("Error sending CoAP request\n");
    }
    return 0;
}
