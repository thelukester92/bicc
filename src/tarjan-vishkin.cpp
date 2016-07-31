#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <map>
#include <tuple>
#include <algorithm>
#include <queue>
using namespace std;

typedef pair<size_t, size_t> Edge;
Edge reverseEdge(const Edge &e)
{
	return Edge(e.second, e.first);
}

class Graph
{
public:
	void resize(size_t v)
	{
		for(size_t i = m_adj.size(); i < v; i++)
			m_vertices.push_back(i == 0 ? 'a' : m_vertices.back() + 1);
		m_adj.resize(v);
	}
	
	void addVertex()					{ m_adj.push_back(list<size_t>()); m_vertices.push_back(m_adj.size() == 0 ? 'a' : m_vertices.back() + 1); }
	void addEdge(size_t u, size_t v)	{ addDirectedEdge(u, v); addDirectedEdge(v, u); }
	
	// adds edge (u, v), keeping m_edges sorted lexicographically
	void addDirectedEdge(size_t u, size_t v)
	{
		Edge e(u, v);
		m_edges.insert(lower_bound(m_edges.begin(), m_edges.end(), e), e);
		m_adj[u].push_back(v);
	}
	
	list<size_t> &adj(size_t u)					{ return m_adj[u]; }
	const list<size_t> &adj(size_t u) const		{ return m_adj[u]; }
	const vector<Edge> &edges() const			{ return m_edges; }
	char vertex(size_t u) const					{ return m_vertices[u]; }
	size_t V() const							{ return m_adj.size(); }
	size_t E() const							{ return m_edges.size(); }
private:
	vector< list<size_t> > m_adj;
	vector<Edge> m_edges;
	vector<char> m_vertices;
};
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

void spanningTree(const Graph &g, Graph &t, Graph &nt)
{
	vector<bool> visited(g.V(), false), discovered(g.V(), false);
	
	queue<size_t> q;
	q.push(0);
	discovered[0] = true;
	
	t.resize(g.V());
	nt.resize(g.V());
	
	while(!q.empty())
	{
		size_t u = q.front();
		q.pop();
		
		if(visited[u])
			continue;
		visited[u] = true;
		
		for(list<size_t>::const_iterator i = g.adj(u).begin(); i != g.adj(u).end(); ++i)
		{
			size_t v = *i;
			if(!visited[v])
			{
				if(!discovered[v])
				{
					discovered[v] = true;
					q.push(v);
					t.addEdge(u, v);
				}
				else
					nt.addEdge(u, v);
			}
		}
	}
}

void eulerTour(const Graph &t, vector<size_t> &succ)
{
	const vector<Edge> &edges = t.edges();
	vector<size_t> first(t.V(), -1), next(edges.size(), -1), twin(edges.size(), -1);
	
	for(size_t i = 0; i < edges.size(); i++) // parallelizable
	{
		twin[distance(edges.begin(), lower_bound(edges.begin(), edges.end(), reverseEdge(edges[i])))] = i;
		if(i == 0 || edges[i].first != edges[i-1].first)
			first[edges[i].first] = i;
		else if(edges[i].first == edges[i-1].first)
			next[i-1] = i;
	}
	
	succ.resize(edges.size());
	for(size_t i = 0; i < edges.size(); i++) // parallelizable
	{
		if(next[twin[i]] != -1)
			succ[i] = next[twin[i]];
		else
			succ[i] = first[edges[i].second];
	}
}

void TV(const Graph &g)
{
	Graph t, nt;
	vector<size_t> succ;
	
	spanningTree(g, t, nt);
	eulerTour(t, succ);
	
	cout << "Graph:\n" << g << endl;
	cout << "MST:\n" << t << endl;
	cout << "NTE:\n" << nt << endl;
	cout << "Tour:\n";
	size_t i = 0;
	do
	{
		cout << t.vertex(t.edges()[i].first) << ", " << t.vertex(t.edges()[i].second) << endl;
		i = succ[i];
	}
	while(i != 0);
}

int main(int argc, char **argv)
{
	if(argc < 2)
	{
		cout << "Usage: bicc [graph.txt]" << endl;
		return 0;
	}
	
	ifstream fin;
	fin.open(argv[1]);
	
	Graph g;
	size_t V, E;
	fin >> V;
	g.resize(V);
	
	for(size_t u = 0; u < V; u++)
	{
		fin >> E;
		for(size_t i = 0; i < E; i++)
		{
			size_t v;
			fin >> v;
			g.addEdge(u, v);
		}
	}
	
	fin.close();
	
	TV(g);
	
	return 0;
}
