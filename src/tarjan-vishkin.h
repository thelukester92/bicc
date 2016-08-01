#ifndef TARJAN_VISHKIN_H
#define TARJAN_VISHKIN_H

#include "bicc.h"

class TarjanVishkin : public BiCC
{
public:
	virtual const char *name();
	virtual void getBiCC(const Graph &g, std::vector< std::set<size_t> > &bicc);
private:
	void eulerTour();
	void preorderVertices();
	void findLow();
	void prefixSum(std::vector<size_t> &v);
	size_t LCA(size_t u, size_t v);
	void auxiliaryGraph(const Graph &g);
	void remapAuxiliaryGraph(std::vector< std::set<size_t> > &bicc);
	
	Graph t, nt, gPrime;
	std::vector<size_t> succ, parent, level, preorder, low;
	std::vector< std::vector<size_t> > components;
};

#endif
