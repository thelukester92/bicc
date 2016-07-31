#include <iostream>
#include <vector>
#include <tuple>
#include <set>
#include <map>
using namespace std;

typedef size_t Vertex;
typedef pair<Vertex, Vertex> Edge;

// assumes edges are 1) directed and 2) sorted lexicographically
// todo: remove these assumptions
vector<Edge> eulerTour(const vector<Edge> &edges)
{
	map<Vertex, Edge> first;
	map<Edge, Edge> next;
	vector<Edge> succ(edges.size());
	
	Vertex previous = -1;
	for(size_t i = 0; i < edges.size(); i++) // can be parallelized
	{
		Vertex u = edges[i].first;
		if(u != previous)
			first[u] = edges[i];
		else
			next[edges[i - 1]] = edges[i];
	}
	
	Edge e = edges[0];
	for(size_t i = 0; i < edges.size(); i++)
	{
		succ[i] = e;
		map<Edge, Edge>::iterator f = next.find(make_pair(e.second, e.first));
		if(f != next.end())
			e = f->second;
		else
			e = first[e.second];
	}
	
	return succ;
}

/*
int main()
{
	
	
	return 0;
}
*/
