#include "graph.h"
#include "tree.h"
#include <iostream>
using std::cout;
using std::endl;

int main(int argc, char **argv)
{
	if(argc > 1)
	{
		cout << "Graph:" << endl;
		Graph g(argv[1]);
		for(size_t i = 0; i < g.V(); i++)
		{
			for(size_t j = 0; j < g.adj(i).size(); j++)
				cout << g.adj(i)[j] << " ";
			cout << endl;
		}
		cout << endl;
		
		cout << "Tree:" << endl;
		Tree t(g);
		for(size_t i = 0; i < t.V(); i++)
		{
			for(size_t j = 0; j < t.treeEdges(i).size(); j++)
				cout << t.treeEdges(i)[j] << " ";
			cout << endl;
		}
	}
	else
	{
		cout << "Usage: ./bicc [inputgraph]" << endl;
	}
	
	return 0;
}
