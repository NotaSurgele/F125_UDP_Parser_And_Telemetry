#include "f125parser.h"
#include "f125.h"
#include "receiver.h"


void F125parser::parse(std::shared_ptr<Packet>& packet) {
    try {
        auto packetHeader = packet->head;

        for (auto &it : _parseMap) {
            auto &enumId = it.first;
            auto &parsePacket = it.second;

            if (packetHeader.m_packetId == enumId)
                parsePacket(packet);
        }
    } catch (std::exception &e) {
        throw e;
    }

    /*try {
        switch (packetHeader.m_packetId) {
            case ePacketIdMotion:
                //parseMotionData(recv_buf);
                break;
            case ePacketIdSession:
                // do stuff
                    break;
            case ePacketIdLapData:
                // do stuff
                    break;
            case ePacketIdEvent:
                // do stuff
                    break;
            case ePacketIdParticipants:
                    _parseParticipantData(packet);
                    break;
            case ePacketIdCarSetups:
                // do stuff
                    break;
            case ePacketIdCarTelemetry:
                //do stuff
                    break;
            case ePacketIdCarStatus:
                // do stuff;
                    break;
            case ePacketIdFinalClassification:
                // do stuff
                    break;
            case ePacketIdLobbyInfo:
                // do stuff
                    break;
            case ePacketIdCarDamage:
                // do stuff
                    break;
            case ePacketIdSessionHistory:
                //do stuff
                    break;
            case ePacketIdTyreSets:
                // do stuff
                break;
            case ePacketIdMotionEx:
                // do stuff
                    break;
            case ePacketIdTimeTrial:
                // do stuff
                    break;
            case ePacketIdLapPositions:
                // do stuff
                    break;
            default:
                throw std::invalid_argument("oupsi should not happend hihihi");
                break;
        }
    } catch (std::exception& e) {
        throw e;
    }*/
}

void F125parser::participantDataDisplay() {
    auto &packet = getPacket<ParticipantData>();
    if (!packet) return;

    try {
        auto buffer = packet->data;
        auto activeCars = *buffer.data();
        auto ppd = reinterpret_cast<ParticipantData *>(buffer.data() + sizeof(uint8)); // after m_numActiveCars

        for (int i = 0; i < (int)activeCars; i++) {
            auto pd = ppd[i];
            std::string name(pd.m_name, strnlen(pd.m_name, sizeof(pd.m_name)));

            std::cout << "Packet format " << name << std::endl;
        }

    } catch (std::exception &e) {
        throw e.what();
    }
}

/*
void F125parser::_parseParticipantData(const std::shared_ptr<Packet>& packet)
{
    _registerPacket<ParticipantData>(packet);
    try {
        auto buffer = packet->data;
        auto activeCars = *buffer.data();
        auto ppd = reinterpret_cast<ParticipantData *>(buffer.data() + sizeof(uint8)); // after m_numActiveCars

        for (int i = 0; i < (int)activeCars; i++) {
            auto pd = ppd[i];
            std::string name(pd.m_name, strnlen(pd.m_name, sizeof(pd.m_name)));
            
            std::cout << "Packet format " << name << std::endl;
        }

    } catch (std::exception &e) {
        throw e.what();
    }
}*/