#pragma once
#include <memory>
#include <map>
#include "receiver.h"

#define SIGNATURE(type) \
    typeid(type).name()

class F125parserError : public std::exception {
    const char* what() const noexcept override {
        return "F125parser error : ";
    }
};

class F125parser {
public:
    F125parser() = default;
    ~F125parser() = default;

    template<typename T>
    std::shared_ptr<Packet>& getPacket() {
        auto& packet = _packetMap[SIGNATURE(T)];

        return packet;
    }

    void parse(std::shared_ptr<Packet>& packet);


    void participantDataDisplay();
    
private:
    std::map<std::string, std::shared_ptr<Packet>> _packetMap;


    // Define every packet for each enum
    std::map<PacketId, std::function<void(const std::shared_ptr<Packet>&)>> _parseMap = {
        { ePacketIdParticipants, [this](const std::shared_ptr<Packet>& packet) {
            _registerPacket<ParticipantData>(packet);
        }}
    };

    //void _parseParticipantData(const std::shared_ptr<Packet>& packet);
    
    template<typename T>
    void _registerPacket(const std::shared_ptr<Packet>& packet) {
        _packetMap[SIGNATURE(T)] = packet;
    }
};