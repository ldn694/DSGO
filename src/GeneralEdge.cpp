#include "GeneralEdge.h"

GeneralEdge::GeneralEdge(int from, int to, int weight): from(from), to(to), weight(weight) {
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