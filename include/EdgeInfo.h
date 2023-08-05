#pragma once
#include "Template.h"
#include "Color.h"

struct EdgeInfo {
    int idStart, idEnd;
    std::string weight;
    EdgeInfo(int idStart, int idEnd, std::string weight = "");
};