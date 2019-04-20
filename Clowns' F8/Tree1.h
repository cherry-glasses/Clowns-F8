#ifndef _Tree1_H__
#define _Tree1_H__

#include "Object.h"
#include "Animation.h"

class Tree1 : public Object
{
public:
	Tree1(ENTITY_TYPE _type, pugi::xml_node _config, int _copy);

	// Destructor
	~Tree1();

};

#endif // !__Tree1_H__