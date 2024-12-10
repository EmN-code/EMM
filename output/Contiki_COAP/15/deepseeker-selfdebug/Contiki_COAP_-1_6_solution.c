
#include <stdio.h>
#include <string.h>
#include "net/coap.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-udp-packet.h"

#define COAP_SERVER_IP "47.102.103.1"
#define COAP_SERVER_PORT 5683
#define COAP_URI_PATH "/coap/test"

static void coap_send_post_request(void) {
    coap_message_t request;
    coap_message_t response;
    uint8_t buffer[128];
    int32_t offset = 0;

    memset(&request, 0, sizeof(request));
    memset(&response, 0, sizeof(response));

    request.type = COAP_TYPE_CON;
    request.code = COAP_METHOD_POST;
    request.id = 1234; // Example CoAP message ID
    request.token_len = 0;
    request.payload_len = strlen("Hello, COAP Cloud");
    request.payload = (uint8_t *)"Hello, COAP Cloud";

    coap_add_option(&request, COAP_OPTION_URI_PATH, strlen(COAP_URI_PATH), (uint8_t *)COAP_URI_PATH);

    coap_handler_status_t status = coap_call_handlers(&request, &response, buffer, sizeof(buffer), &offset);

    if (status == COAP_HANDLER_STATUS_OK) {
        printf("CoAP POST request sent successfully.\n");
    } else {
        printf("Failed to send CoAP POST request.\n");
    }
}

int main(void) {
    printf("Starting CoAP client...\n");

    // Initialize network stack
    uip_init();

    // Set up the CoAP server IP address and port
    uip_ipaddr_t server_ip;
    uip_ipaddr(&server_ip, COAP_SERVER_IP);
    uip_udp_conn_t *conn = uip_udp_new(&server_ip, UIP_HTONS(COAP_SERVER_PORT));

    if (conn != NULL) {
        printf("UDP connection to CoAP server established.\n");
    } else {
        printf("Failed to establish UDP connection to CoAP server.\n");
        return 1;
    }

    // Send the CoAP POST request
    coap_send_post_request();

    return 0;
}
