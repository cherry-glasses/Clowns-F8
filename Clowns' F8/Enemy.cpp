#include "Log.h"
#include "Application.h"
#include "Enemy.h"
#include "ModuleEntityManager.h"
#include "ModulePathfinding.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleMap.h"


bool Enemy::PreUpdate()
{
	if (current_state == ALIVE && App->entity_manager->ThereAreCharAlive()) {

		objective_position.clear();
		objective_position.push_back(position);

		if (current_stats.Hp <= 0)
		{
			Die();
		}
		else
		{
			if (current_turn == SEARCH_MOVE)
			{
				++start;
				if (start == START_TURN)
				{
					start = 0;
					SearchWalk();
				}
			}
			else if (current_turn == SEARCH_ATTACK)
			{
				SearchAttack();
			}
			else if (current_turn == SEARCH_ABILITY_1) {
				SearchAbility_1();
			}
		}
	}
	else if (current_turn == SEARCH_MOVE)
	{
		current_turn = END_TURN;
	}
	

	return true;
}

bool Enemy::Update(float dt)
{
	if (current_turn == NONE)
	{
		return true;
	}
	else if (current_turn == MOVE)
	{
		Walk(App->pathfinding->GetLastPath());
	}
	else if (current_turn == ATTACK)
	{
		Attack(App->pathfinding->GetLastPath());
	}
	else if (current_turn == ABILITY_1) {
		Ability_1(App->pathfinding->GetLastPath());
	}

	
	return true;
}


// Load and Save
bool Enemy::Load(pugi::xml_node& node)
{
	bool ret = true;

	position.first = node.child("position").attribute("x").as_float(0);
	position.second = node.child("position").attribute("y").as_float(0);

	return ret;
}

bool Enemy::Save(pugi::xml_node& node) const
{
	bool ret = true;

	pugi::xml_node pos = node.append_child("position");
	pos.append_attribute("x") = position.first;
	pos.append_attribute("y") = position.second;

	return ret;
}

std::pair<int, int> Enemy::Path2_0(std::pair<int, int> position, std::pair<int, int> objective)
{
	std::pair<int, int> returner = objective;
	if ((this)->type == ENTITY_TYPE::ENTITY_ENEMY_HOTDOG)
	{

	}
	else 
	{
		std::pair<int, int> helper;
		helper.first = objective.first - position.first;
		helper.second = objective.second - position.second;
		int helper_2 = sqrt(helper.first*helper.first + helper.second*helper.second);
		

		int i = 1;
		
		while (App->pathfinding->IsUsed(returner, this)) {
			if (helper.first > 0 && helper.second == 0) // derecha
			{
				returner.first = objective.first - i;
				returner.second = objective.second;
			}
			if (helper.first < 0 && helper.second == 0) // izquierda
			{
				returner.first = objective.first + i;
				returner.second = objective.second;
			}
			if (helper.first == 0 && helper.second > 0) // arriba
			{
				returner.first = objective.first;
				returner.second = objective.second - i;
			}
			if (helper.first == 0 && helper.second < 0) // abajo
			{
				returner.first = objective.first;
				returner.second = objective.second + i;
			}
			if (helper.first > 0 && helper.second > 0) // diagonal / arriba
			{
				returner.first = objective.first - i;
				returner.second = objective.second - i;
			}
			if (helper.first < 0 && helper.second < 0) // diagonal / abajo
			{
				returner.first = objective.first + i;
				returner.second = objective.second + i;
			}
			if (helper.first < 0 && helper.second > 0) // diagonal \ arriba
			{
				returner.first = objective.first + i;
				returner.second = objective.second - i;
			}
			if (helper.first > 0 && helper.second < 0) // diagonal \ abajo
			{
				returner.first = objective.first - i;
				returner.second = objective.second + i;
			}
			i++;
		}


		
	}


	return returner;
}


void Enemy::Die()
{

	if (current_movement == IDLE_LEFT_FRONT)
	{
		CurrentMovement(DEAD_LEFT_FRONT);
	}
	else if (current_movement == IDLE_RIGHT_FRONT)
	{
		CurrentMovement(DEAD_RIGHT_FRONT);
	}
	else if (current_movement == IDLE_LEFT_BACK)
	{
		CurrentMovement(DEAD_LEFT_BACK);
	}
	else if (current_movement == IDLE_BACK) {
		CurrentMovement(DEAD_BACK);
	}
	else if (current_movement == IDLE_FRONT) {
		CurrentMovement(DEAD_FRONT);
	}
	else if (current_movement == IDLE_RIGHT) {
		CurrentMovement(DEAD_RIGHT);
	}
	else if (current_movement == IDLE_LEFT) {
		CurrentMovement(DEAD_LEFT);
	}
	else if (current_movement == DEAD_BACK) {
		CurrentMovement(DEAD_BACK);
	}
	else if (current_movement == DEAD_FRONT) {
		CurrentMovement(DEAD_FRONT);
	}
	else if (current_movement == DEAD_RIGHT) {
		CurrentMovement(DEAD_RIGHT);
	}
	else if (current_movement == DEAD_LEFT) {
		CurrentMovement(DEAD_LEFT);
	}
	else
	{
		CurrentMovement(DEAD_DEFAULT);
	}


}

