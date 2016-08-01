#include "util.h"
using namespace std;

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

Edge reverseEdge(const Edge &e)
{
	return Edge(e.second, e.first);
}

size_t LCA(vector<size_t> &parent, vector<size_t> &level, size_t u, size_t v)
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

void prefixSum(vector<size_t> &v)
{
	size_t sum = v[0];
	for(size_t i = 1; i < v.size(); i++) // parallelize
	{
		sum += v[i];
		v[i] = sum;
	}
}
