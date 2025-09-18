#pragma once
#include <memory>
#include "receiver.h"

class F125parserError : public std::exception {
    const char* what() const noexcept override {
        return "F125parser error : ";
    }
};

class F125parser {
public:
    F125parser() = default;
    ~F125parser() = default;

    void parse(std::shared_ptr<Packet>& packet); 
    
private:
    void _parseParticipantData(std::vector<unsigned char>& recv_buff);
};