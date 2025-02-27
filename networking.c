#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "networking.h"
#include "defines.h"
#include "game.h"

int sockfd;
pthread_mutex_t socket_mutex;

void send_data(int sockfd, DataPacket *packet)
{
    size_t total_size = sizeof(packet->player_id) + sizeof(packet->length) + (packet->length * sizeof(SnakeSegment));
    char *buffer = malloc(total_size);
    if (!buffer)
    {
        printf("ERROR: malloc failed\n");
        return;
    }

    memcpy(buffer, &packet->player_id, sizeof(packet->player_id));
    memcpy(buffer + sizeof(packet->player_id), &packet->length, sizeof(packet->length));
    if (packet->length > 0)
    {
        memcpy(buffer + sizeof(packet->player_id) + sizeof(packet->length), packet->snake, packet->length * sizeof(SnakeSegment));
    }

    size_t sent_bytes = 0;
    while (sent_bytes < total_size)
    {
        ssize_t result = send(sockfd, buffer + sent_bytes, total_size - sent_bytes, 0);
        if (result < 0)
        {
            printf("ERROR: send failed\n");
            break;
        }
        sent_bytes += result;
    }

    free(buffer);
}

void receive_data(int sockfd, DataPacket *packet)
{
    ssize_t received_bytes;
    received_bytes = recv(sockfd, &packet->player_id, sizeof(packet->player_id), MSG_WAITALL);
    if (received_bytes <= 0) return;
    
    received_bytes = recv(sockfd, &packet->length, sizeof(packet->length), MSG_WAITALL);
    if (received_bytes <= 0) return;

    if (packet->length > 0)
    {
        packet->snake = malloc(packet->length * sizeof(SnakeSegment));
        if (!packet->snake)
        {
            printf("ERROR: malloc failed\n");
            return;
        }
        received_bytes = recv(sockfd, packet->snake, packet->length * sizeof(SnakeSegment), MSG_WAITALL);
        if (received_bytes <= 0)
        {
            free(packet->snake);
            packet->snake = NULL;
            return;
        }
    }
    else
    {
        packet->snake = NULL;
    }
}

void *sender_func(void *data)
{
    Game *game = (Game*)data;
    while (true)
    {
        DataPacket packet;
        packet.length = game->world->snakes[0]->length;
        packet.player_id = game->players[0]->id;
        packet.snake = malloc(packet.length * sizeof(SnakeSegment));
        memcpy(packet.snake, game->world->snakes[0]->body, packet.length * sizeof(SnakeSegment));

        send_data(sockfd, &packet);
        printf("Data sent\n");
        free(packet.snake);
        sleep(1 / game->world->snakes[0]->speed);
    }
    return NULL;
}

void *receiver_func(void *data)
{
    Game *game = (Game *)data;
    while (true)
    {
        DataPacket received;
        receive_data(sockfd, &received);

        bool new_player = true;

        if (received.snake)
        {
            printf("Receiving data...\n");
            pthread_mutex_lock(&game->update_mutex);
            for (size_t i = 0; i < game->players_count; ++i)
            {
                if (game->players[i]->id == received.player_id)
                {
                    new_player = false;
                    game->world->snakes[game->players[i]->snake_idx]->length = received.length;
                    memcpy(game->world->snakes[game->players[i]->snake_idx]->body, received.snake, received.length * sizeof(SnakeSegment));
                    break;
                }
            }
            pthread_mutex_unlock(&game->update_mutex);

            if (new_player) add_player(game, &received);
            
            free(received.snake);
            received.snake = NULL;

            // sleep(1 / game->world->snakes[0]->speed);
        }
    }
    return NULL;
}

void start_networking(Game *game)
{
    pthread_mutex_init(&socket_mutex, NULL);

    struct sockaddr_in server_addr;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("ERROR: socket creation failed\n");
        return;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    while (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        sleep(1);
    }

    pthread_t sender_thread, receiver_thread;
    pthread_create(&sender_thread, NULL, sender_func, game);
    pthread_create(&receiver_thread, NULL, receiver_func, game);
}