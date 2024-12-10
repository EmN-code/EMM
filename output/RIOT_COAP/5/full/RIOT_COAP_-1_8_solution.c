#include <ztimer.h>
#include <xtimer.h>
#include <time_units.h>
#include <timex.h>
#include <thread.h>
#include <string.h>
#include <paho_mqtt.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <mutex.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <esp_wifi.h>
#include <dht_params.h>
#include <dht.h>
#include <arpa/inet.h>
#include <assert.h>
#include <MQTTClient.h>

// Define the address and port for the CoAP server
#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define EMQX_URI_PATH "/coap/test"

// Define the payload
#define PAYLOAD "Hello, COAP Cloud"

// Define the length of the message queue
#define MAIN_QUEUE_SIZE (8)

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t *pdu, const sock_udp_ep_t *remote) {
    (void)memo;   // Suppress unused parameter warning
    (void)remote; // Suppress unused parameter warning
    if (pdu != NULL) {
        printf("Response received: %.*s\n", (int)pdu->payload_len, (char *)pdu->payload);
    } else {
        printf("No response received\n");
    }
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    // Define variables for the CoAP request
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    sock_udp_ep_t remote = { .family = AF_INET, .port = COAP_SERVER_PORT };

    // Convert the IP string to a packed binary format
    if (inet_pton(AF_INET, COAP_SERVER_IP, &remote.addr.ipv4) != 1) {
        printf("ERROR: unable to parse destination address\n");
        return 1;
    }

    // Initialize the CoAP request
    gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, EMQX_URI_PATH);

    // Set the payload
    memcpy(pdu.payload, PAYLOAD, strlen(PAYLOAD));
    pdu.payload_len = strlen(PAYLOAD);

    // Send the CoAP request
    ssize_t len = gcoap_req_send(buf, pdu.payload_len, &remote, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (len <= 0) {
        printf("Error sending CoAP request\n");
    }
    return 0;
}