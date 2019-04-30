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
	TransitionState state;

	float transition_time; // total transition entering and exiting time
	Timer* current_time = nullptr;



	float percent = 0; //percent of the current respect the total time. It goes from 0 to 1


public:
	bool todestroy = false;
	Transition(float transition_time);
	~Transition();

	void PostUpdate();


	virtual void Entering();
	virtual void Action();
	virtual void Exiting();

	float LerpValue(float percent, float start, float end);

};

#endif // _Transition_H_