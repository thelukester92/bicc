#ifndef CHAITANYA_KOTHAPALLI_H
#define CHAITANYA_KOTHAPALLI_H

#include "bicc.h"

class ChaitanyaKothapalli : public BiCC
{
public:
	virtual const char *name();
	virtual void getBiCC(const Graph &g, std::vector< std::set<size_t> > &bicc);
private:
	void removeBridges(const Graph &g, std::vector< std::vector<size_t> > &components);
	
	Graph t, nt;
	std::vector<size_t> parent, level;
	std::vector< std::vector<size_t> > gComponents;
};

#endif
