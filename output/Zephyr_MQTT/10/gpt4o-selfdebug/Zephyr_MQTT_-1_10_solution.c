#include <zephyr/kernel.h>  // Correct path for kernel.h
#include <zephyr/net/socket.h>  // Necessary for socket operations
#include <zephyr/device.h>  // Required for device management

#define STACKSIZE 1024
#define PRIORITY 7

#define BROKER_ADDR "47.102.103.1"
#define BROKER_PORT 1883

void mqtt_publish_task(void)
{
    struct sockaddr_in broker;
    int sock;
    char message[] = "work";
    int counter = 0;

    // Setting up the broker address
    broker.sin_family = AF_INET;
    broker.sin_port = htons(BROKER_PORT);
    zsock_inet_pton(AF_INET, BROKER_ADDR, &broker.sin_addr);

    // Connecting to the broker
    sock = zsock_socket(AF_INET, SOCK_STREAM, 0);
    zsock_connect(sock, (struct sockaddr *)&broker, sizeof(broker));

    while (1) {
        // Toggle message state
        if (counter % 2 == 0) {
            memcpy(message, "work", strlen("work")+1);
        } else {
            memcpy(message, "not work", strlen("not work")+1);
        }

        // Publish the message
        zsock_send(sock, message, strlen(message), 0);
        counter++;

        // Wait for 5 seconds
        k_sleep(K_SECONDS(5));
    }
}

void main(void)
{
    k_tid_t task_id = k_thread_create(&mqtt_pub_thread, mqtt_pub_stack,
                                       STACKSIZE, (k_thread_entry_t)mqtt_publish_task,
                                       NULL, NULL, NULL, PRIORITY, 0, K_NO_WAIT);

    k_thread_name_set(task_id, "mqtt_publish_task");
}