#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <queue>
using namespace std;

struct Edge
{
	explicit Edge(size_t u = -1, size_t v = -1, bool isTreeEdge = false) : u(u), v(v), isTreeEdge(isTreeEdge) {}
	size_t u, v;
	bool isTreeEdge;
};

class Graph
{
public:
	Graph(size_t V) : m_adj(V) {}
	void addEdge(size_t u, size_t v)			{ m_adj[u].push_back(v); m_adj[v].push_back(u); }
	vector<size_t> &adj(size_t u)				{ return m_adj[u]; }
	const vector<size_t> &adj(size_t u) const	{ return m_adj[u]; }
	size_t V() const							{ return m_adj.size(); }
private:
	vector< vector<size_t> > m_adj;
};

void spanningTree(const Graph &g, vector<Edge> &edges)
{
	vector<bool> visited(g.V(), false), discovered(g.V(), false);
	queue<size_t> q;
	
	discovered[0] = true;
	q.push(0);
	
	while(!q.empty())
	{
		size_t u = q.front();
		q.pop();
		
		if(visited[u])
			continue;
		visited[u] = true;
		
		for(size_t i = 0; i < g.adj(u).size(); i++)
		{
			size_t v = g.adj(u)[i];
			if(!visited[v])
			{
				if(!discovered[v])
				{
					discovered[v] = true;
					q.push(v);
					edges.push_back(Edge(u, v, true));
				}
				else
					edges.push_back(Edge(u, v, false));
			}
		}
	}
}

void eulerTour(const Graph &g, const vector<Edge> &edges, vector<Edge> &tour)
{
	// todo: order these lexicographically...
	vector<Edge> directedEdges;
	for(size_t i = 0; i < edges.size(); i++)
	{
		if(edges[i].isTreeEdge)
		{
			directedEdges.push_back(edges[i]);
			directedEdges.push_back(Edge(edges[i].v, edges[i].u));
		}
	}
	
	vector<size_t> first(g.V(), -1), next(directedEdges.size(), -1), succ(directedEdges.size(), -1);
	size_t e;
	
	size_t prev = -1;
	for(size_t i = 0; i < directedEdges.size(); i++)
	{
		if(directedEdges[i].u != prev)
		{
			cout << "first[" << directedEdges[i].u << "] = " << i << endl;
			first[directedEdges[i].u] = i;
			e = i;
		}
		else
		{
			cout << "next[" << e << "] = " << i << endl;
			next[e] = i;
			e = i;
		}
		prev = directedEdges[i].u;
	}
	
	cout << "-----" << endl;
	for(size_t i = 0; i < directedEdges.size(); i++)
	{
		cout << directedEdges[i].u << ", " << directedEdges[i].v << " -> " << directedEdges[next[i]].u << ", " << directedEdges[next[i]].v << endl;
		
		size_t j = i % 2 == 0 ? i + 1 : i - 1;
		if(next[j] != -1)
			succ[i] = next[j];
		else
			succ[i] = first[directedEdges[j].u];
	}
	
	tour.resize(succ.size());
	e = first[0];
	
	for(size_t i = 0; i < tour.size(); i++)
	{
		tour[i] = directedEdges[e];
		e = succ[e];
	}
}

void preorder(const Graph &g, const vector<Edge> &tour, vector<size_t> &order)
{
	order.resize(g.V(), -1);
	
	size_t j = 0;
	order[tour[0].u] = j++;
	
	for(size_t i = 0; i < tour.size(); i++)
		if(order[tour[i].v] == -1)
			order[tour[i].v] = j++;
}

void aux(const Graph &g, const vector<Edge> &t, const vector<Edge> &nt, const vector<size_t> &order, Graph &gp)
{
	
}

void TV(const Graph &g)
{
	vector<Edge> edges, tour;
	vector<size_t> order;
	
	spanningTree(g, edges);
	eulerTour(g, edges, tour);
	preorder(g, tour, order);
	
	cout << "=====" << endl;
	for(size_t i = 0; i < tour.size(); i++)
		cout << i << ": " << tour[i].u << ", " << tour[i].v << endl;
	
	cout << "=====" << endl;
	for(size_t i = 0; i < order.size(); i++)
		cout << i << ": " << order[i] << endl;
	
	// aux(g, t, nt, order, gp);
	
	/*
	c = components(gp);
	*/
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
	
	size_t v;
	fin >> v;
	
	Graph g(v);
	
	for(size_t i = 0; i < v; i++)
	{
		size_t e;
		fin >> e;
		for(size_t j = 0; j < e; j++)
		{
			size_t v;
			fin >> v;
			g.addEdge(i, v);
		}
	}
	
	TV(g);
	
	return 0;
}
