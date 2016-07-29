#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <set>
#include <tuple>

class Graph
{
public:
	static Graph BFSTree(const Graph &g, std::set< std::pair<size_t, size_t> > *nonTreeEdges = NULL);
	
	Graph(size_t v = 0);
	Graph(const char *filename);
	void addVertex();
	void addEdge(size_t u, size_t v);
	void removeEdge(size_t u, size_t v);
	const std::set<size_t> &adj(size_t i) const;
	size_t V() const;
	
	size_t parent(size_t i) const;
	size_t level(size_t i) const;
	bool isTree() const;
private:
	std::vector< std::set<size_t> > m_adj;
	std::vector<size_t> m_parent;
	std::vector<size_t> m_level;
	bool m_isTree;
};

#endif
