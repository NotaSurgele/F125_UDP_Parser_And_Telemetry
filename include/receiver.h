//
// Created by User on 16/09/2025.
//

#ifndef SERVER_H
#define SERVER_H

#include <queue>
#include <iostream>
#include <exception>
#include <thread>
#include <memory>
#include <array>

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00 // Windows 10
#endif

#include <asio.hpp>
#include "f125.h"

using asio::ip::udp;

class Packet {

    public:
    Packet() = default;
    ~Packet() = default;
    PacketHeader head;
    std::vector<unsigned char> data;
};


class Receiver {
public:
    Receiver(asio::io_context &context, unsigned short port);
    ~Receiver();
    std::shared_ptr<Packet> poll(void);


    void receive() {

        while (true) {

            try {

                std::array<unsigned char, 2048> buff = {0};
                size_t l = this->sock.receive_from(asio::buffer(buff), endpoint);

                auto newPacket = std::make_shared<Packet>();

                std::memcpy(&newPacket->head, buff.data(), sizeof(PacketHeader));

                newPacket->data.resize(l - sizeof(PacketHeader));

                std::memcpy(newPacket->data.data(), buff.data() + sizeof(PacketHeader), l - sizeof(PacketHeader));

                this->queue.push(newPacket);

            } catch (std::exception &e) {

                std::cerr << "error : " << e.what() << std::endl;

            }

        }

    };

    void start(void);
    void stop(void);
private:



    udp::socket sock;
    udp::endpoint endpoint;

    std::queue<std::shared_ptr<Packet>> queue;

    std::thread thread_;

};




#endif //SERVER_H