#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <list>

typedef std::pair<size_t, size_t> Edge;
Edge reverseEdge(const Edge &e);

class Graph
{
public:
	void resize(size_t v);
	void addVertex();
	void addEdge(size_t u, size_t v);
	void addDirectedEdge(size_t u, size_t v);
	std::list<size_t> &adj(size_t u);
	const std::list<size_t> &adj(size_t u) const;
	const std::vector<Edge> &edges() const;
	char vertex(size_t u) const;
	size_t V() const;
	size_t E() const;
	
	void spanningTree(Graph &t, Graph &nt, std::vector<size_t> &parent, std::vector<size_t> &level) const;
	void connectedComponents(std::vector< std::vector<size_t> > &components) const;
private:
	std::vector< std::list<size_t> > m_adj;
	std::vector<Edge> m_edges;
	std::vector<char> m_vertices;
};

std::ostream &operator<<(std::ostream &out, const Graph &g);

#endif
