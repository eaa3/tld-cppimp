#ifndef _SERIALIZABLE_
#define _SERIALIZABLE_

#include <iostream>
#include <fstream>

using std::ostream;
using std::istream;


class Serializable {
public:



	virtual void save(ostream& out) const = 0 ;
	virtual void restore(istream& in) = 0;
	

};

#endif