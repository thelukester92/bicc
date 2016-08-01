#include "chaitanya-kothapalli.h"
using namespace std;

// virtual
const char *ChaitanyaKothapalli::name()
{
	return "Chaitanya-Kothapalli";
}

// virtual
void ChaitanyaKothapalli::getBiCC(const Graph &g, vector< set<size_t> > &bicc)
{
	g.spanningTree(&t, &nt, &parent, &level);
	removeBridges(g, gComponents);
	
	// todo: loop over each gi in gComponents
	
	
	/*
	set< set<size_t> > components;
	set< pair<size_t, size_t> > nonTreeEdges;
	
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
	*/
}

void ChaitanyaKothapalli::removeBridges(const Graph &g, vector< vector<size_t> > &components)
{
	Graph gPrime(g);
	
	vector<bool> marked(t.V(), false);
	size_t u, v, lu, lv;
	
	for(size_t i = 0; i < nt.E(); i++)
	{
		u = nt.edges()[i].first, v = nt.edges()[i].second;
		while(u != v)
		{
			lu = level[u];
			lv = level[v];
			if(lu >= lv)
			{
				marked[u] = true;
				u = parent[u];
			}
			if(lv >= lu)
			{
				marked[v] = true;
				v = parent[v];
			}
		}
	}
	
	for(size_t i = 1; i < g.V(); i++)
		if(marked[i])
			gPrime.removeEdge(i, parent[i]);
	
	gPrime.spanningTree(NULL, NULL, NULL, NULL, &components);
}
