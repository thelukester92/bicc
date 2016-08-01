#include "graph.h"
#include <queue>
using namespace std;

Edge reverseEdge(const Edge &e)
{
	return Edge(e.second, e.first);
}

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

void Graph::spanningTree(Graph &t, Graph &nt, std::vector<size_t> &parent, std::vector<size_t> &level) const
{
	vector<bool> visited(V(), false), discovered(V(), false);
	
	queue<size_t> q;
	q.push(0);
	discovered[0] = true;
	
	t.resize(V());
	nt.resize(V());
	parent.resize(V());
	level.resize(V());
	parent[0] = 0;
	level[0] = 0;
	
	while(!q.empty())
	{
		size_t u = q.front();
		q.pop();
		
		if(visited[u])
			continue;
		visited[u] = true;
		
		for(list<size_t>::const_iterator i = adj(u).begin(); i != adj(u).end(); ++i)
		{
			size_t v = *i;
			if(!visited[v])
			{
				if(!discovered[v])
				{
					discovered[v] = true;
					q.push(v);
					t.addEdge(u, v);
					parent[v] = u;
					level[v] = level[u] + 1;
				}
				else
					nt.addEdge(u, v);
			}
		}
	}
}

void Graph::connectedComponents(std::vector< std::vector<size_t> > &components) const
{
	vector<bool> visited(V(), false), discovered(V(), false);
	queue<size_t> q;
	for(size_t i = 0; i < V(); i++)
	{
		if(!visited[i])
		{
			components.push_back(vector<size_t>(1, i));
			q.push(i);
			discovered[i] = true;
			while(!q.empty())
			{
				size_t u = q.front();
				q.pop();
				
				if(visited[u])
					continue;
				visited[u] = true;
				
				for(list<size_t>::const_iterator j = adj(u).begin(); j != adj(u).end(); ++j)
				{
					if(!visited[*j] && !discovered[*j])
					{
						discovered[*j] = true;
						q.push(*j);
						components.back().push_back(*j);
					}
				}
			}
		}
	}
}

ostream &operator<<(ostream &out, const Graph &g)
{
	for(size_t i = 0; i < g.V(); i++)
	{
		cout << g.vertex(i) << ": ";
		for(list<size_t>::const_iterator j = g.adj(i).begin(); j != g.adj(i).end(); ++j)
			cout << g.vertex(*j) << " ";
		cout << endl;
	}
	return out;
}
