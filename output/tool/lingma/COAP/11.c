#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <xtimer.h>

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"
#define COAP_PAYLOAD "Hello, COAP Cloud"

static sock_udp_ep_t server_ep = {
    .family = AF_INET,
    .port = COAP_SERVER_PORT
};

static void coap_response_handler(coap_pkt_t *pdu, uint8_t type, uint8_t code, uint8_t msg_id, void *context) {
    (void)pdu;
    (void)type;
    (void)code;
    (void)msg_id;
    (void)context;

    printf("Received response from CoAP server\n");
}

int main(void) {
    // Initialize the network
    sock_udp_t sock;
    int res = sock_udp_create(&sock, NULL, NULL, 0);
    if (res < 0) {
        printf("Error creating UDP socket: %d\n", res);
        return 1;
    }

    // Set the server endpoint
    res = inet_pton(AF_INET, COAP_SERVER_IP, &server_ep.addr);
    if (res <= 0) {
        printf("Invalid server IP address\n");
        return 1;
    }

    // Initialize CoAP request
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    size_t len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, COAP_URI_PATH);

    // Add payload to the CoAP request
    len += coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    len += coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    memcpy(buf + len, COAP_PAYLOAD, strlen(COAP_PAYLOAD));
    len += strlen(COAP_PAYLOAD);

    // Send the CoAP request
    res = gcoap_req_send(buf, len, &server_ep, coap_response_handler, NULL, GNRC_NETTYPE_IPV4);
    if (res < 0) {
        printf("Error sending CoAP request: %d\n", res);
        return 1;
    }

    printf("CoAP POST request sent to %s:%d\n", COAP_SERVER_IP, COAP_SERVER_PORT);

    // Wait for a response
    xtimer_sleep(5); // Wait for 5 seconds

    // Clean up
    sock_udp_close(&sock);

    return 0;
}