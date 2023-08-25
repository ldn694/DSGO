#pragma once

#include "Color.h"
struct GeneralEdge {
    int from, to, weight;
    float opacity;
    General::ColorType type;
    GeneralEdge(int from, int to, int weight, General::ColorType type = General::ColorType::normal, float opacity = 1.0f);
    bool operator < (const GeneralEdge& other) const;
};