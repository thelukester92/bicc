#include "graph.h"
#include <fstream>
using std::vector;
using std::string;
using std::ifstream;

Graph::Graph(size_t v) : m_adj(v, vector<bool>(v, false))
{}

Graph::Graph(const char *filename)
{
	size_t numV, numE, v;
	
	ifstream fin(filename);
	fin >> numV;
	m_adj.resize(numV, vector<bool>(numV, false));
	
	for(size_t u = 0; u < numV; u++)
	{
		fin >> numE;
		for(size_t i = 0; i < numE; i++)
		{
			fin >> v;
			m_adj[u][v] = true;
			m_adj[v][u] = true;
		}
	}
}

void Graph::addVertex()
{
	for(size_t i = 0; i < m_adj.size(); i++)
		m_adj[i].push_back(false);
	m_adj.push_back(vector<bool>(m_adj[0].size(), false));
}

void Graph::addEdge(size_t u, size_t v)
{
	m_adj[u][v] = true;
	m_adj[v][u] = true;
}

bool Graph::hasEdge(size_t u, size_t v) const
{
	return m_adj[u][v];
}

size_t Graph::V() const
{
	return m_adj.size();
}
