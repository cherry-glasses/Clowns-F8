#ifndef _Tree2_H__
#define _Tree2_H__

#include "Object.h"

class Tree2 : public Object
{
public:
	Tree2(ENTITY_TYPE _type, pugi::xml_node _config, int _copy);

	// Destructor
	~Tree2();

};

#endif // !__Tree2_H__