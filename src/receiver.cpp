//
// Created by User on 16/09/2025.
//

#include "receiver.h"

Receiver::Receiver(asio::io_context &context, unsigned short port)
: sock(context, udp::endpoint(udp::v4(), port)), queue() {
}

Receiver::~Receiver(){
  this->stop();
}

Packet *Receiver::poll(void) {
  if (!this->queue.empty()) {

    Packet *test = this->queue.front();
    this->queue.pop();
    return (test);
  } else {
    return (nullptr);
  }
}

void Receiver::start(void) {
   thread_ = std::thread(&Receiver::receive, this);
}

void Receiver::stop(void) {
  thread_.join();
}