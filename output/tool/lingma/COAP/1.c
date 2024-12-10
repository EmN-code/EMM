#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <xtimer.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static sock_udp_ep_t server_ep = {
    .port = SERVER_PORT,
    .family = AF_INET,
};

static void send_coap_request(void) {
    // Initialize the UDP socket
    sock_udp_t sock;
    int res = sock_udp_create(&sock, NULL, NULL, 0);
    if (res < 0) {
        printf("Failed to create UDP socket: %d\n", res);
        return;
    }

    // Set the server IP address
    res = inet_pton(AF_INET, SERVER_IP, &server_ep.addr.ipv4);
    if (res <= 0) {
        printf("Invalid server IP address\n");
        sock_udp_close(&sock);
        return;
    }

    // Initialize the CoAP packet
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    size_t len = gcoap_req_init(&pdu, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);

    // Add the payload to the CoAP packet
    coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);
    len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
    len += coap_pkt_add_payload(&pdu, (uint8_t *)PAYLOAD, strlen(PAYLOAD));

    // Send the CoAP request
    res = sock_udp_send(&sock, buf, len, &server_ep);
    if (res < 0) {
        printf("Failed to send CoAP request: %d\n", res);
    } else {
        printf("CoAP request sent successfully\n");
    }

    // Close the UDP socket
    sock_udp_close(&sock);
}

int main(void) {
    // Initialize the network interface (assuming it's already connected)
    // You might need to add code to connect to Wi-Fi or Ethernet here

    // Send the CoAP request
    send_coap_request();

    // Wait for a while before sending another request (optional)
    xtimer_sleep(10);

    return 0;
}