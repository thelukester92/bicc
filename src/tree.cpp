#include "tree.h"
#include <queue>
#include <tuple>
#include <set>
using std::vector;
using std::queue;
using std::pair;
using std::make_pair;
using std::set;

Tree::Tree(size_t v) : Graph(v), m_treeEdge(V())
{
	identifyTreeEdges();
}

Tree::Tree(const char *filename) : Graph(filename), m_treeEdge(V())
{
	identifyTreeEdges();
}

Tree::Tree(const Graph &g) : Graph(g), m_treeEdge(V())
{
	identifyTreeEdges();
}

const vector<bool> &Tree::treeEdges(size_t i) const
{
	return m_treeEdge[i];
}

// private
void Tree::identifyTreeEdges()
{
	vector<bool> discovered(V(), false);
	set< pair<size_t, size_t> > tEdges;
	
	for(size_t i = 0; i < V(); i++)
	{
		if(!discovered[i])
		{
			discovered[i] = true;
			queue<size_t> Q;
			Q.push(i);
			while(!Q.empty())
			{
				size_t u = Q.front();
				Q.pop();
				for(size_t j = 0; j < adj(u).size(); j++)
				{
					if(!discovered[adj(u)[j]])
					{
						discovered[adj(u)[j]] = true;
						tEdges.insert(make_pair(u, adj(u)[j]));
						Q.push(adj(u)[j]);
					}
				}
			}
		}
	}
	
	for(size_t i = 0; i < V(); i++)
	{
		m_treeEdge[i].resize(adj(i).size(), false);
		for(size_t j = 0; j < adj(i).size(); j++)
		{
			if(i < adj(i)[j])
				m_treeEdge[i][j] = tEdges.count(make_pair(i, adj(i)[j]));
			else
				m_treeEdge[i][j] = tEdges.count(make_pair(adj(i)[j], i));
		}
	}
}
