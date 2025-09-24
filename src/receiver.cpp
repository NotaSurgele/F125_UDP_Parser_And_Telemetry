//
// Created by User on 16/09/2025.
//

  #include "../include/receiver.h"


  Receiver::Receiver(asio::io_context &context, unsigned short prt)
  : sock(context, udp::endpoint(udp::v4(), prt)), queue(), port(prt) {
  }

  Receiver::~Receiver(){
    this->stop();
  }

  std::shared_ptr<Packet> Receiver::poll(void) {
    if (!this->queue.empty()) {

      auto test = this->queue.front();

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
    this->stopped = true;
    if (thread_.joinable())
      thread_.join();
  }