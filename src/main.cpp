#include "graph.h"
#include <iostream>
#include <list>
#include <vector>
using std::cout;
using std::endl;
using std::ostream;
using std::list;
using std::vector;

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

size_t leastCommonAncestor(const Graph &t, size_t u, size_t v)
{
	if(!t.isTree())
		return leastCommonAncestor(Graph::BFSTree(t), u, v);
	
	while(t.level(u) > t.level(v)) u = t.parent(u);
	while(t.level(v) > t.level(u)) v = t.parent(v);
	while(u != v)
	{
		u = t.parent(u);
		v = t.parent(v);
	}
	return u;
}

/*
Graph auxiliaryGraph(const Graph &g)
{
	Graph gPrime(g);
	Tree t(g);
	
	for(size_t i = 0; i < g.V(); i++)
	{
		
		
		for(size_t j = i+1; j < g.V(); j++)
		{
			if(g.hasEdge(i, j) && !t.isTreeEdge(i, j))
			{
				cout << i << ", " << j << endl;
				
				size_t x = leastCommonAncestor(g, i, j);
				size_t b1 = i, b2 = j; // todo: fixme
				gPrime.removeEdge(x, b1);
				gPrime.removeEdge(x, b2);
				
				size_t xPrime = gPrime.addVertex();
				gPrime.addEdge(x, xPrime);
				gPrime.addEdge(xPrime, b1);
				gPrime.addEdge(xPrime, b2);
				
				// todo: what if multiple cycles share LCA?
			}
		}
	}
	
	return gPrime;
}
*/

void printGraph(const Graph &g, ostream &out = cout)
{
	for(size_t i = 0; i < g.V(); i++)
	{
		for(list<size_t>::const_iterator j = g.adj(i).begin(); j != g.adj(i).end(); ++j)
			out << *j << " ";
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
		cout << "Graph:\n" << g << endl;
		
		Graph t = Graph::BFSTree(g);
		cout << "Tree:\n" << t << endl;
		
		for(size_t i = 0; i < t.V(); i++)
			for(size_t j = i+1; j < t.V(); j++)
				cout << "LCA(" << i << ", " << j << ") = " << leastCommonAncestor(t, i, j) << endl;
		
		/*
		cout << "Graph Prime:" << endl;
		Graph gPrime = auxiliaryGraph(g);
		cout << gPrime << endl;
		*/
	}
	else
	{
		cout << "Usage: ./bicc [inputgraph]" << endl;
	}
	
	return 0;
}
