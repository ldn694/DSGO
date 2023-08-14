struct GeneralEdge {
    int from, to, weight;
    GeneralEdge(int from, int to, int weight);
    bool operator < (const GeneralEdge& other) const;
};