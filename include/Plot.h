#pragma once


#include <vector>
#include "imgui.h"


template<typename T>
class Plot {
    public:
        std::map<ImGuiID, T> _data;
        Plot() = default;
        ~Plot() = default;
        PlotVar()

}