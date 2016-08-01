#include "chaitanya-kothapalli.h"
#include "util.h"
using namespace std;

// virtual
const char *ChaitanyaKothapalli::name()
{
	return "Chaitanya-Kothapalli";
}

// virtual
void ChaitanyaKothapalli::getBiCC(const Graph &g, vector< set<size_t> > &bicc)
{
	Graph t, nt;
	vector<size_t> parent, level;
	vector< vector<size_t> > components;
	
	g.spanningTree(&t, &nt, &parent, &level);
	removeBridges(g, t, nt, parent, level, components);
	
	for(size_t i = 0; i < components.size(); i++) // parallelize
	{
		Graph aux, tPrime, ntPrime;
		vector<size_t> parentPrime, levelPrime, alias;
		vector< vector<size_t> > componentsPrime;
		
		auxiliaryGraph(g, nt, parent, level, components[i], aux, alias);
		aux.spanningTree(&tPrime, &ntPrime, &parentPrime, &levelPrime);
		removeBridges(aux, tPrime, ntPrime, parentPrime, levelPrime, componentsPrime);
		
		for(size_t j = 0; j < componentsPrime.size(); j++)
		{
			set<size_t> component;
			for(size_t k = 0; k < componentsPrime[j].size(); k++)
			{
				if(componentsPrime[j][k] < g.V())
					component.insert(componentsPrime[j][k]);
				else
					component.insert(alias[componentsPrime[j][k] - g.V()]);
			}
			if(component.size() > 1)
				bicc.push_back(component);
		}
	}
}

void ChaitanyaKothapalli::removeBridges(const Graph &g, const Graph &t, const Graph &nt, vector<size_t> &parent, vector<size_t> &level, vector< vector<size_t> > &components)
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
		if(!marked[i])
			gPrime.removeEdgeSafe(i, parent[i]);
	
	gPrime.spanningTree(NULL, NULL, NULL, NULL, &components);
}

void ChaitanyaKothapalli::auxiliaryGraph(const Graph &g, const Graph &nt, const vector<size_t> &parent, const vector<size_t> &level, const vector<size_t> &component, Graph &aux, vector<size_t> &alias)
{
	aux = g;
	for(size_t i = 0; i < component.size(); i++)
	{
		size_t u = component[i];
		for(list<size_t>::const_iterator j = nt.adj(u).begin(); j != nt.adj(u).end(); ++j)
		{
			size_t v = *j, a, b;
			if(u < v)
			{
				size_t x = LCA(parent, level, u, v, &a, &b), xPrime = aux.V();
				alias.push_back(x);
				aux.removeEdgeSafe(x, a);
				aux.removeEdgeSafe(x, b);
				aux.addVertex();
				aux.addEdge(xPrime, a);
				aux.addEdge(xPrime, b);
			}
		}
	}
}
