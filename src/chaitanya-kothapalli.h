#ifndef CHAITANYA_KOTHAPALLI_H
#define CHAITANYA_KOTHAPALLI_H

#include "bicc.h"

class ChaitanyaKothapalli : public BiCC
{
public:
	virtual const char *name();
	virtual void getBiCC(const Graph &g, std::vector< std::set<size_t> > &bicc);
};

#endif
