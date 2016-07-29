#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

class Graph
{
public:
	Graph(size_t v = 0);
	Graph(const char *filename);
	const std::vector<size_t> &adj(size_t i) const;
	size_t V() const;
private:
	std::vector< std::vector<size_t> > m_adj;
};

#endif
