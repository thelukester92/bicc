#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include "tarjan-vishkin.h"
#include "chaitanya-kothapalli.h"
#include "CK-TV.h"
#include "util.h"
using namespace std;

#define DEBUG

// Loads a graph from a file
// The first line of the file is the number of vertices
// Remaining lines are the edges [fromVertex toVertex]
void loadGraph(const char *filename, Graph &g)
{
	ifstream fin;
	fin.open(filename);
	
	size_t V, u, v;
	fin >> V;
	g.resize(V);
	
	while(!fin.fail())
	{
		fin >> u >> v;
		if(!fin.fail())
			g.addDirectedEdge(u, v);
	}
	
	fin.close();
}

int main(int argc, char **argv)
{
	if(argc < 2)
	{
		cout << "Usage: bicc [graph.txt]" << endl;
		return 0;
	}
	
	Graph g;
	loadGraph(argv[1], g);
	
	vector<BiCC *> algorithms;
	algorithms.push_back(new TarjanVishkin());
	algorithms.push_back(new ChaitanyaKothapalli());
	algorithms.push_back(new CKTV());
	
	for(size_t i = 0; i < algorithms.size(); i++)
	{
		cout << "===== " << algorithms[i]->name() << " =====" << endl;
		
		vector< set<size_t> > bicc;
		clock_t start = clock();
		algorithms[i]->getBiCC(g, bicc);
		
		cout << "Found " << bicc.size() << " biconnected components in " << double(clock() - start) / CLOCKS_PER_SEC << " seconds!" << endl;
		
#ifdef DEBUG
		for(size_t j = 0; j < bicc.size(); j++)
		{
			cout << j << ": ";
			for(set<size_t>::iterator k = bicc[j].begin(); k != bicc[j].end(); ++k)
				cout << char('a' + *k) << " ";
			cout << endl;
		}
		
		set<size_t> articulationPoints;
		biccToArticulationPoints(g, bicc, articulationPoints);
		
		cout << "Articulation points: ";
		for(set<size_t>::iterator j = articulationPoints.begin(); j != articulationPoints.end(); ++j)
			cout << char('a' + *j) << " ";
		cout << endl;
#endif
		
		delete algorithms[i];
	}
	
	return 0;
}
