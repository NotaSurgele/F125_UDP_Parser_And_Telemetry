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
    //std::mutex packetMutex;

    F125parser() = default;
    ~F125parser() = default;

    template<typename T>
    std::shared_ptr<Packet>& getPacket() {
        return _packetMap[SIGNATURE(T)];
    }


    void parse(std::shared_ptr<Packet>& packet);

    // je met sa la pour tester si sa fonctionne mais on peux faire une class f125metricDisplayer ou on peux mettre sa dedans et affiché les data dans une window
    void participantDataDisplay();
    
private:
    mutable std::map<std::string, std::shared_ptr<Packet>> _packetMap;

    // Define every function packet parsing for each enum
    std::map<PacketId, std::function<void(const std::shared_ptr<Packet>&)>> _parseMap = {
        {
            ePacketIdParticipants, [this](const std::shared_ptr<Packet>& packet) {
            _registerPacket<PacketParticipantsData>(packet);
            }
        },
        {
            ePacketIdMotion, [this](const std::shared_ptr<Packet>& packet) {
                _registerPacket<PacketMotionData>(packet);
            }
        },
        {
            ePacketIdCarTelemetry, [this](const std::shared_ptr<Packet>& packet) {
                _registerPacket<PacketCarTelemetryData>(packet);
            }
        }
    };

    template<typename T>
    void _registerPacket(const std::shared_ptr<Packet>& packet) {
        _packetMap[SIGNATURE(T)] = packet;
    }
};