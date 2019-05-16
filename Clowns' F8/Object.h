#ifndef __Object_H__
#define __Object_H__

#include "Animation.h"
#include "Entity.h"

class Object : public Entity
{
public:
	Object(ENTITY_TYPE _type, pugi::xml_node _config, int _copy);

	// Destructor
	virtual ~Object();

	// Called before the first frame
	virtual bool Start() { return true; }

	// Called each loop iteration
	virtual bool PreUpdate();
	virtual bool Update(float _dt);
	virtual bool PostUpdate(float _dt);


	// Called before quitting
	virtual bool CleanUp() { return true; }

	//Save and Load
	virtual bool Load(pugi::xml_node&);
	virtual bool Save(pugi::xml_node&) const;

public:
	bool used = false;

protected:
	std::pair<int, int> nearposition;

};

#endif // !__Object_H__
