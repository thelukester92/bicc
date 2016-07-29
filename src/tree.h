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
	const std::vector<bool> &treeEdges(size_t i) const;
private:
	void identifyTreeEdges();
	std::vector< std::vector<bool> > m_treeEdge;
};

#endif
