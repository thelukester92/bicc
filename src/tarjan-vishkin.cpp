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

void spanningTree(const Graph &g, Graph &t, Graph &nt, vector<size_t> &parent, vector<size_t> &level)
{
	vector<bool> visited(g.V(), false), discovered(g.V(), false);
	
	queue<size_t> q;
	q.push(0);
	discovered[0] = true;
	
	t.resize(g.V());
	nt.resize(g.V());
	parent.resize(g.V());
	level.resize(g.V());
	parent[0] = 0;
	level[0] = 0;
	
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
					parent[v] = u;
					level[v] = level[u] + 1;
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

void preorderVertices(const Graph &t, const vector<size_t> &succ, vector<size_t> &preorder)
{
	preorder.resize(t.V(), -1);
	preorder[t.edges()[0].first] = 0;
	preorder[t.edges()[0].second] = 1;
	
	size_t i = 0, j = 2;
	do
	{
		size_t k = t.edges()[succ[i]].second;
		if(preorder[k] == -1)
			preorder[t.edges()[succ[i]].second] = j++;
		i = succ[i];
	}
	while(i != 0);
}

void findLow(const Graph &t, const Graph &nt, const vector<size_t> &level, vector<size_t> &low)
{
	low.resize(nt.V());
	for(size_t i = 0; i < nt.V(); i++) // parallelize
		low[i] = i;
	
	bool changed = true;
	while(changed)
	{
		changed = false;
		for(size_t i = 0; i < nt.V(); i++)
		{
			for(list<size_t>::const_iterator j = t.adj(i).begin(); j != t.adj(i).end(); ++j)
			{
				if(level[i] < level[*j] && low[*j] < low[i])
				{
					low[i] = low[*j];
					changed = true;
				}
			}
			for(list<size_t>::const_iterator j = nt.adj(i).begin(); j != nt.adj(i).end(); ++j)
			{
				if(low[*j] < low[i])
				{
					low[i] = low[*j];
					changed = true;
				}
			}
		}
	}
}

void prefixSum(vector<size_t> &v)
{
	size_t sum = v[0];
	for(size_t i = 1; i < v.size(); i++) // parallelize
	{
		sum += v[i];
		v[i] = sum;
	}
}

size_t LCA(const vector<size_t> &parent, const vector<size_t> &level, size_t u, size_t v)
{
	size_t lu, lv;
	while(u != v)
	{
		lu = level[u];
		lv = level[v];
		if(lu >= lv)
			u = parent[u];
		if(lv >= lu)
			v = parent[v];
	}
	return u;
}

void auxiliaryGraph(const Graph &g, const Graph &t, const Graph &nt, const vector<size_t> &parent, const vector<size_t> &level, const vector<size_t> &preorder, const vector<size_t> &low, Graph &gPrime)
{
	gPrime.resize(g.V() + nt.E() / 2);
	vector<size_t> N(g.E(), 0);
	
	size_t ti = 0, nti = 0;
	for(size_t i = 0; i < g.E(); i++) // to parallelize, need a vector<bool> isTreeEdge
	{
		if(nti >= nt.E() || g.edges()[i] == t.edges()[ti])
			ti++;
		else
		{
			if(g.edges()[i].first < g.edges()[i].second)
				N[i] = 1;
			nti++;
		}
	}
	
	prefixSum(N);
	
	ti = 0, nti = 0;
	for(size_t i = 0; i < g.E(); i++) // to parallelize, need a vector<bool> isTreeEdge
	{
		size_t u = g.edges()[i].first, v = g.edges()[i].second;
		if(nti >= nt.edges().size() || g.edges()[i] == t.edges()[ti])
		{
			if(preorder[u] < preorder[v] && low[v] < preorder[u])
				gPrime.addEdge(u, v);
			ti++;
		}
		else
		{
			size_t p = LCA(parent, level, u, v);
			if(preorder[v] < preorder[u])
				gPrime.addEdge(u, N[i] + g.V() - 1);
			if(u < v && p != u && p != v)
				gPrime.addEdge(u, v);
			nti++;
		}
	}
}

void connectedComponents(const Graph &g, vector< vector<size_t> > &components)
{
	vector<bool> visited(g.V(), false), discovered(g.V(), false);
	queue<size_t> q;
	for(size_t i = 0; i < g.V(); i++)
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
				
				for(list<size_t>::const_iterator j = g.adj(u).begin(); j != g.adj(u).end(); ++j)
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

void TV(const Graph &g)
{
	Graph t, nt, gPrime;
	vector<size_t> succ, parent, level, preorder, low;
	vector< vector<size_t> > components;
	
	spanningTree(g, t, nt, parent, level);
	eulerTour(t, succ);
	preorderVertices(t, succ, preorder);
	findLow(t, nt, level, low);
	auxiliaryGraph(g, t, nt, parent, level, preorder, low, gPrime);
	connectedComponents(gPrime, components);
	
	cout << "Graph:\n" << g << endl;
	cout << "MST:\n" << t << endl;
	cout << "NTE:\n" << nt << endl;
	cout << "Preorder:\n";
	for(size_t i = 0; i < g.V(); i++)
		cout << g.vertex(i) << ": " << preorder[i] << endl;
	cout << endl;
	cout << "Auxiliary:\n" << gPrime << endl;
	cout << "Components:\n";
	for(size_t i = 0; i < components.size(); i++)
	{
		cout << i << ": ";
		for(size_t j = 0; j < components[i].size(); j++)
			cout << components[i][j] << " ";
		cout << endl;
	}
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
