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
	if (current_state == ALIVE) {

		objective_position.clear();
		objective_position.push_back(position);

		if (current_stats.Hp <= 0)
		{
			Die();
		}

		if (current_turn == SEARCH_MOVE)
		{
			SearchWalk();
		}
		else if (current_turn == SEARCH_ATTACK)
		{
			SearchAttack();
		}
		else if (current_turn == SEARCH_ABILITY_1) {
			SearchAbility_1();
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

bool Enemy::PostUpdate()
{
	if (entity_texture != nullptr)
	{
		App->render->Blit(entity_texture, position.first - (current.w/2) + position_margin.first, position.second - current.h + position_margin.second, &current_animation->GetCurrentFrame(), 1.0f, flipX);
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
		CurrentMovement(DEAD_RIGHT_BACK);
	}


}

