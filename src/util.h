#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <vector>
#include "graph.h"

std::ostream &operator<<(std::ostream &out, const Graph &g);
Edge reverseEdge(const Edge &e);
size_t LCA(std::vector<size_t> &parent, std::vector<size_t> &level, size_t u, size_t v);
void prefixSum(std::vector<size_t> &v);

#endif
