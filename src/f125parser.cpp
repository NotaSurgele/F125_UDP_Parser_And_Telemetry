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
        throw;
    }
}

void F125parser::participantDataDisplay() {
    auto packet = getPacket<ParticipantData>();
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
