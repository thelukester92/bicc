#include "graph.h"
#include <queue>
#include <fstream>
#include <stdexcept>
using std::vector;
using std::set;
using std::pair;
using std::make_pair;
using std::queue;
using std::ifstream;
using std::invalid_argument;

// static
Graph Graph::BFSTree(const Graph &g, set< pair<size_t, size_t> > *nonTreeEdges)
{
	Graph t(g.V());
	t.m_parent.resize(g.V());
	t.m_level.resize(g.V());
	t.m_isTree = true;
	
	vector<bool> discovered(g.V(), false);
	vector<bool> visited(g.V(), false);
	for(size_t i = 0; i < g.V(); i++)
	{
		if(!discovered[i])
		{
			discovered[i] = true;
			t.m_parent[i] = i;
			t.m_level[i] = 0;
			queue<size_t> Q;
			Q.push(i);
			while(!Q.empty())
			{
				size_t u = Q.front();
				Q.pop();
				
				if(visited[u])
					continue;
				visited[u] = true;
				
				for(set<size_t>::const_iterator j = g.adj(u).begin(); j != g.adj(u).end(); ++j)
				{
					if(!visited[*j])
					{
						if(!discovered[*j])
						{
							discovered[*j] = true;
							t.m_parent[*j] = u;
							t.m_level[*j] = t.m_level[u] + 1;
							t.m_adj[u].insert(*j);
							t.m_adj[*j].insert(u);
							Q.push(*j);
						}
						else if(nonTreeEdges)
						{
							if(u < *j)
								nonTreeEdges->insert(make_pair(u, *j));
							else
								nonTreeEdges->insert(make_pair(*j, u));
						}
					}
				}
			}
		}
	}
	return t;
}

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
		for(size_t i = 0; i < numE; i++)
		{
			fin >> v;
			m_adj[u].insert(v);
			m_adj[v].insert(u);
		}
	}
}

void Graph::addVertex()
{
	m_adj.push_back(set<size_t>());
}

void Graph::addEdge(size_t u, size_t v)
{
	m_adj[u].insert(v);
	m_adj[v].insert(u);
}

void Graph::removeEdge(size_t u, size_t v)
{
	set<size_t>::iterator i = m_adj[u].find(v);
	if(i != m_adj[u].end()) m_adj[u].erase(i);
	
	i = m_adj[v].find(u);
	if(i != m_adj[v].end()) m_adj[v].erase(i);
}

const set<size_t> &Graph::adj(size_t i) const
{
	return m_adj[i];
}

size_t Graph::V() const
{
	return m_adj.size();
}

size_t Graph::parent(size_t i) const
{
	if(!m_isTree) throw invalid_argument("can only get parent in a tree");
	return m_parent[i];
}

size_t Graph::level(size_t i) const
{
	if(!m_isTree) throw invalid_argument("can only get level in a tree");
	return m_level[i];
}

bool Graph::isTree() const
{
	return m_isTree;
}
