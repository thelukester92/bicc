#include "graph.h"
#include <queue>
using namespace std;

void Graph::resize(size_t v)
{
	for(size_t i = m_adj.size(); i < v; i++)
		m_vertices.push_back(i == 0 ? 'a' : m_vertices.back() + 1);
	m_adj.resize(v);
}

void Graph::addVertex()
{
	m_adj.push_back(list<size_t>());
	m_vertices.push_back(m_adj.size() == 0 ? 'a' : m_vertices.back() + 1);
}

void Graph::addEdge(size_t u, size_t v)
{
	addDirectedEdge(u, v);
	addDirectedEdge(v, u);
}

// adds edge (u, v), keeping m_edges sorted lexicographically
void Graph::addDirectedEdge(size_t u, size_t v)
{
	Edge e(u, v);
	m_edges.insert(lower_bound(m_edges.begin(), m_edges.end(), e), e);
	m_adj[u].push_back(v);
}

list<size_t> &Graph::adj(size_t u)
{
	return m_adj[u];
}

const list<size_t> &Graph::adj(size_t u) const
{
	return m_adj[u];
}

const vector<Edge> &Graph::edges() const
{
	return m_edges;
}

char Graph::vertex(size_t u) const
{
	return m_vertices[u];
}

size_t Graph::V() const
{
	return m_adj.size();
}

size_t Graph::E() const
{
	return m_edges.size();
}

void Graph::spanningTree(Graph *t, Graph *nt, vector<size_t> *parent, vector<size_t> *level, vector< vector<size_t> > *components) const
{
	vector<bool> visited(V(), false), discovered(V(), false);
	
	queue<size_t> q;
	q.push(0);
	discovered[0] = true;
	
	if(t)
		t->resize(V());
	if(nt)
		nt->resize(V());
	
	for(size_t i = 0; i < V(); i++)
	{
		if(!visited[i])
		{
			q.push(i);
			discovered[i] = true;
			
			if(parent)
			{
				parent->resize(V());
				(*parent)[i] = i;
			}
			if(level)
			{
				level->resize(V());
				(*level)[i] = i;
			}
			if(components)
				components->push_back(vector<size_t>(1, i));
			
			while(!q.empty())
			{
				size_t u = q.front();
				q.pop();
				
				if(visited[u])
					continue;
				visited[u] = true;
				
				for(list<size_t>::const_iterator j = adj(u).begin(); j != adj(u).end(); ++j)
				{
					size_t v = *j;
					if(!visited[v])
					{
						if(!discovered[v])
						{
							discovered[v] = true;
							q.push(v);
							
							if(t)
								t->addEdge(u, v);
							if(parent)
								(*parent)[v] = u;
							if(level)
								(*level)[v] = (*level)[u] + 1;
							if(components)
								components->back().push_back(*j);
						}
						else if(nt)
							nt->addEdge(u, v);
					}
				}
			}
		}
	}
}
