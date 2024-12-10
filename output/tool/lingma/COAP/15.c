#include <stdio.h>
#include <string.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define SERVER_IP "47.102.103.1"
#define SERVER_PORT 5683
#define URI_PATH "/coap/test"
#define PAYLOAD "Hello, COAP Cloud"

static sock_udp_ep_t server_ep = {
    .port = SERVER_PORT,
    .family = AF_INET,
};

static void send_coap_request(void) {
    // Initialize the CoAP packet
    coap_pkt_t pkt;
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    size_t len;

    // Initialize the CoAP request
    len = gcoap_req_init(&pkt, buf, sizeof(buf), COAP_METHOD_POST, URI_PATH);

    // Add the payload
    coap_opt_add_format(&pkt, COAP_FORMAT_TEXT);
    coap_opt_finish(&pkt, COAP_OPT_FINISH_PAYLOAD);
    coap_put_payload(&pkt, PAYLOAD, strlen(PAYLOAD));

    // Send the CoAP request
    sock_udp_ep_t local_ep = SOCK_IPV4_EP_ANY;
    sock_udp_t sock;
    int res = sock_udp_create(&sock, &local_ep, NULL, 0);
    if (res < 0) {
        printf("Error creating UDP socket: %d\n", res);
        return;
    }

    res = sock_udp_send(&sock, buf, len, &server_ep);
    if (res < 0) {
        printf("Error sending CoAP request: %d\n", res);
    } else {
        printf("CoAP request sent successfully\n");
    }

    // Close the socket
    sock_udp_close(&sock);
}

int main(void) {
    // Initialize the network interface
    gnrc_netif_t *netif = gnrc_netif_get_by_pid(0); // Assuming the first network interface
    if (!netif) {
        printf("No network interface found\n");
        return -1;
    }

    // Bring up the network interface
    gnrc_netif_acquire(netif);
    gnrc_netif_set_flags(netif, GNRC_NETIF_FLAGS_IPV4 | GNRC_NETIF_FLAGS_UP);
    gnrc_netif_release(netif);

    // Set the server IP address
    if (inet_pton(AF_INET, SERVER_IP, &server_ep.addr.ipv4)) {
        printf("Server IP address set successfully\n");
    } else {
        printf("Invalid server IP address\n");
        return -1;
    }

    // Send the CoAP request
    send_coap_request();

    return 0;
}