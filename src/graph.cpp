#include "graph.h"
#include <fstream>
using std::vector;
using std::string;
using std::ifstream;

Graph::Graph(size_t v) : m_adj(v)
{}

Graph::Graph(const char *filename)
{
	size_t numV, numE, v;
	
	ifstream fin(filename);
	fin >> numV;
	m_adj.resize(numV);
	
	for(size_t u = 0; u < numV; u++)
	{
		fin >> numE;
		m_adj[u].resize(numE);
		for(size_t i = 0; i < numE; i++)
		{
			fin >> v;
			m_adj[u][i] = v;
		}
	}
}

vector<size_t> &Graph::adj(size_t i)
{
	return m_adj[i];
}

size_t Graph::V() const
{
	return m_adj.size();
}
