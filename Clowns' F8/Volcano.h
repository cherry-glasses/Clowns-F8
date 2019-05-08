#ifndef _Volcano_H__
#define _Volcano_H__

#include "Object.h"

class Volcano : public Object
{
public:
	Volcano(ENTITY_TYPE _type, pugi::xml_node _config);

	// Destructor
	~Volcano();

};

#endif // !__Volcano_H__