#include "Application.h"
#include "Character.h"
#include "ModuleRender.h"
#include "ModuleMap.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModulePathfinding.h"
#include "ModuleAudio.h"
#include "ModuleParticleSystem.h"
#include "Emitter.h"

bool Character::PreUpdate() {

	if (current_state == ALIVE) {

		if (current_stats.Hp <= 0)
		{
			Die();
		}

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
		else if (current_turn == SEARCH_ABILITY_1)
		{
			SearchAbility_1();
		}
		else if (current_turn == SEARCH_ABILITY_2)
		{
			SearchAbility_2();
		}
		else if (current_turn == SEARCH_ABILITY_3)
		{
			SearchAbility_3();
		}
	}
	else if (current_turn == SEARCH_MOVE)
	{
		current_turn = END_TURN;
	}

	return true;
}
bool Character::Update(float _dt) {

	if (current_turn == NONE)
	{
	}
	else if (current_turn == SELECT_MOVE)
	{
		SelectWalk();
	}
	else if (current_turn == SELECT_ATTACK)
	{
		SelectAttack();
	}
	else if (current_turn == SELECT_ABILITY_1)
	{
		SelectAbility_1();
	}
	else if (current_turn == SELECT_ABILITY_2)
	{
		SelectAbility_2();
	}
	else if (current_turn == SELECT_ABILITY_3)
	{
		SelectAbility_3();
	}
	else if (current_turn == MOVE)
	{
		Walk();
	}
	else if (current_turn == ATTACK)
	{
		if (critic) 
		{
			current_stats.AtkF = current_stats.AtkF * 1.25;
			current_stats.AtkS = current_stats.AtkS * 1.25;
			Attack();
			current_stats.AtkF = current_stats.AtkF / 1.25;
			current_stats.AtkS = current_stats.AtkS / 1.25;
		}
		else 
		{
			Attack();
		}
	}
	else if (current_turn == ABILITY_1)
	{
		if (critic)
		{
			current_stats.AtkF = current_stats.AtkF * 1.25;
			current_stats.AtkS = current_stats.AtkS * 1.25;
			Ability_1();
			current_stats.AtkF = current_stats.AtkF / 1.25;
			current_stats.AtkS = current_stats.AtkS / 1.25;
		}
		else
		{
			Ability_1();
		}
	}
	else if (current_turn == ABILITY_2)
	{
		if (critic)
		{
			current_stats.AtkF = current_stats.AtkF * 1.25;
			current_stats.AtkS = current_stats.AtkS * 1.25;
			Ability_2();
			current_stats.AtkF = current_stats.AtkF / 1.25;
			current_stats.AtkS = current_stats.AtkS / 1.25;
		}
		else
		{
			Ability_2();
		}
	}
	else if (current_turn == ABILITY_3)
	{
		if (critic)
		{
			current_stats.AtkF = current_stats.AtkF * 1.25;
			current_stats.AtkS = current_stats.AtkS * 1.25;
			Ability_3();
			current_stats.AtkF = current_stats.AtkF / 1.25;
			current_stats.AtkS = current_stats.AtkS / 1.25;
		}
		else
		{
			Ability_3();
		}
	}
	else if (current_turn == DEFEND)
	{
		Defend();
	}

	if (Cap != -1 && possible_map.size() > 0)
	{
		target = possible_map.at(Cap);
		possible_map.clear();
	}
	if (current_turn == END_TURN)
	{
		EndTurn();
	}
	
	return true;
}



bool Character::Load(pugi::xml_node& node) {
	
	return true;
}

bool Character::Save(pugi::xml_node& node) const {
	
	return true;
}

void Character::SelectWalk() {

	if (Cap == -1) {
		std::pair<int, int> tmp = App->map->WorldToMap(position.first, position.second);
		Cap = tmp.first + (tmp.second * 25);
	}

	int i = 0;
	for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
	{
		possible_map.push_back(App->map->MapToWorld((*possible_mov).first, (*possible_mov).second));

		if (i != Cap && (std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov)) != inrange_mov_list.end()
			|| std::find(nomov_list.begin(), nomov_list.end(), (*possible_mov)) != nomov_list.end()))
		{
			if (App->pathfinding->IsWalkable({ (*possible_mov).first , (*possible_mov).second })
				&& !App->pathfinding->IsUsed({ (*possible_mov).first , (*possible_mov).second }, this)
				&& std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov)) != inrange_mov_list.end())
			{
				App->render->Blit(debug_texture, possible_map.at(i).first, possible_map.at(i).second, &debug_blue);
			}
			else
			{
				App->render->Blit(debug_texture, possible_map.at(i).first, possible_map.at(i).second, &debug_red);
			}
		}
		++i;
	}
	App->render->Blit(debug_texture, possible_map.at(Cap).first, possible_map.at(Cap).second, &debug_green);

	InputSelectMove();

	if (App->input->Accept()
		&& std::find(inrange_mov_list.begin(), inrange_mov_list.end(), App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second)) != inrange_mov_list.end()
		&& App->pathfinding->IsWalkable(App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second))
		&& !App->pathfinding->IsUsed(App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second), this)) {

		current_turn = Entity::MOVE;
		comeback_position = { position.first, position.second };
		comeback_movement = current_movement;
		
	}
}

void Character::Walk()
{
	int i = 0;
	for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
	{
		if (i == Cap)
		{
			objective_position.push_back(App->map->MapToWorld((*possible_mov).first, (*possible_mov).second));
		}
		++i;
	}

	if (objective_position.back().first < position.first && objective_position.back().second > position.second) {
		CurrentMovement(WALK_LEFT_FRONT);
	}
	else if (objective_position.back().first > position.first && objective_position.back().second > position.second) {
		CurrentMovement(WALK_RIGHT_FRONT);
	}
	else if (objective_position.back().first < position.first && objective_position.back().second < position.second) {
		CurrentMovement(WALK_LEFT_BACK);
	}
	else if (objective_position.back().first > position.first && objective_position.back().second < position.second) {
		CurrentMovement(WALK_RIGHT_BACK);
	}
	else if (objective_position.back().first == position.first && objective_position.back().second > position.second) {
		CurrentMovement(WALK_FRONT);
	}
	else if (objective_position.back().first == position.first && objective_position.back().second < position.second) {
		CurrentMovement(WALK_BACK);
	}
	else if (objective_position.back().first < position.first && objective_position.back().second == position.second) {
		CurrentMovement(WALK_LEFT);
	}
	else if (objective_position.back().first > position.first && objective_position.back().second == position.second) {
		CurrentMovement(WALK_RIGHT);
	}
	else {
		current_turn = SELECT_ACTION;
	}

	// Ending walk and start idle animation
	if (objective_position.back().first == position.first && objective_position.back().second == position.second) {
		EndTurn();
		if (current_movement == WALK_LEFT_FRONT)
		{
			CurrentMovement(IDLE_LEFT_FRONT);
		}
		else if (current_movement == WALK_RIGHT_FRONT)
		{
			CurrentMovement(IDLE_RIGHT_FRONT);
		}
		else if (current_movement == WALK_LEFT_BACK)
		{
			CurrentMovement(IDLE_LEFT_BACK);
		}
		else if (current_movement == WALK_RIGHT_BACK)
		{
			CurrentMovement(IDLE_RIGHT_BACK);
		}
		else if (current_movement == WALK_LEFT)
		{
			CurrentMovement(IDLE_LEFT);
		}
		else if (current_movement == WALK_RIGHT)
		{
			CurrentMovement(IDLE_RIGHT);
		}
		else if (current_movement == WALK_FRONT)
		{
			CurrentMovement(IDLE_FRONT);
		}
		else if (current_movement == WALK_BACK)
		{
			CurrentMovement(IDLE_BACK);
		}
		current_turn = SELECT_ACTION;
		
	}

}

void Character::SelectAttack() {

	if (Cap == -1) {
		std::pair<int, int> tmp = App->map->WorldToMap(position.first, position.second);
		Cap = tmp.first + (tmp.second * 25);
	}

	int i = 0;
	for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
	{
		possible_map.push_back(App->map->MapToWorld((*possible_mov).first, (*possible_mov).second));

		if (i != Cap && std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov)) != inrange_mov_list.end())
		{
			if (App->pathfinding->IsAttackable({ (*possible_mov).first , (*possible_mov).second } , type))
			{
				App->render->Blit(debug_texture, possible_map.at(i).first, possible_map.at(i).second, &debug_blue);
			}
			else
			{
				App->render->Blit(debug_texture, possible_map.at(i).first, possible_map.at(i).second, &debug_red);
			}
		}
		++i;
	}
	App->render->Blit(debug_texture, possible_map.at(Cap).first, possible_map.at(Cap).second, &debug_green);

	InputSelectMove();
	

	if (App->input->Accept() && App->pathfinding->IsAttackable(App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second) , type)
		&& std::find(inrange_mov_list.begin(), inrange_mov_list.end(), App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second)) != inrange_mov_list.end())	
	{
		current_turn = ATTACK;
		objective_position.push_back({ possible_map.at(Cap).first, possible_map.at(Cap).second });
	}
	else if (App->input->Decline()) 
	{
		EndTurn();
		current_turn = SELECT_ACTION;
	}

}

void Character::Attack()
{
	if (!sound_fx) {
		App->audio->PlayFx(sfx.Attack_SFX);
		sound_fx = true;
	}
	if (!finish_attack)
	{
		if (objective_position.back().first < position.first && objective_position.back().second > position.second) {
			CurrentMovement(ATTACK_LEFT_FRONT);
		}
		else if (objective_position.back().first > position.first && objective_position.back().second > position.second) {
			CurrentMovement(ATTACK_RIGHT_FRONT);
		}
		else if (objective_position.back().first < position.first && objective_position.back().second < position.second) {
			CurrentMovement(ATTACK_LEFT_BACK);
		}
		else if (objective_position.back().first > position.first && objective_position.back().second < position.second) {
			CurrentMovement(ATTACK_RIGHT_BACK);
		}
		else if (objective_position.back().first == position.first && objective_position.back().second > position.second) {
			CurrentMovement(ATTACK_FRONT);
		}
		else if (objective_position.back().first == position.first && objective_position.back().second < position.second) {
			CurrentMovement(ATTACK_BACK);
		}
		else if (objective_position.back().first < position.first && objective_position.back().second == position.second) {
			CurrentMovement(ATTACK_LEFT);
		}
		else if (objective_position.back().first > position.first && objective_position.back().second == position.second) {
			CurrentMovement(ATTACK_RIGHT);
		}
		else {
			CurrentMovement(ATTACK_RIGHT_FRONT);
		}
	}
	if (emitter == nullptr)
	{
		switch (type)
		{
		case ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE:
			if (current_animation->GetCurrentFrameIndex() == 1)
			{
				emitter = App->particle_system->AddEmiter({ position.first + 50, position.second - 80 }, EmitterType::EMITTER_TYPE_ATTACK);
				emitter->SetTextureRect({ 356, 78, 16, 14 });
			}
			break;
		case ENTITY_TYPE::ENTITY_CHARACTER_STORM:
			if (current_animation->GetCurrentFrameIndex() == 2)
			{
				emitter = App->particle_system->AddEmiter({ position.first + 16, position.second - 16 }, EmitterType::EMITTER_TYPE_ATTACK);
				emitter->SetTextureRect({ 356, 78, 16, 14 });
				emitter->SetColor({ 100, 80, 10, 255 }, { 100, 80, 10, 255 }, SDL_BlendMode::SDL_BLENDMODE_BLEND);
			}
			break;
		case ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB:
			if (current_animation->GetCurrentFrameIndex() == 2)
			{
				emitter = App->particle_system->AddEmiter({ position.first + 32, position.second - 16 }, EmitterType::EMITTER_TYPE_ATTACK);
				emitter->SetTextureRect({ 364, 94, 7, 7 });
			}
			break;
		default:
			break;
		}
	}
	if (emitter != nullptr)
	{

		if (emitter->GetEmitterPos().first < objective_position.back().first + 10)
			emitter->SetPosition({ emitter->GetEmitterPos().first + 4, emitter->GetEmitterPos().second });
		if (emitter->GetEmitterPos().first > objective_position.back().first + 10)
			emitter->SetPosition({ emitter->GetEmitterPos().first - 4, emitter->GetEmitterPos().second });
		if (emitter->GetEmitterPos().second < objective_position.back().second - 16)
			emitter->SetPosition({ emitter->GetEmitterPos().first, emitter->GetEmitterPos().second + 4 });
		if (emitter->GetEmitterPos().second > objective_position.back().second - 16)
			emitter->SetPosition({ emitter->GetEmitterPos().first, emitter->GetEmitterPos().second - 4 });
		if ((emitter->GetEmitterPos().first <= objective_position.back().first + 20) && (emitter->GetEmitterPos().first >= objective_position.back().first)
			&& (emitter->GetEmitterPos().second >= objective_position.back().second - 32) && (emitter->GetEmitterPos().second <= objective_position.back().second))
		{
			emitter->StopEmission();
			emitter = nullptr;
		}
	}
	// Ending attack and start idle animation
	if (finish_attack && emitter == nullptr) {
		
		EndTurn();
		if (current_movement == ATTACK_LEFT_FRONT)
		{
			CurrentMovement(IDLE_LEFT_FRONT);
		}
		else if (current_movement == ATTACK_RIGHT_FRONT)
		{
			CurrentMovement(IDLE_RIGHT_FRONT);
		}
		else if (current_movement == ATTACK_LEFT_BACK)
		{
			CurrentMovement(IDLE_LEFT_BACK);
		}
		else if (current_movement == ATTACK_RIGHT_BACK)
		{
			CurrentMovement(IDLE_RIGHT_BACK);
		}
		else if (current_movement == ATTACK_LEFT)
		{
			CurrentMovement(IDLE_LEFT);
		}
		else if (current_movement == ATTACK_RIGHT)
		{
			CurrentMovement(IDLE_RIGHT);
		}
		else if (current_movement == ATTACK_FRONT)
		{
			CurrentMovement(IDLE_FRONT);
		}
		else if (current_movement == ATTACK_BACK)
		{
			CurrentMovement(IDLE_BACK);
		}
	}
}

void Character::SelectAbility_1() {

	if (Cap == -1) {
		std::pair<int, int> tmp = App->map->WorldToMap(position.first, position.second);
		Cap = tmp.first + (tmp.second * 25);
	}

	for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
	{
		possible_map.push_back(App->map->MapToWorld((*possible_mov).first, (*possible_mov).second));
	}

	int i = 0;
	int mod = sqrt(possible_mov_list.size());
	for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
	{

		if (i != Cap && std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov)) != inrange_mov_list.end())
		{
			if (App->pathfinding->IsAttackable({ (*possible_mov).first , (*possible_mov).second }, type) 
				|| (type == ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB && App->pathfinding->IsWalkable({ (*possible_mov).first , (*possible_mov).second })
					&& App->pathfinding->CanTrap({ (*possible_mov).first , (*possible_mov).second })))
			{
				App->render->Blit(debug_texture, possible_map.at(i).first, possible_map.at(i).second, &debug_blue);
			}
			else
			{
				App->render->Blit(debug_texture, possible_map.at(i).first, possible_map.at(i).second, &debug_red);
			}
		}

		switch (type)
		{
		case ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE:
			if (std::find(inrange_mov_list.begin(), inrange_mov_list.end(), App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second)) != inrange_mov_list.end()
				&& std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov)) != inrange_mov_list.end())
			{
				if (i == Cap + 1)
				{
					App->render->Blit(debug_texture, possible_map.at(Cap + 1).first, possible_map.at(Cap + 1).second, &debug_green);
				}
				else if (i == Cap - 1)
				{
					App->render->Blit(debug_texture, possible_map.at(Cap - 1).first, possible_map.at(Cap - 1).second, &debug_green);
				}
				else if (i == Cap + mod)
				{
					App->render->Blit(debug_texture, possible_map.at(Cap + mod).first, possible_map.at(Cap + mod).second, &debug_green);
				}
				else if (i == Cap - mod)
				{
					App->render->Blit(debug_texture, possible_map.at(Cap - mod).first, possible_map.at(Cap - mod).second, &debug_green);
				}
			}
			break;
		case ENTITY_TYPE::ENTITY_CHARACTER_IRIS:
			if (i == Cap && std::find(inrange_mov_list.begin(), inrange_mov_list.end(), App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second)) != inrange_mov_list.end()) {
				if (std::find(inrange_mov_list.begin(), inrange_mov_list.end(), App->map->WorldToMap(possible_map.at(Cap - 2).first, possible_map.at(Cap - 2).second)) != inrange_mov_list.end()
					|| std::find(inrange_mov_list.begin(), inrange_mov_list.end(), App->map->WorldToMap(possible_map.at(Cap + 2).first, possible_map.at(Cap + 2).second)) != inrange_mov_list.end())
				{
					App->render->Blit(debug_texture, possible_map.at(Cap + mod).first, possible_map.at(Cap + mod).second, &debug_green);
					App->render->Blit(debug_texture, possible_map.at(Cap - mod).first, possible_map.at(Cap - mod).second, &debug_green);
				}
				else if (std::find(inrange_mov_list.begin(), inrange_mov_list.end(), App->map->WorldToMap(possible_map.at(Cap - (mod*2)).first, possible_map.at(Cap - (mod * 2)).second)) != inrange_mov_list.end()
					|| std::find(inrange_mov_list.begin(), inrange_mov_list.end(), App->map->WorldToMap(possible_map.at(Cap + (mod*2)).first, possible_map.at(Cap + (mod * 2)).second)) != inrange_mov_list.end())
				{
					App->render->Blit(debug_texture, possible_map.at(Cap + 1).first, possible_map.at(Cap + 1).second, &debug_green);
					App->render->Blit(debug_texture, possible_map.at(Cap - 1).first, possible_map.at(Cap - 1).second, &debug_green);
				}
			}
			break;
		default:
			break;
		}
		++i;
	}

	App->render->Blit(debug_texture, possible_map.at(Cap).first, possible_map.at(Cap).second, &debug_green);
	
	// Input Select Tiled
	InputSelectMove();

	// Input Accept and Decline
	if (App->input->Accept() 
		&& std::find(inrange_mov_list.begin(), inrange_mov_list.end(), App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second)) != inrange_mov_list.end()
			&&(App->pathfinding->IsAttackable(App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second), type)
			|| (type == ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE || type == ENTITY_TYPE::ENTITY_CHARACTER_IRIS)
			|| (type == ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB && App->pathfinding->IsWalkable(App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second))
			&& App->pathfinding->CanTrap(App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second)))))
	{
		current_turn = ABILITY_1;

		i = 0;
		mod = sqrt(possible_mov_list.size());
		switch (type)
		{
		case ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE:
			for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
			{
				if (std::find(inrange_mov_list.begin(), inrange_mov_list.end(), App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second)) != inrange_mov_list.end()
					&& std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov)) != inrange_mov_list.end())
				{
					if (i == Cap + 1)
					{
						objective_position.push_back({ possible_map.at(Cap + 1).first, possible_map.at(Cap + 1).second });
					}
					else if (i == Cap - 1)
					{
						objective_position.push_back({ possible_map.at(Cap - 1).first, possible_map.at(Cap - 1).second });
					}
					else if (i == Cap + mod)
					{
						objective_position.push_back({ possible_map.at(Cap + mod).first, possible_map.at(Cap + mod).second });
					}
					else if (i == Cap - mod)
					{
						objective_position.push_back({ possible_map.at(Cap - mod).first, possible_map.at(Cap - mod).second });
					}
				}
				++i;
			}
			break;
		case ENTITY_TYPE::ENTITY_CHARACTER_IRIS:
			for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
			{
				if (i == Cap && std::find(inrange_mov_list.begin(), inrange_mov_list.end(), App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second)) != inrange_mov_list.end()) 
				{
					if (std::find(inrange_mov_list.begin(), inrange_mov_list.end(), App->map->WorldToMap(possible_map.at(Cap - 2).first, possible_map.at(Cap - 2).second)) != inrange_mov_list.end()
						|| std::find(inrange_mov_list.begin(), inrange_mov_list.end(), App->map->WorldToMap(possible_map.at(Cap + 2).first, possible_map.at(Cap + 2).second)) != inrange_mov_list.end())
					{
						objective_position.push_back({ possible_map.at(Cap + mod).first, possible_map.at(Cap + mod).second });
						objective_position.push_back({ possible_map.at(Cap - mod).first, possible_map.at(Cap - mod).second });
					}
					else if (std::find(inrange_mov_list.begin(), inrange_mov_list.end(), App->map->WorldToMap(possible_map.at(Cap - (mod * 2)).first, possible_map.at(Cap - (mod * 2)).second)) != inrange_mov_list.end()
						|| std::find(inrange_mov_list.begin(), inrange_mov_list.end(), App->map->WorldToMap(possible_map.at(Cap + (mod * 2)).first, possible_map.at(Cap + (mod * 2)).second)) != inrange_mov_list.end())
					{
						objective_position.push_back({ possible_map.at(Cap + 1).first, possible_map.at(Cap + 1).second });
						objective_position.push_back({ possible_map.at(Cap - 1).first, possible_map.at(Cap - 1).second });
					}
				}
				++i;
			}
			break;
		default:
			break;
		}

		objective_position.push_back({ possible_map.at(Cap).first, possible_map.at(Cap).second });
	}
	else if (App->input->Decline())
	{
		EndTurn();
		current_turn = SELECT_ACTION;
	}

}

void Character::Ability_1()
{
	if (current_stats.Mana >= 25) {
		if (!sound_fx) {
			App->audio->PlayFx(sfx.Ability_1_SFX);
			sound_fx = true;
		}
		if (!finish_attack)
		{
			if (objective_position.back().first < position.first && objective_position.back().second > position.second) {
				CurrentMovement(ABILITY_1_LEFT_FRONT);
			}
			else if (objective_position.back().first > position.first && objective_position.back().second > position.second) {
				CurrentMovement(ABILITY_1_RIGHT_FRONT);
			}
			else if (objective_position.back().first < position.first && objective_position.back().second < position.second) {
				CurrentMovement(ABILITY_1_LEFT_BACK);
			}
			else if (objective_position.back().first > position.first && objective_position.back().second < position.second) {
				CurrentMovement(ABILITY_1_RIGHT_BACK);
			}
			else if (objective_position.back().first == position.first && objective_position.back().second > position.second) {
				CurrentMovement(ABILITY_1_FRONT);
			}
			else if (objective_position.back().first == position.first && objective_position.back().second < position.second) {
				CurrentMovement(ABILITY_1_BACK);
			}
			else if (objective_position.back().first < position.first && objective_position.back().second == position.second) {
				CurrentMovement(ABILITY_1_LEFT);
			}
			else if (objective_position.back().first > position.first && objective_position.back().second == position.second) {
				CurrentMovement(ABILITY_1_RIGHT);
			}
			else {
				CurrentMovement(ABILITY_1_RIGHT);
			}
		}
	}
	else {
		inrange_mov_list.clear();
		current_turn = SELECT_ACTION;
	}
	
	if (emitter == nullptr)
	{
		switch (type)
		{
		case ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE:
			if (current_animation->GetCurrentFrameIndex() == 1)
			{
				emitter = App->particle_system->AddEmiter({ position.first + 16, position.second }, EmitterType::EMITTER_TYPE_ABILITY1);
				emitter->SetTextureRect({ 384, 64, 62, 53 });
				emitter->SetSize(62, 62);
				emitter->SetPosition({ objective_position.front().first + 32 , objective_position.front().second - 16 });
			}
			break;
		case ENTITY_TYPE::ENTITY_CHARACTER_STORM:
			if (current_animation->GetCurrentFrameIndex() == 2)
			{
				emitter = App->particle_system->AddEmiter({ position.first + 16, position.second}, EmitterType::EMITTER_TYPE_ATTACK);
				emitter->SetTextureRect({ 384, 64, 62, 53 });
				emitter->SetSize(62, 62);
			}
			break;
		default:
			break;
		}
	}
	if (emitter != nullptr)
	{
		if (type == ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE)
		{
			if (emitter->GetTime() <= 0)
			{
				emitter->StopEmission();
				emitter = nullptr;
			}
		}
		else
		{
			if (emitter->GetEmitterPos().first < objective_position.back().first + 10)
				emitter->SetPosition({ emitter->GetEmitterPos().first + 4, emitter->GetEmitterPos().second });
			if (emitter->GetEmitterPos().first > objective_position.back().first + 10)
				emitter->SetPosition({ emitter->GetEmitterPos().first - 4, emitter->GetEmitterPos().second });
			if (emitter->GetEmitterPos().second < objective_position.back().second - 16)
				emitter->SetPosition({ emitter->GetEmitterPos().first, emitter->GetEmitterPos().second + 4 });
			if (emitter->GetEmitterPos().second > objective_position.back().second - 16)
				emitter->SetPosition({ emitter->GetEmitterPos().first, emitter->GetEmitterPos().second - 4 });
			if ((emitter->GetEmitterPos().first <= objective_position.back().first + 20) && (emitter->GetEmitterPos().first >= objective_position.back().first)
				&& (emitter->GetEmitterPos().second >= objective_position.back().second - 32) && (emitter->GetEmitterPos().second <= objective_position.back().second))
			{
				emitter->StopEmission();
				emitter = nullptr;
			}
		}
	}
	// Ending attack and start idle animation
	if (finish_attack && emitter == nullptr) {
		current_stats.Mana -= 25;
		EndTurn();
		if (current_movement == ABILITY_1_LEFT_FRONT)
		{
			CurrentMovement(IDLE_LEFT_FRONT);
		}
		else if (current_movement == ABILITY_1_RIGHT_FRONT)
		{
			CurrentMovement(IDLE_RIGHT_FRONT);
		}
		else if (current_movement == ABILITY_1_LEFT_BACK)
		{
			CurrentMovement(IDLE_LEFT_BACK);
		}
		else if (current_movement == ABILITY_1_RIGHT_BACK)
		{
			CurrentMovement(IDLE_RIGHT_BACK);
		}
		else if (current_movement == ABILITY_1_LEFT)
		{
			CurrentMovement(IDLE_LEFT);
		}
		else if (current_movement == ABILITY_1_RIGHT)
		{
			CurrentMovement(IDLE_RIGHT);
		}
		else if (current_movement == ABILITY_1_FRONT)
		{
			CurrentMovement(IDLE_FRONT);
		}
		else if (current_movement == ABILITY_1_BACK)
		{
			CurrentMovement(IDLE_BACK);
		}

	}
}

void Character::SelectAbility_2() {

	if (Cap == -1) {
		std::pair<int, int> tmp = App->map->WorldToMap(position.first, position.second);
		Cap = tmp.first + (tmp.second * 25);
	}

	int i = 0;
	int mod = sqrt(possible_mov_list.size());
	for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
	{
		possible_map.push_back(App->map->MapToWorld((*possible_mov).first, (*possible_mov).second));
		if (i != Cap && std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov)) != inrange_mov_list.end())
		{
			if (App->pathfinding->IsAttackable({ (*possible_mov).first , (*possible_mov).second }, type)
				|| type == ENTITY_TYPE::ENTITY_CHARACTER_IRIS || type == ENTITY_TYPE::ENTITY_CHARACTER_STORM)
			{
				App->render->Blit(debug_texture, possible_map.at(i).first, possible_map.at(i).second, &debug_blue);
			}
			else
			{
				App->render->Blit(debug_texture, possible_map.at(i).first, possible_map.at(i).second, &debug_red);
			}
		}

		++i;
	}

	App->render->Blit(debug_texture, possible_map.at(Cap).first, possible_map.at(Cap).second, &debug_green);

	// Input Select Tiled
	InputSelectMove();

	// Input Accept and Decline
	if (App->input->Accept()
		&& std::find(inrange_mov_list.begin(), inrange_mov_list.end(), App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second)) != inrange_mov_list.end()
			&& (App->pathfinding->IsAttackable(App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second), type)
			|| type == ENTITY_TYPE::ENTITY_CHARACTER_IRIS || type == ENTITY_TYPE::ENTITY_CHARACTER_STORM))
	{
		current_turn = ABILITY_2;

		objective_position.push_back({ possible_map.at(Cap).first, possible_map.at(Cap).second });
	}
	else if (App->input->Decline())
	{
		EndTurn();
		current_turn = SELECT_ACTION;
	}

}

void Character::Ability_2()
{
	if (current_stats.Mana >= 50) {
		if (!sound_fx) {
			App->audio->PlayFx(sfx.Ability_2_SFX);
			sound_fx = true;
		}
		if (!finish_attack)
		{
			if (objective_position.back().first < position.first && objective_position.back().second > position.second) {
				CurrentMovement(ABILITY_2_LEFT_FRONT);
			}
			else if (objective_position.back().first > position.first && objective_position.back().second > position.second) {
				CurrentMovement(ABILITY_2_RIGHT_FRONT);
			}
			else if (objective_position.back().first < position.first && objective_position.back().second < position.second) {
				CurrentMovement(ABILITY_2_LEFT_BACK);
			}
			else if (objective_position.back().first > position.first && objective_position.back().second < position.second) {
				CurrentMovement(ABILITY_2_RIGHT_BACK);
			}
			else if (objective_position.back().first == position.first && objective_position.back().second > position.second) {
				CurrentMovement(ABILITY_2_FRONT);
			}
			else if (objective_position.back().first == position.first && objective_position.back().second < position.second) {
				CurrentMovement(ABILITY_2_BACK);
			}
			else if (objective_position.back().first < position.first && objective_position.back().second == position.second) {
				CurrentMovement(ABILITY_2_LEFT);
			}
			else if (objective_position.back().first > position.first && objective_position.back().second == position.second) {
				CurrentMovement(ABILITY_2_RIGHT);
			}
			else {
				CurrentMovement(ABILITY_2_RIGHT);
			}
		}
	}
	else {
		inrange_mov_list.clear();
		current_turn = SELECT_ACTION;
	}

	if (emitter == nullptr)
	{
		switch (type)
		{
		case ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE:
			if (current_animation->GetCurrentFrameIndex() == 1)
			{
				emitter = App->particle_system->AddEmiter({ position.first + 64, position.second - 64 }, EmitterType::EMITTER_TYPE_ABILITY2);
				emitter->SetTextureRect({ 320, 78, 13, 11 });
				emitter->SetSize(32, 32);
				sapphire_ability = true;
			}
			break;
		case ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB:
			if (current_animation->GetCurrentFrameIndex() == 1)
			{
				emitter = App->particle_system->AddEmiter({ position.first + 32, position.second }, EmitterType::EMITTER_TYPE_ATTACK);
				emitter->SetTextureRect({ 448, 0, 64, 64 });
				emitter->SetSize(64, 128);
			}
			break;
		case ENTITY_TYPE::ENTITY_CHARACTER_IRIS:
			if (current_animation->GetCurrentFrameIndex() == 1)
			{
				emitter = App->particle_system->AddEmiter({ position.first + 32, position.second - 64 }, EmitterType::EMITTER_TYPE_ABILITY2);
				emitter->SetTextureRect({ 448, 0, 64, 64 });
			}
			break;
		default:
			break;
		}
	}
	if (emitter != nullptr)
	{
		if (type == ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE || type == ENTITY_TYPE::ENTITY_CHARACTER_IRIS)
		{
			if (emitter->GetTime() <= 0)
			{
				emitter->StopEmission();
				emitter = nullptr;
				if (sapphire_ability)
				{
					emitter = App->particle_system->AddEmiter({ position.first + 16, position.second }, EmitterType::EMITTER_TYPE_ABILITY2);
					emitter->SetTextureRect({ 338, 80, 11, 9 });
					emitter->SetSize(62, 62);
					emitter->SetPosition({ objective_position.front().first + 32 , objective_position.front().second + 62 });
					sapphire_ability = false;
				}
			}
		}
		else
		{
			if (emitter->GetEmitterPos().first < objective_position.back().first + 10)
				emitter->SetPosition({ emitter->GetEmitterPos().first + 4, emitter->GetEmitterPos().second });
			if (emitter->GetEmitterPos().first > objective_position.back().first + 10)
				emitter->SetPosition({ emitter->GetEmitterPos().first - 4, emitter->GetEmitterPos().second });
			if (emitter->GetEmitterPos().second < objective_position.back().second - 16)
				emitter->SetPosition({ emitter->GetEmitterPos().first, emitter->GetEmitterPos().second + 4 });
			if (emitter->GetEmitterPos().second > objective_position.back().second - 16)
				emitter->SetPosition({ emitter->GetEmitterPos().first, emitter->GetEmitterPos().second - 4 });
			if ((emitter->GetEmitterPos().first <= objective_position.back().first + 20) && (emitter->GetEmitterPos().first >= objective_position.back().first)
				&& (emitter->GetEmitterPos().second >= objective_position.back().second - 32) && (emitter->GetEmitterPos().second <= objective_position.back().second))
			{
				emitter->StopEmission();
				emitter = nullptr;
			}
		}
	}
	// Ending attack and start idle animation
	if (finish_attack && emitter == nullptr) {
		current_stats.Mana -= 50;
		EndTurn();
		if (current_movement == ABILITY_2_LEFT_FRONT)
		{
			CurrentMovement(IDLE_LEFT_FRONT);
		}
		else if (current_movement == ABILITY_2_RIGHT_FRONT)
		{
			CurrentMovement(IDLE_RIGHT_FRONT);
		}
		else if (current_movement == ABILITY_2_LEFT_BACK)
		{
			CurrentMovement(IDLE_LEFT_BACK);
		}
		else if (current_movement == ABILITY_2_RIGHT_BACK)
		{
			CurrentMovement(IDLE_RIGHT_BACK);
		}
		else if (current_movement == ABILITY_2_LEFT)
		{
			CurrentMovement(IDLE_LEFT);
		}
		else if (current_movement == ABILITY_2_RIGHT)
		{
			CurrentMovement(IDLE_RIGHT);
		}
		else if (current_movement == ABILITY_2_FRONT)
		{
			CurrentMovement(IDLE_FRONT);
		}
		else if (current_movement == ABILITY_2_BACK)
		{
			CurrentMovement(IDLE_BACK);
		}
	}
}

void Character::SelectAbility_3() {

	if (Cap == -1) {
		std::pair<int, int> tmp = App->map->WorldToMap(position.first, position.second);
		Cap = tmp.first + (tmp.second * 25);
	}

	int i = 0;
	int mod = sqrt(possible_mov_list.size());
	for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
	{
		possible_map.push_back(App->map->MapToWorld((*possible_mov).first, (*possible_mov).second));

		if (i != Cap && std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov)) != inrange_mov_list.end())
		{
			if (App->pathfinding->IsAttackable({ (*possible_mov).first , (*possible_mov).second }, type)
				|| type == ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB || type == ENTITY_TYPE::ENTITY_CHARACTER_STORM)
			{
				App->render->Blit(debug_texture, possible_map.at(i).first, possible_map.at(i).second, &debug_blue);
			}
			else
			{
				App->render->Blit(debug_texture, possible_map.at(i).first, possible_map.at(i).second, &debug_red);
			}
		}
		++i;
	}

	App->render->Blit(debug_texture, possible_map.at(Cap).first, possible_map.at(Cap).second, &debug_green);
	InputSelectMove();
	

	if (App->input->Accept()
		&& std::find(inrange_mov_list.begin(), inrange_mov_list.end(), App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second)) != inrange_mov_list.end()
			&& (App->pathfinding->IsAttackable(App->map->WorldToMap(possible_map.at(Cap).first, possible_map.at(Cap).second), type)
			|| type == ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB || type == ENTITY_TYPE::ENTITY_CHARACTER_STORM))
	{
		current_turn = ABILITY_3;
		if (type == ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB)
		{
			i = 0;
			for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
			{
				if (std::find(inrange_mov_list.begin(), inrange_mov_list.end(), (*possible_mov)) != inrange_mov_list.end())
				{
					objective_position.push_back({ possible_map.at(i).first, possible_map.at(i).second });
				}
				++i;
			}
		}
		else
		{
			objective_position.push_back({ possible_map.at(Cap).first, possible_map.at(Cap).second });
		}
	}
	else if (App->input->Decline())
	{
		EndTurn();
		current_turn = SELECT_ACTION;
	}

}

void Character::Ability_3()
{
	if (current_stats.Mana >= 100) {
		if (!sound_fx) {
			App->audio->PlayFx(sfx.Ability_3_SFX);
			sound_fx = true;
		}
		if (!finish_attack)
		{
			if (objective_position.back().first < position.first && objective_position.back().second > position.second) {
				CurrentMovement(ABILITY_3_LEFT_FRONT);
			}
			else if (objective_position.back().first > position.first && objective_position.back().second > position.second) {
				CurrentMovement(ABILITY_3_RIGHT_FRONT);
			}
			else if (objective_position.back().first < position.first && objective_position.back().second < position.second) {
				CurrentMovement(ABILITY_3_LEFT_BACK);
			}
			else if (objective_position.back().first > position.first && objective_position.back().second < position.second) {
				CurrentMovement(ABILITY_3_RIGHT_BACK);
			}
			else if (objective_position.back().first == position.first && objective_position.back().second > position.second) {
				CurrentMovement(ABILITY_3_FRONT);
			}
			else if (objective_position.back().first == position.first && objective_position.back().second < position.second) {
				CurrentMovement(ABILITY_3_BACK);
			}
			else if (objective_position.back().first < position.first && objective_position.back().second == position.second) {
				CurrentMovement(ABILITY_3_LEFT);
			}
			else if (objective_position.back().first > position.first && objective_position.back().second == position.second) {
				CurrentMovement(ABILITY_3_RIGHT);
			}
			else {
				CurrentMovement(ABILITY_3_RIGHT);
			}
		}
	}
	else {
		inrange_mov_list.clear();
		current_turn = SELECT_ACTION;
	}


	if (emitter == nullptr)
	{
		switch (type)
		{
		case ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE:
			if (current_animation->GetCurrentFrameIndex() == 1)
			{
				emitter = App->particle_system->AddEmiter({ -(App->window->GetScreenWidth()/2), 400 }, EmitterType::EMITTER_TYPE_ABILITY3);
				emitter->SetTextureRect({ 0, 128, 512, 384 });
			}
			break;
		case ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB:
			if (current_animation->GetCurrentFrameIndex() == 1)
			{
				emitter = App->particle_system->AddEmiter({ position.first + 32, position.second - 16 }, EmitterType::EMITTER_TYPE_ABILITY3);
				emitter->SetTextureRect({ 364, 94, 7, 7 });
			}
			break;
		case ENTITY_TYPE::ENTITY_CHARACTER_IRIS:
			if (current_animation->GetCurrentFrameIndex() == 2)
			{
				emitter = App->particle_system->AddEmiter({ position.first + 32, position.second - 16 }, EmitterType::EMITTER_TYPE_ABILITY2);
				emitter->SetTextureRect({ 364, 94, 7, 7 });
			}
			break;
		default:
			break;
		}
	}
	if (emitter != nullptr)
	{
		if ((emitter->GetEmitterPos().first > App->window->GetScreenWidth()/2 && type == ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE) || type == ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB)
		{
			emitter->StopEmission();
			emitter = nullptr;
		}
		else
		{
			if (emitter->GetEmitterPos().first < objective_position.back().first + 10)
				emitter->SetPosition({ emitter->GetEmitterPos().first + 4, emitter->GetEmitterPos().second });
			if (emitter->GetEmitterPos().first > objective_position.back().first + 10)
				emitter->SetPosition({ emitter->GetEmitterPos().first - 4, emitter->GetEmitterPos().second });
			if (emitter->GetEmitterPos().second < objective_position.back().second - 16)
				emitter->SetPosition({ emitter->GetEmitterPos().first, emitter->GetEmitterPos().second + 4 });
			if (emitter->GetEmitterPos().second > objective_position.back().second - 16)
				emitter->SetPosition({ emitter->GetEmitterPos().first, emitter->GetEmitterPos().second - 4 });
			if ((emitter->GetEmitterPos().first <= objective_position.back().first + 20) && (emitter->GetEmitterPos().first >= objective_position.back().first)
				&& (emitter->GetEmitterPos().second >= objective_position.back().second - 32) && (emitter->GetEmitterPos().second <= objective_position.back().second))
			{
				emitter->StopEmission();
				emitter = nullptr;
			}
		}
	}
	
	// Ending attack and start idle animation
	if (finish_attack && emitter == nullptr) {
		current_stats.Mana -= 100;
		EndTurn();
		if (current_movement == ABILITY_3_LEFT_FRONT)
		{
			CurrentMovement(IDLE_LEFT_FRONT);
		}
		else if (current_movement == ABILITY_3_RIGHT_FRONT)
		{
			CurrentMovement(IDLE_RIGHT_FRONT);
		}
		else if (current_movement == ABILITY_3_LEFT_BACK)
		{
			CurrentMovement(IDLE_LEFT_BACK);
		}
		else if (current_movement == ABILITY_3_RIGHT_BACK)
		{
			CurrentMovement(IDLE_RIGHT_BACK);
		}
		else if (current_movement == ABILITY_3_LEFT)
		{
			CurrentMovement(IDLE_LEFT);
		}
		else if (current_movement == ABILITY_3_RIGHT)
		{
			CurrentMovement(IDLE_RIGHT);
		}
		else if (current_movement == ABILITY_3_FRONT)
		{
			CurrentMovement(IDLE_FRONT);
		}
		else if (current_movement == ABILITY_3_BACK)
		{
			CurrentMovement(IDLE_BACK);
		}
	}
}

void Character::Defend()
{
	App->audio->PlayFx(sfx.Defend_SFX);
	defend = true;
	switch (current_movement)
	{
	case Entity::IDLE_LEFT_FRONT:
		CurrentMovement(DEFEND_LEFT_FRONT);
		break;
	case Entity::IDLE_RIGHT_FRONT:
		CurrentMovement(DEFEND_RIGHT_FRONT);
		break;
	case Entity::IDLE_LEFT_BACK:
		CurrentMovement(DEFEND_LEFT_BACK);
		break;
	case Entity::IDLE_RIGHT_BACK:
		CurrentMovement(DEFEND_RIGHT_BACK);
		break;
	case Entity::IDLE_LEFT:
		CurrentMovement(DEFEND_LEFT);
		break;
	case Entity::IDLE_RIGHT:
		CurrentMovement(DEFEND_RIGHT);
		break;
	case Entity::IDLE_FRONT:
		CurrentMovement(DEFEND_FRONT);
		break;
	case Entity::IDLE_BACK:
		CurrentMovement(DEFEND_BACK);
		break;
	case Entity::DEFEND_LEFT_FRONT:
		CurrentMovement(DEFEND_LEFT_FRONT);
		break;
	case Entity::DEFEND_RIGHT_FRONT:
		CurrentMovement(DEFEND_RIGHT_FRONT);
		break;
	case Entity::DEFEND_LEFT_BACK:
		CurrentMovement(DEFEND_LEFT_BACK);
		break;
	case Entity::DEFEND_RIGHT_BACK:
		CurrentMovement(DEFEND_RIGHT_BACK);
		break;
	case Entity::DEFEND_LEFT:
		CurrentMovement(DEFEND_LEFT);
		break;
	case Entity::DEFEND_RIGHT:
		CurrentMovement(DEFEND_RIGHT);
		break;
	case Entity::DEFEND_FRONT:
		CurrentMovement(DEFEND_FRONT);
		break;
	case Entity::DEFEND_BACK:
		CurrentMovement(DEFEND_BACK);
		break;
	default:
		break;
	}
	sound_fx = false;
}

void Character::ComeBack()
{
	position = comeback_position;
	CurrentMovement(comeback_movement);
	inrange_mov_list.clear();
	nomov_list.clear();
	possible_map.clear();
	objective_position.clear();
	Cap = -1;
	current_turn = SEARCH_MOVE;
}

void Character::Die()
{
	App->audio->PlayFx(sfx.Dead_SFX);
		
	switch (current_movement)
	{
	case Entity::IDLE_LEFT_FRONT:
		CurrentMovement(DEAD_LEFT_FRONT);
		break;
	case Entity::IDLE_RIGHT_FRONT:
		CurrentMovement(DEAD_RIGHT_FRONT);
		break;
	case Entity::IDLE_LEFT_BACK:
		CurrentMovement(DEAD_LEFT_BACK);
		break;
	case Entity::IDLE_RIGHT_BACK:
		CurrentMovement(DEAD_RIGHT_BACK);
		break;
	case Entity::IDLE_LEFT:
		CurrentMovement(DEAD_LEFT);
		break;
	case Entity::IDLE_RIGHT:
		CurrentMovement(DEAD_RIGHT);
		break;
	case Entity::IDLE_FRONT:
		CurrentMovement(DEAD_FRONT);
		break;
	case Entity::IDLE_BACK:
		CurrentMovement(DEAD_BACK);
		break;
	case Entity::DEFEND_LEFT_FRONT:
		CurrentMovement(DEAD_LEFT_FRONT);
		break;
	case Entity::DEFEND_RIGHT_FRONT:
		CurrentMovement(DEAD_RIGHT_FRONT);
		break;
	case Entity::DEFEND_LEFT_BACK:
		CurrentMovement(DEAD_LEFT_BACK);
		break;
	case Entity::DEFEND_RIGHT_BACK:
		CurrentMovement(DEAD_RIGHT_BACK);
		break;
	case Entity::DEFEND_LEFT:
		CurrentMovement(DEAD_LEFT);
		break;
	case Entity::DEFEND_RIGHT:
		CurrentMovement(DEAD_RIGHT);
		break;
	case Entity::DEFEND_FRONT:
		CurrentMovement(DEAD_FRONT);
		break;
	case Entity::DEFEND_BACK:
		CurrentMovement(DEAD_BACK);
		break;
	default:
		break;
	}
}

void Character::EndTurn() {
	inrange_mov_list.clear();
	nomov_list.clear();
	possible_map.clear();
	objective_position.clear();
	Cap = -1;
	current_animation->Reset();
	target = { 0,0 };
	sound_fx = false;
	finish_attack = false;
	current_turn = END_TURN;
}

void Character::InputSelectMove() {


	if (App->input->LeftUp()) {

		int i = 0;
		for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
		{
			if (i >= possible_mov_list.size()) {
				break;
			}
			else if (i == Cap)
			{
				int j = 0;
				for (std::list<std::pair<int, int>>::iterator possible_mov_2 = possible_mov_list.begin(); possible_mov_2 != possible_mov_list.end(); ++possible_mov_2)
				{
					if (j == Cap - 1) {
						if ((*possible_mov).first - 1 == (*possible_mov_2).first && (*possible_mov).second == (*possible_mov_2).second)
						{
							Cap -= 1;
							i = possible_mov_list.size();
							break;
						}
					}
					++j;
				}
			}
			++i;
		}
	}
	else if (App->input->RightDown()) {

		int i = 0;
		for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
		{
			if (i >= possible_mov_list.size()) {
				break;
			}
			else if (i == Cap)
			{
				int j = 0;
				for (std::list<std::pair<int, int>>::iterator possible_mov_2 = possible_mov_list.begin(); possible_mov_2 != possible_mov_list.end(); ++possible_mov_2)
				{
					if (j == Cap + 1) {
						if ((*possible_mov).first + 1 == (*possible_mov_2).first && (*possible_mov).second == (*possible_mov_2).second)
						{
							Cap += 1;
							i = possible_mov_list.size();
							break;
						}
					}
					++j;
				}
			}
			++i;
		}
	}
	else if (App->input->LeftDown()) {

		int i = 0;
		for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
		{
			if (i >= possible_mov_list.size()) {
				break;
			}
			else if (i == Cap)
			{
				int j = 0;
				for (std::list<std::pair<int, int>>::iterator possible_mov_2 = possible_mov_list.begin(); possible_mov_2 != possible_mov_list.end(); ++possible_mov_2)
				{
					if (j == Cap + sqrt(possible_mov_list.size())) {
						if ((*possible_mov).first == (*possible_mov_2).first && (*possible_mov).second + 1 == (*possible_mov_2).second)
						{
							Cap += sqrt(possible_mov_list.size());
							i = possible_mov_list.size();
							break;
						}
					}
					++j;
				}
			}
			++i;
		}
	}
	else if (App->input->RightUp()) {

		int i = 0;
		for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
		{
			if (i >= possible_mov_list.size()) {
				break;
			}
			else if (i == Cap)
			{
				int j = 0;
				for (std::list<std::pair<int, int>>::iterator possible_mov_2 = possible_mov_list.begin(); possible_mov_2 != possible_mov_list.end(); ++possible_mov_2)
				{
					if (j == Cap - sqrt(possible_mov_list.size())) {
						if ((*possible_mov).first == (*possible_mov_2).first && (*possible_mov).second - 1 == (*possible_mov_2).second)
						{
							Cap -= sqrt(possible_mov_list.size());
							i = possible_mov_list.size();
							break;
						}
					}
					++j;
				}
			}
			++i;
		}
	}
	else if (App->input->Left()) {

		int i = 0;
		for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
		{
			if (i >= possible_mov_list.size()) {
				break;
			}
			else if (i == Cap)
			{
				int j = 0;
				for (std::list<std::pair<int, int>>::iterator possible_mov_2 = possible_mov_list.begin(); possible_mov_2 != possible_mov_list.end(); ++possible_mov_2)
				{
					if (j == Cap - 1 + sqrt(possible_mov_list.size())) {
						if ((*possible_mov).first - 1 == (*possible_mov_2).first && (*possible_mov).second + 1 == (*possible_mov_2).second)
						{
							Cap += sqrt(possible_mov_list.size()) - 1;
							i = possible_mov_list.size();
							break;
						}
					}
					++j;
				}
			}
			++i;
		}
	}
	else if (App->input->Right()) {

		int i = 0;
		for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
		{
			if (i >= possible_mov_list.size()) {
				break;
			}
			else if (i == Cap)
			{
				int j = 0;
				for (std::list<std::pair<int, int>>::iterator possible_mov_2 = possible_mov_list.begin(); possible_mov_2 != possible_mov_list.end(); ++possible_mov_2)
				{
					if (j == Cap + 1 - sqrt(possible_mov_list.size())) {
						if ((*possible_mov).first + 1 == (*possible_mov_2).first && (*possible_mov).second - 1 == (*possible_mov_2).second)
						{
							Cap -= sqrt(possible_mov_list.size()) - 1;
							i = possible_mov_list.size();
							break;
						}
					}
					++j;
				}
			}
			++i;
		}
	}
	else if (App->input->Up()) {

		int i = 0;
		for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
		{
			if (i >= possible_mov_list.size()) {
				break;
			}
			else if (i == Cap)
			{
				int j = 0;
				for (std::list<std::pair<int, int>>::iterator possible_mov_2 = possible_mov_list.begin(); possible_mov_2 != possible_mov_list.end(); ++possible_mov_2)
				{
					if (j == Cap - 1 - sqrt(possible_mov_list.size())) {
						if ((*possible_mov).first - 1 == (*possible_mov_2).first && (*possible_mov).second - 1 == (*possible_mov_2).second)
						{
							Cap -= sqrt(possible_mov_list.size()) + 1;
							i = possible_mov_list.size();
							break;
						}
					}
					++j;
				}
			}
			++i;
		}
	}
	else if (App->input->Down()) {

		int i = 0;
		for (std::list<std::pair<int, int>>::iterator possible_mov = possible_mov_list.begin(); possible_mov != possible_mov_list.end(); ++possible_mov)
		{
			if (i >= possible_mov_list.size()) {
				break;
			}
			else if (i == Cap)
			{
				int j = 0;
				for (std::list<std::pair<int, int>>::iterator possible_mov_2 = possible_mov_list.begin(); possible_mov_2 != possible_mov_list.end(); ++possible_mov_2)
				{
					if (j == Cap + 1 + sqrt(possible_mov_list.size())) {
						if ((*possible_mov).first + 1 == (*possible_mov_2).first && (*possible_mov).second + 1 == (*possible_mov_2).second)
						{
							Cap += sqrt(possible_mov_list.size()) + 1;
							i = possible_mov_list.size();
							break;
						}
					}
					++j;
				}
			}
			++i;
		}
	}
}
