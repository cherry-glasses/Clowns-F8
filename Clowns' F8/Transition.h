#ifndef __Transition_H_
#define __Transition_H_

#include "Timer.h"
#include "SDL\include\SDL_rect.h"
#include "Color.h"

class Transition
{
private:
	enum class TransitionState {
		NONE,
		ENTERING,
		ACTION,
		EXITING
	};

protected:
	TransitionState state = TransitionState::NONE;

	float transition_time;
	Timer* current_time = nullptr;


public:

	Transition();
	Transition(float transition_time);
	~Transition();


	void PreUpdate();
	void Update();
	void PostUpdate();


	virtual void Entering();
	virtual void Action();
	virtual void Exiting();

};

#endif // _Transition_H_