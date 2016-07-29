#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <list>

class Graph
{
public:
	static Graph BFSTree(const Graph &g);
	
	Graph(size_t v = 0);
	Graph(const char *filename);
	void addVertex();
	void addEdge(size_t u, size_t v);
	void removeEdge(size_t u, size_t v);
	const std::list<size_t> &adj(size_t i) const;
	size_t V() const;
	
	size_t parent(size_t i) const;
	size_t level(size_t i) const;
	bool isTree() const;
private:
	std::vector< std::list<size_t> > m_adj;
	std::vector<size_t> m_parent;
	std::vector<size_t> m_level;
	bool m_isTree;
};

#endif
