#ifndef _Stone_H__
#define _Stone_H__

#include "Object.h"

class Stone : public Object
{
public:
	Stone(ENTITY_TYPE _type, pugi::xml_node _config, int _copy);

	// Destructor
	~Stone();

};

#endif // !__Stone_H__