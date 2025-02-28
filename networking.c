#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "networking.h"
#include "defines.h"
#include "game.h"
#include <sys/types.h>


typedef struct {
    uint32_t player_id;    // Use fixed-width types
    uint32_t snake_length;
} __attribute__((packed)) Header; // Packed to avoid padding

typedef struct {
    int sockfd;
    Game *game;
} ThreadData;

void send_data(int sockfd, const DataPacket *packet) {
    Header header;
    header.player_id = htonl(packet->player_id);
    header.snake_length = htonl(packet->snake_length);

    // Send header
    size_t sent = 0;
    while (sent < sizeof(header)) {
        ssize_t res = send(sockfd, (char*)&header + sent, sizeof(header) - sent, 0);
        if (res <= 0) {
            perror("send header failed");
            return;
        }
        sent += res;
    }

    // Send snake segments if present
    if (packet->snake_length > 0) {
        sent = 0;
        size_t total = packet->snake_length * sizeof(SnakeSegment);
        while (sent < total) {
            ssize_t res = send(sockfd, (char*)packet->snake + sent, total - sent, 0);
            if (res <= 0) {
                perror("send snake failed");
                return;
            }
            sent += res;
        }
    }
}

bool receive_data(int sockfd, DataPacket *packet) {
    Header header;
    ssize_t received = recv(sockfd, &header, sizeof(header), MSG_WAITALL);
    if (received != sizeof(header)) return false;

    packet->player_id = ntohl(header.player_id);
    packet->snake_length = ntohl(header.snake_length);

    // Validate snake_length to prevent excessive allocation
    if (packet->snake_length > SNAKE_MAX_LENGTH) {
        fprintf(stderr, "Invalid snake length: %u\n", packet->snake_length);
        return false;
    }

    packet->snake = NULL;
    if (packet->snake_length > 0) {
        packet->snake = malloc(packet->snake_length * sizeof(SnakeSegment));
        if (!packet->snake) {
            perror("malloc failed");
            return false;
        }
        received = recv(sockfd, packet->snake, packet->snake_length * sizeof(SnakeSegment), MSG_WAITALL);
        if (received != (ssize_t)(packet->snake_length * sizeof(SnakeSegment))) {
            free(packet->snake);
            packet->snake = NULL;
            return false;
        }
    }
    return true;
}

void *sender_func(void *arg) {
    ThreadData *data = (ThreadData*)arg;
    Game *game = data->game;
    int sockfd = data->sockfd;

    DataPacket packet;
    packet.snake = malloc(SNAKE_MAX_LENGTH * sizeof(SnakeSegment)); // Pre-allocate max size

    while (true) {
        Snake *snake = game->world->snakes[0];
        if (!snake) break;

        packet.player_id = game->players[0]->id;
        packet.snake_length = snake->length;
        memcpy(packet.snake, snake->body, snake->length * sizeof(SnakeSegment));

        send_data(sockfd, &packet);

        // Calculate delay with floating-point division
        double delay = 1.0 / snake->speed;
        usleep((useconds_t)(delay * 1000000));
    }

    free(packet.snake);
    return NULL;
}

void *receiver_func(void *arg) {
    ThreadData *data = (ThreadData*)arg;
    Game *game = data->game;
    int sockfd = data->sockfd;

    DataPacket received;
    while (true) {
        if (!receive_data(sockfd, &received)) break;

        if (received.snake) {
            bool is_existing = update_player(game, &received);
            if (!is_existing) {
                add_player(game, &received);
            }
            free(received.snake);
        }
    }
    return NULL;
}

void start_networking(Game *game) {
    struct sockaddr_in server_addr;
    int sockfd;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation failed");
        return;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    while (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        sleep(1); // Wait before retrying
    }

    ThreadData *data = malloc(sizeof(ThreadData));
    data->sockfd = sockfd;
    data->game = game;

    pthread_t sender_thread, receiver_thread;
    if (pthread_create(&sender_thread, NULL, sender_func, data) != 0) {
        perror("pthread_create (sender) failed");
        close(sockfd);
        return;
    }
    if (pthread_create(&receiver_thread, NULL, receiver_func, data) != 0) {
        perror("pthread_create (receiver) failed");
        close(sockfd);
        return;
    }

    // Detach threads to avoid memory leaks (or handle joining elsewhere)
    pthread_detach(sender_thread);
    pthread_detach(receiver_thread);
}