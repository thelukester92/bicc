#include "graph.h"
#include <iostream>
#include <set>
#include <vector>
#include <tuple>
using std::cout;
using std::endl;
using std::ostream;
using std::set;
using std::vector;
using std::pair;

size_t leastCommonAncestor(const Graph &t, size_t u, size_t v, size_t *c1 = NULL, size_t *c2 = NULL)
{
	if(!t.isTree())
		return leastCommonAncestor(Graph::BFSTree(t), u, v, c1, c2);
	while(t.level(u) > t.level(v)) u = t.parent(u);
	while(t.level(v) > t.level(u)) v = t.parent(v);
	while(u != v)
	{
		if(c1) *c1 = u;
		if(c2) *c2 = v;
		u = t.parent(u);
		v = t.parent(v);
	}
	return u;
}

/*
AUX(G)
	{
		G' = copy of G
		Tg = BFS(G) -- optimize this part --
		for each non-tree edge e = (u, v) in G \ Tg do
			x = LCA(u, v)
			remove (x, u) and (x, v) from G'
			add x' to G'
			add (x, x'), (x', u), and (x', v) to G'
	}
*/

Graph auxiliaryGraph(const Graph &g)
{
	set< pair<size_t, size_t> > nonTreeEdges;
	
	Graph gPrime(g);
	Graph t = Graph::BFSTree(g, &nonTreeEdges);
	
	for(set< pair<size_t, size_t> >::iterator j = nonTreeEdges.begin(); j != nonTreeEdges.end(); ++j)
	{
		size_t b1, b2;
		size_t x = leastCommonAncestor(t, j->first, j->second, &b1, &b2);
		size_t xPrime = gPrime.V();
		gPrime.removeEdge(x, b1);
		gPrime.removeEdge(x, b2);
		gPrime.addVertex();
		gPrime.addEdge(x, xPrime);
		gPrime.addEdge(xPrime, b1);
		gPrime.addEdge(xPrime, b2);
	}
	
	return gPrime;
}

void printGraph(const Graph &g, ostream &out = cout)
{
	for(size_t i = 0; i < g.V(); i++)
	{
		cout << char('a' + i) << ": ";
		for(set<size_t>::const_iterator j = g.adj(i).begin(); j != g.adj(i).end(); ++j)
			out << char('a' + *j) << " ";
		out << endl;
	}
}

ostream &operator<<(ostream &out, const Graph &g)
{
	printGraph(g, out);
	return out;
}

int main(int argc, char **argv)
{
	if(argc > 1)
	{
		Graph g(argv[1]);
		g.removeEdge(1, 5);
		cout << "Graph:\n" << g << endl;
		
		Graph t = Graph::BFSTree(g);
		cout << "Tree:\n" << t << endl;
		
		Graph gPrime = auxiliaryGraph(g);
		cout << "Graph Prime:\n" << gPrime << endl;
	}
	else
	{
		cout << "Usage: ./bicc [inputgraph]" << endl;
	}
	
	return 0;
}
