
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define STACK_SIZE 1024
#define MSG_QUEUE_SIZE 8

typedef struct {
    int sender_pid;
    int type;
    union {
        void *ptr;
        int value;
    } content;
} msg_t;

msg_t msg_queue[MSG_QUEUE_SIZE];

void sender_thread(void *arg) {
    int target_pid = *(int *)arg;
    msg_t msg;
    msg.type = 1;
    msg.content.value = 42;

    while (1) {
        printf("Sender: Sending message...\n");
        if (send(target_pid, &msg, sizeof(msg), 0) < 0) {
            perror("send");
        }
        sleep(1);
    }
}

void receiver_thread(void *arg) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(12345);

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    msg_t msg;
    while (1) {
        printf("Receiver: Waiting for message...\n");
        if (recv(sockfd, &msg, sizeof(msg), 0) < 0) {
            perror("recv");
        } else {
            printf("Receiver: Received message with value %d\n", msg.content.value);
        }
    }
}

int main() {
    int sender_pid = 1;
    int receiver_pid = 2;

    pthread_t sender_tid, receiver_tid;

    if (pthread_create(&sender_tid, NULL, (void *(*)(void *))sender_thread, &receiver_pid) != 0) {
        perror("pthread_create sender");
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&receiver_tid, NULL, (void *(*)(void *))receiver_thread, NULL) != 0) {
        perror("pthread_create receiver");
        exit(EXIT_FAILURE);
    }

    pthread_join(sender_tid, NULL);
    pthread_join(receiver_tid, NULL);

    return 0;
}
