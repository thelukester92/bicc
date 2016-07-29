#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

class Graph
{
public:
	Graph(size_t v = 0);
	Graph(const char *filename);
	std::vector<size_t> &adj(size_t i);
	size_t V() const;
private:
	std::vector< std::vector<size_t> > m_adj;
};

#endif
