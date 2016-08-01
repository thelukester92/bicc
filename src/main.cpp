#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include "tarjan-vishkin.h"
using namespace std;

int main(int argc, char **argv)
{
	if(argc < 2)
	{
		cout << "Usage: bicc [graph.txt]" << endl;
		return 0;
	}
	
	ifstream fin;
	fin.open(argv[1]);
	
	Graph g;
	size_t V, E;
	fin >> V;
	g.resize(V);
	
	for(size_t u = 0; u < V; u++)
	{
		fin >> E;
		for(size_t i = 0; i < E; i++)
		{
			size_t v;
			fin >> v;
			g.addEdge(u, v);
		}
	}
	
	fin.close();
	
	vector< set<size_t> > bicc;
	TarjanVishkin tv;
	tv.getBiCC(g, bicc);
	
	for(size_t i = 0; i < bicc.size(); i++)
	{
		cout << i << ": ";
		for(set<size_t>::iterator j = bicc[i].begin(); j != bicc[i].end(); ++j)
			cout << g.vertex(*j) << " ";
		cout << endl;
	}
	
	return 0;
}
