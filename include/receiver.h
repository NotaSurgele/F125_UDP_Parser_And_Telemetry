//
// Created by User on 16/09/2025.
//

#ifndef SERVER_H
#define SERVER_H

#include <queue>
#include <iostream>
#include <exception>

#include "asio.hpp"

#include "f125.h"




using asio::ip::udp;


struct Packet {
    PacketHeader *head;
    void *data;
};


class Receiver {
public:
    Receiver(asio::io_context &context, unsigned short port);
    ~Receiver();
    Packet * poll(void);


    void receive() {


        while (true) {

            try {

                std::array<unsigned char, 1284> buff = {0};
                size_t l = this->sock.receive_from(asio::buffer(buff), endpoint);

                Packet *newPacket = new Packet();

                if (newPacket != nullptr) {
                    newPacket->head = new PacketHeader();
                    std::memcpy(&newPacket->head->m_packetFormat, buff.data(), sizeof(PacketHeader));

                    newPacket->data = malloc(l);

                    std::memcpy(newPacket->data, buff.data(), l);

                    this->queue.push(newPacket);
                }

            } catch (std::exception e) {

                std::cerr << e.what() << std::endl;

            }

        }

    };

    void start(void);
    void stop(void);
private:



    udp::socket sock;
    udp::endpoint endpoint;
    //std::array<char, 2048> buff;

    std::queue<Packet *> queue;

    std::thread thread_;

};




#endif //SERVER_H