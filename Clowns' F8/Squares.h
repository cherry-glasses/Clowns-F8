#ifndef __Squares_H_
#define __Squares_H_

#include "Transition.h"
#include "Scene.h"

class Squares :
	public Transition
{
private:
	Color color = Black;
	bool is_scene_change = false;
	SCENE_TYPE scene_to_change = SCENE_TYPE::NONE;

	uint width, height, square_width, square_height;
	std::vector<std::pair<int,int>>positions;

	int total_squares_col = 16;
	int total_squares_row = 9;

public:
	Squares(float transition_time, bool is_scene_change = false, SCENE_TYPE scene_to_change = SCENE_TYPE::NONE, Color color = Black);
	~Squares();

	void Entering();
	void Action();
	void Exiting();

	void SetColor(Color color);
	void DrawSquares(int num_Squares);
};

#endif // !__Squares_H_