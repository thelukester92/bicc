#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <list>
using namespace std;

class Graph
{
public:
	Graph(size_t v = 0) : m_adj(v)			{}
	void resize(size_t v)					{ m_adj.resize(v); }
	void addVertex()						{ m_adj.push_back(list<size_t>()); }
	void addEdge(size_t u, size_t v)		{ m_adj[u].push_back(v); m_adj[v].push_back(u); }
	const list<size_t> &adj(size_t u) const	{ return m_adj[u]; }
	size_t V() const						{ return m_adj.size(); }
private:
	vector< list<size_t> > m_adj;
};

void rerootCB(const Graph &t, size_t r, size_t u, vector< vector<size_t> > &PR)
{
	
}

// returns a rooted spanning tree T and Euler tour E
void CB(const Graph &g)
{
	// shared
	vector<size_t> D(g.V()), winner(g.V()), parent(g.V());
	vector< vector<size_t> > PR(g.V());
	
	// processor ID
	size_t K = 0;
	
	for(size_t i = 0; i < g.V(); i++) // parallel
		D[i] = i;
	
	for(size_t u = 0; u < g.V(); u++) // parallel
	{
		for(size_t i = 0; i < g.adj(u).size(); i++) // parallel
		{
			size_t v = g.adj(u)[i];
			if(D[v] < D[u])
				winner[D[u]] = K;
		}
	}
	
	for(size_t u = 0; u < g.V(); u++) // parallel
	{
		for(size_t i = 0; i < g.adj(u).size(); i++) // parallel
		{
			size_t v = g.adj(u)[i];
			if(D[v] < D[u] && winner[D[u]] == K)
			{
				D[D[u]] = D[v];
				parent[u] = v;
				// label u as the new root of the subtree
			}
		}
	}
	
	for(size_t u = 0; u < g.V(); u++) // parallel
		if(D[u] != D[D[u]])
			PR[u].push_back(D[u]);
	
	// above original algorithm was:
	// j = 0
	// for each vertex u
	//     if D[u] != D[D[u]]
	//         PR[j] = D[i] // nonsense, PR[j] is a list!
	//         j++
	
	// for each u that was labeled as the new root of its subtree
		// call alg. 1 to reroot the subtree
}

int main(int argc, char **argv)
{
	if(argc < 2)
	{
		cout << "Usage: bicc [graph.txt]" << endl;
		return 0;
	}
	
	Graph g;
	size_t v, V, E;
	
	ifstream fin;
	fin.open(argv[1]);
	fin >> V;
	g.resize(V);
	
	for(size_t u = 0; u < V; u++)
	{
		fin >> E;
		for(size_t j = 0; j < E; j++)
		{
			fin >> v;
			g.addEdge(u, v);
		}
	}
	
	CB(g);
	
	return 0;
}
