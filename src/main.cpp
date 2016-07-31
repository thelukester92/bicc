#include "graph.h"
#include <iostream>
#include <set>
#include <map>
#include <vector>
#include <tuple>
#include <stdexcept>
using std::cout;
using std::endl;
using std::ostream;
using std::set;
using std::vector;
using std::pair;
using std::make_pair;
using std::map;
using std::invalid_argument;

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

vector<Graph> removeBridges(const Graph &g, const Graph &t, const set< pair<size_t, size_t> > &nonTreeEdges)
{
	Graph gPrime(g);
	
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
		gPrime.removeEdge(i->first, i->second);
	
	vector< vector<size_t> > components;
	Graph::BFSTree(gPrime, &components);
	
	vector<Graph> forest(components.size());
	for(size_t i = 0; i < components.size(); i++)
		forest[i] = Graph(gPrime, components[i]);
	
	return forest;
}

Graph auxiliaryGraph(const Graph &g)
{
	set< pair<size_t, size_t> > nonTreeEdges;
	
	Graph gPrime(g);
	Graph t = Graph::BFSTreeRooted(g, 0, &nonTreeEdges);
	
	for(set< pair<size_t, size_t> >::iterator j = nonTreeEdges.begin(); j != nonTreeEdges.end(); ++j)
	{
		size_t b1, b2;
		size_t x = leastCommonAncestor(t, j->first, j->second, &b1, &b2);
		size_t xPrime = gPrime.V();
		gPrime.removeEdge(x, b1);
		gPrime.removeEdge(x, b2);
		gPrime.addVertex(g.vertex(x));
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
		cout << char('a' + g.vertex(i)) << ": ";
		for(set<size_t>::const_iterator j = g.adj(i).begin(); j != g.adj(i).end(); ++j)
			out << char('a' + g.vertex(*j)) << " ";
		out << endl;
	}
}

ostream &operator<<(ostream &out, const Graph &g)
{
	printGraph(g, out);
	return out;
}

set< set<size_t> > biconnectedComponents(const Graph &g)
{
	set< set<size_t> > components;
	set< pair<size_t, size_t> > nonTreeEdges;
	Graph t = Graph::BFSTree(g, NULL, &nonTreeEdges);
	
	vector<Graph> forest = removeBridges(g, t, nonTreeEdges);
	for(size_t i = 0; i < forest.size(); i++) // todo: parallelize this loop
	{
		Graph gPrime = auxiliaryGraph(forest[i]);
		
		set< pair<size_t, size_t> > nonTreeEdgesPrime;
		Graph tPrime = Graph::BFSTreeRooted(gPrime, 0, &nonTreeEdgesPrime);
		vector<Graph> fPrime = removeBridges(gPrime, tPrime, nonTreeEdgesPrime);
		
		for(size_t j = 0; j < fPrime.size(); j++)
		{
			set<size_t> component;
			for(size_t k = 0; k < fPrime[j].V(); k++)
				component.insert(fPrime[j].vertex(k));
			if(component.size() > 1)
				components.insert(component);
		}
	}
	
	return components;
}

vector< pair<size_t, size_t> > eulerianTour(const Graph &t)
{
	if(!t.isTree()) throw invalid_argument("can only get eulerian tour from a tree");
	
	vector< pair<size_t, size_t> > tour;
	map< size_t, pair<size_t, size_t> > first;
	map< pair<size_t, size_t>, pair<size_t, size_t> > next, succ;
	pair<size_t, size_t> prev, curr;
	
	for(size_t i = 0; i < t.V(); i++)
	{
		for(set<size_t>::const_iterator j = t.adj(i).begin(); j != t.adj(i).end(); ++j)
		{
			if(j == t.adj(i).begin())
			{
				prev = make_pair(i, *j);
				first[i] = prev;
			}
			else
			{
				next[prev] = make_pair(i, *j);
				prev = next[prev];
			}
		}
	}
	
	for(size_t i = 0; i < t.V(); i++)
	{
		for(set<size_t>::const_iterator j = t.adj(i).begin(); j != t.adj(i).end(); ++j)
		{
			curr = make_pair(i, *j);
			map< pair<size_t, size_t>, pair<size_t, size_t> >::iterator s = next.find(make_pair(*j, i));
			if(s != next.end())
				succ[curr] = s->second;
			else
				succ[curr] = first[*j];
		}
	}
	
	tour.push_back(first[0]);
	while(succ[tour.back()] != first[0])
	{
		tour.push_back(succ[tour.back()]);
	}
	
	return tour;
}

Graph tarjanVishkinAuxiliaryGraph(const Graph &g, const vector< pair<size_t, size_t> > &eTour)
{
	set< pair<size_t, size_t> > nonTreeEdges;
	Graph tPrime = Graph::BFSTreeRooted(g, 0, &nonTreeEdges);
	
	Graph gPrime(g); // to keep aliases; optimize this
	for(size_t i = 0; i < nonTreeEdges.size(); i++)
		gPrime.addVertex();
	for(size_t i = 0; i < tPrime.V(); i++)
		gPrime.adj(i).clear();
	
	vector<size_t> N(gPrime.V(), 0);
	vector<size_t> order(g.V(), -1);
	size_t j = 0;
	
	order[eTour[0].first] = j++;
	for(size_t i = 0; i < eTour.size(); i++) // todo: parallelize
	{
		if(order[eTour[i].second] == -1)
			order[eTour[i].second] = j++;
	}
	
	for(size_t i = 0; i < gPrime.V(); i++) // todo: parallelize
	{
		if(...)
	}
	
	// first, all tree edges
	for(size_t i = 0; i < tPrime.V(); i++)
	{
		for(set<size_t>::const_iterator j = tPrime.adj(i).begin(); j != tPrime.adj(i).end(); ++j)
		{
			if(order[*j] < order[i])
				gPrime.addEdge(i, g.V() + nd[i] - 1);
			if(leastCommonAncestor(tPrime, i, *j) == 0 && i != 0 && *j != 0)
				gPrime.addEdge(i, *j);
		}
	}
	
	// next, all non-tree edges
	for(set< pair<size_t, size_t> >::iterator i = nonTreeEdges.begin(); i != nonTreeEdges.end(); ++i)
	{
		if(i->first != 0 && i->second != 0)
			gPrime.addEdge(i->first, i->second);
	}
	
	cout << "====" << endl << gPrime << endl;
	
	return gPrime;
}

set< set<size_t> > tarjanVishkin(const Graph &g)
{
	set< set<size_t> > components;
	set< pair<size_t, size_t> > nonTreeEdges;
	Graph t = Graph::BFSTree(g, NULL, &nonTreeEdges);
	
	vector< vector<size_t> > cPrime;
	vector< pair<size_t, size_t> > eTour = eulerianTour(t);
	
	Graph gPrime = tarjanVishkinAuxiliaryGraph(g, eTour); // parallelize this function
	Graph::BFSTree(gPrime, &cPrime);
	
	for(size_t j = 0; j < cPrime.size(); j++)
	{
		set<size_t> component;
		for(size_t k = 0; k < cPrime[j].size(); k++)
			component.insert(g.vertex(cPrime[j][k]));
		if(component.size() > 1)
			components.insert(component);
	}
	
	return components;
}

int main(int argc, char **argv)
{
	if(argc > 1)
	{
		Graph g(argv[1]);
		cout << "Graph:\n" << g << endl;
		
		set< set<size_t> > components = biconnectedComponents(g);
		cout << "Biconnected components:" << endl;
		for(set< set<size_t> >::iterator i = components.begin(); i != components.end(); ++i)
		{
			for(set<size_t>::iterator j = i->begin(); j != i->end(); ++j)
				cout << char('a' + *j) << " ";
			cout << endl;
		}
		cout << endl;
		
		components = tarjanVishkin(g);
		cout << "Tarjan Vishkin:" << endl;
		for(set< set<size_t> >::iterator i = components.begin(); i != components.end(); ++i)
		{
			for(set<size_t>::iterator j = i->begin(); j != i->end(); ++j)
				cout << char('a' + *j) << " ";
			cout << endl;
		}
	}
	else
	{
		cout << "Usage: ./bicc [inputgraph]" << endl;
	}
	
	return 0;
}
