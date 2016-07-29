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
using std::make_pair;

size_t leastCommonAncestor(const Graph &t, size_t u, size_t v, size_t *c1 = NULL, size_t *c2 = NULL)
{
	if(!t.isTree())
		return leastCommonAncestor(Graph::BFSTree(t), u, v, c1, c2);
	size_t lu, lv;
	while(u != v)
	{
		if(c1) *c1 = u;
		if(c2) *c2 = v;
		lu = t.level(u);
		lv = t.level(v);
		if(lu >= lv)
			u = t.parent(u);
		if(lv >= lu)
			v = t.parent(v);
	}
	return u;
}

void removeBridges(const Graph &g, const Graph &t, const set< pair<size_t, size_t> > &nonTreeEdges, Graph &gPrime, set<size_t> &roots)
{
	gPrime = g;
	
	set< pair<size_t, size_t> > bridges;
	for(size_t i = 0; i < g.V(); i++)
		for(set<size_t>::const_iterator j = t.adj(i).begin(); j != t.adj(i).end(); ++j)
			if(i < *j)
				bridges.insert(make_pair(i, *j));
	
	for(set< pair<size_t, size_t> >::const_iterator i = nonTreeEdges.begin(); i != nonTreeEdges.end(); ++i)
	{
		size_t u = i->first, v = i->second, lu, lv;
		while(u != v)
		{
			lu = t.level(u);
			lv = t.level(v);
			if(lu >= lv)
			{
				if(u < t.parent(u))
					bridges.erase(make_pair(u, t.parent(u)));
				else
					bridges.erase(make_pair(t.parent(u), u));
				u = t.parent(u);
			}
			if(lv >= lu)
			{
				if(v < t.parent(v))
					bridges.erase(make_pair(v, t.parent(v)));
				else
					bridges.erase(make_pair(t.parent(v), v));
				v = t.parent(v);
			}
		}
	}
	
	for(set< pair<size_t, size_t> >::iterator i = bridges.begin(); i != bridges.end(); ++i)
	{
		cout << "bridge: " << i->first << " " << i->second << endl;
		gPrime.removeEdge(i->first, i->second);
	}
	
	Graph::BFSTree(gPrime, &roots);
}

Graph auxiliaryGraph(const Graph &g)
{
	set< pair<size_t, size_t> > nonTreeEdges;
	
	Graph gPrime(g);
	Graph t = Graph::BFSTree(g, NULL, &nonTreeEdges);
	
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
		cout << "Graph:\n" << g << endl;
		
		set< pair<size_t, size_t> > nonTreeEdges;
		Graph t = Graph::BFSTree(g, NULL, &nonTreeEdges);
		cout << "Tree:\n" << t << endl;
		
		Graph gPrime = auxiliaryGraph(g);
		cout << "Graph Prime:\n" << gPrime << endl;
		
		Graph f;
		set<size_t> roots;
		removeBridges(g, t, nonTreeEdges, f, roots);
		cout << "Removed bridges:\n" << f << endl << "roots:\n";
		for(set<size_t>::iterator i = roots.begin(); i != roots.end(); ++i)
			cout << *i << " ";
		cout << endl;
	}
	else
	{
		cout << "Usage: ./bicc [inputgraph]" << endl;
	}
	
	return 0;
}
