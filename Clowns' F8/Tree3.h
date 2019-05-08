#ifndef _Tree3_H__
#define _Tree3_H__

#include "Object.h"

class Tree3 : public Object
{
public:
	Tree3(ENTITY_TYPE _type, pugi::xml_node _config, int _copy);

	// Destructor
	~Tree3();

};

#endif // !__Tree3_H__