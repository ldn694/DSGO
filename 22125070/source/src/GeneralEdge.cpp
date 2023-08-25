#include "GeneralEdge.h"

GeneralEdge::GeneralEdge(int from, int to, int weight, General::ColorType type, float opacity): from(from), to(to), weight(weight), type(type), opacity(opacity) {
    
}

bool GeneralEdge::operator<(const GeneralEdge& other) const {
    if (weight != other.weight) {
        return weight < other.weight;
    }
    if (from != other.from) {
        return from < other.from;
    }
    return to < other.to;
}