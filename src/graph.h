#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

class Graph
{
public:
	Graph(size_t v = 0);
	Graph(const char *filename);
	virtual void addVertex();
	virtual void addEdge(size_t u, size_t v);
	bool hasEdge(size_t u, size_t v) const;
	size_t V() const;
private:
	std::vector< std::vector<bool> > m_adj;
};

#endif
