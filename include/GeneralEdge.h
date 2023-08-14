#pragma once

#include "Color.h"
struct GeneralEdge {
    int from, to, weight;
    General::ColorType type;
    GeneralEdge(int from, int to, int weight);
    bool operator < (const GeneralEdge& other) const;
};