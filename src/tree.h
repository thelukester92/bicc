#ifndef TREE_H
#define TREE_H

#include "graph.h"
#include <vector>

class Tree : public Graph
{
public:
	Tree(size_t v = 0);
	Tree(const char *filename);
	Tree(const Graph &g);
	virtual void addVertex();
	virtual void addEdge(size_t u, size_t v);
	bool isTreeEdge(size_t u, size_t v) const;
private:
	void identifyTreeEdges();
	std::vector< std::vector<bool> > m_tAdj;
};

#endif
