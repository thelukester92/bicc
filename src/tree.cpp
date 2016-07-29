#include "tree.h"
#include <queue>
using std::vector;
using std::queue;

Tree::Tree(size_t v) : Graph(v), m_tAdj(V(), vector<bool>(V(), false))
{
	identifyTreeEdges();
}

Tree::Tree(const char *filename) : Graph(filename), m_tAdj(V(), vector<bool>(V(), false))
{
	identifyTreeEdges();
}

Tree::Tree(const Graph &g) : Graph(g), m_tAdj(V(), vector<bool>(V(), false))
{
	identifyTreeEdges();
}

// virtual
void Tree::addVertex()
{
	Graph::addVertex();
}

// virtual
void Tree::addEdge(size_t u, size_t v)
{
	Graph::addEdge(u, v);
}

bool Tree::isTreeEdge(size_t u, size_t v) const
{
	return m_tAdj[u][v];
}

// private
void Tree::identifyTreeEdges()
{
	vector<bool> discovered(V(), false);
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
				for(size_t j = 0; j < V(); j++)
				{
					if(hasEdge(u, j) && !discovered[j])
					{
						discovered[j] = true;
						m_tAdj[u][j] = true;
						m_tAdj[j][u] = true;
						Q.push(j);
					}
				}
			}
		}
	}
}
