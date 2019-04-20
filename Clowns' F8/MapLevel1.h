#ifndef _MapLevel1_H__
#define _MapLevel1_H__

#include "Object.h"
#include "Animation.h"

class MapLevel1 : public Object
{
public:
	MapLevel1(ENTITY_TYPE _type, pugi::xml_node _config);

	// Destructor
	~MapLevel1();


};

#endif // !__MapLevel1_H__