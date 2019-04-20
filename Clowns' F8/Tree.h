#ifndef _Tree_H__
#define _Tree_H__

#include "Object.h"
#include "Animation.h"

class Tree : public Object
{
public:
	Tree(ENTITY_TYPE _type, pugi::xml_node _config);

	// Destructor
	~Tree();


};

#endif // !__Tree_H__