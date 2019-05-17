#include "Log.h"
#include "Application.h"
#include "ModuleEntityManager.h"
#include "ModuleMap.h"
#include "ModulePathfinding.h"


ModulePathfinding::ModulePathfinding() : Module(), map(NULL), last_path(DEFAULT_PATH_LENGTH), width(0), height(0)
{
	name = "pathfinding";
}

// Destructor
ModulePathfinding::~ModulePathfinding()
{
	RELEASE_ARRAY(map);
}

// Called before quitting
bool ModulePathfinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	last_path.clear();
	RELEASE_ARRAY(map);
	return true;
}

void ModulePathfinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width*height];
	memcpy(map, data, width*height);
}

// Utility: return true if pos is inside the map boundaries
bool ModulePathfinding::CheckBoundaries(const std::pair<int, int>& pos) const
{
	return (pos.first >= 0 && pos.first <= (int)width &&
		pos.second >= 0 && pos.second <= (int)height);
}

// Utility: returns true is the tile is walkable
bool ModulePathfinding::IsWalkable(const std::pair<int, int>& _pos) const
{
	uchar t = GetTileAt(_pos);
	return t != INVALID_WALK_CODE && t > 0;
}
bool ModulePathfinding::CanTrap(const std::pair<int, int>& _pos) const
{
	for (std::list<Entity*>::iterator object = App->entity_manager->objects.begin(); object != App->entity_manager->objects.end(); ++object)
	{
		if (App->map->WorldToMap((*object)->GetPosition().first, (*object)->GetPosition().second) == _pos)
		{
			return false;
		}
	}
	return true;
}

// Utility: returns true is the tile is used by other entity
bool ModulePathfinding::IsUsed(const std::pair<int, int>& _pos, Entity* _entity) const
{
	for (std::list<Entity*>::iterator entity = App->entity_manager->entities.begin(); entity != App->entity_manager->entities.end(); ++entity)
	{
		if (App->map->WorldToMap((*entity)->GetPosition().first, (*entity)->GetPosition().second) == _pos
			&& (*entity) != _entity 
			&& !(std::find(App->entity_manager->objects.begin(), App->entity_manager->objects.end(), (*entity)) != App->entity_manager->objects.end()))
		{
			return true;
		}
	}
	return false;
}
// Utility: returns true is the tile is attackable
bool ModulePathfinding::IsAttackable(const std::pair<int, int>& _pos, ENTITY_TYPE _type) const
{
	switch (_type)
	{
	case ENTITY_TYPE::ENTITY_CHARACTER_SAPPHIRE:
		for (std::list<Entity*>::iterator character = App->entity_manager->characters.begin(); character != App->entity_manager->characters.end(); ++character)
		{
			if (App->map->WorldToMap((*character)->GetPosition().first, (*character)->GetPosition().second) == _pos 
				&& (*character)->current_state == Entity::ALIVE){
				return true;
			}
		}
		for (std::list<Entity*>::iterator enemy = App->entity_manager->enemies.begin(); enemy != App->entity_manager->enemies.end(); ++enemy)
		{
			if (App->map->WorldToMap((*enemy)->GetPosition().first, (*enemy)->GetPosition().second) == _pos) {
				return true;
			}
		}
		break;
	case ENTITY_TYPE::ENTITY_CHARACTER_IRIS:
	case ENTITY_TYPE::ENTITY_CHARACTER_STORM:
	case ENTITY_TYPE::ENTITY_CHARACTER_GEORGEB:
		for (std::list<Entity*>::iterator enemy = App->entity_manager->enemies.begin(); enemy != App->entity_manager->enemies.end(); ++enemy)
		{
			if (App->map->WorldToMap((*enemy)->GetPosition().first, (*enemy)->GetPosition().second) == _pos) {
				return true;
			}
		}
		break;
	case ENTITY_TYPE::ENTITY_ENEMY_BONEYMAN:
	case ENTITY_TYPE::ENTITY_ENEMY_PINKKING:
	case ENTITY_TYPE::ENTITY_ENEMY_HOTDOG:
	case ENTITY_TYPE::ENTITY_ENEMY_BURGDOG:
		for (std::list<Entity*>::iterator character = App->entity_manager->characters.begin(); character != App->entity_manager->characters.end(); ++character)
		{
			if (App->map->WorldToMap((*character)->GetPosition().first, (*character)->GetPosition().second) == _pos) {
				return true;
			}
		}
		break;
	case ENTITY_TYPE::NO_TYPE:
		break;
	default:
		break;
	}
	
	return false;
}

// Utility: return the walkability value of a tile
uchar ModulePathfinding::GetTileAt(const std::pair<int, int>& pos) const
{
	if (CheckBoundaries(pos))
		return map[(pos.second*width) + pos.first];

	return INVALID_WALK_CODE;
}

// To request all tiles involved in the last generated path
const std::vector<std::pair<int, int>>* ModulePathfinding::GetLastPath() const
{
	return &last_path;
}

// PathList ------------------------------------------------------------------------
// Looks for a node in this list and returns it's list node or NULL
// ---------------------------------------------------------------------------------
const PathNode* PathList::Find(const std::pair<int, int>& point) const
{
	std::list<PathNode>::const_iterator item = list.begin();
	while (item != list.end())
	{
		if ((*item).pos == point)
			return &(*item);
		++item;
	}
	return NULL;
}

// PathList ------------------------------------------------------------------------
// Returns the Pathnode with lowest score in this list or NULL if empty
// ---------------------------------------------------------------------------------
const PathNode* PathList::GetNodeLowestScore() const
{
	const PathNode* ret = NULL;
	int min = INT_MAX;

	std::list<PathNode>::const_reverse_iterator item = list.rbegin();
	while (item != list.rend())
	{
		if ((*item).Score() < min)
		{
			min = (*item).Score();
			ret = &(*item);
		}
		++item;
	}
	return ret;
}

// PathNode -------------------------------------------------------------------------
// Convenient constructors
// ----------------------------------------------------------------------------------
PathNode::PathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL)
{}

PathNode::PathNode(int g, int h, const std::pair<int, int>& pos, const PathNode* parent) : g(g), h(h), pos(pos), parent(parent)
{}

PathNode::PathNode(const PathNode& node) : g(node.g), h(node.h), pos(node.pos), parent(node.parent)
{}

// PathNode -------------------------------------------------------------------------
// Fills a list (PathList) of all valid adjacent pathnodes
// ----------------------------------------------------------------------------------
uint PathNode::FindWalkableAdjacents(PathList& list_to_fill) const
{
	
	std::pair<int, int> cell;
	uint before = list_to_fill.list.size();

	// north
	cell.first = pos.first;
	cell.second = pos.second + 1;
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this));

	// south
	cell.first = pos.first;
	cell.second = pos.second - 1;
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this));

	// east
	cell.first = pos.first + 1;
	cell.second = pos.second;
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this));

	// west
	cell.first = pos.first - 1;
	cell.second = pos.second;
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this));

	return list_to_fill.list.size();
}

uint PathNode::FindWalkableAdjacentsBishop(PathList & list_to_fill, int Movment_Points) const
{
	std::pair<int, int> cell;
	uint before = list_to_fill.list.size();

	// north
	for (int i = 1; i <= Movment_Points; i++) {
		cell.first = pos.first + i;
		cell.second = pos.second + i;
		if (App->pathfinding->IsWalkable(cell))
			list_to_fill.list.push_back(PathNode(-1, -1, cell, this));
		else
			break;
	}
	

	// south
	for (int i = 1; i <= Movment_Points; i++) {
		cell.first = pos.first + i;
		cell.second = pos.second - i;
		if (App->pathfinding->IsWalkable(cell))
			list_to_fill.list.push_back(PathNode(-1, -1, cell, this));
		else
			break;

	}
	// east
	for (int i = 1; i <= Movment_Points; i++) {
		cell.first = pos.first - i;
		cell.second = pos.second + i;
		if (App->pathfinding->IsWalkable(cell))
			list_to_fill.list.push_back(PathNode(-1, -1, cell, this));
		else
			break;
	}
	// west
	for (int i = 1; i <= Movment_Points; i++) {
		cell.first = pos.first - i;
		cell.second = pos.second - i;
		if (App->pathfinding->IsWalkable(cell))
			list_to_fill.list.push_back(PathNode(-1, -1, cell, this));
		else
			break;
	}
	return list_to_fill.list.size();
}

uint PathNode::FindWalkableAdjacentsTower(PathList & list_to_fill, int Movment_Points) const
{
	std::pair<int, int> cell;
	uint before = list_to_fill.list.size();

	// north
	for (int i = 1; i <= Movment_Points; i++) {
		cell.first = pos.first + i;
		cell.second = pos.second;
		if (App->pathfinding->IsWalkable(cell))
			list_to_fill.list.push_back(PathNode(-1, -1, cell, this));
	}


	// south
	for (int i = 1; i <= Movment_Points; i++) {
		cell.first = pos.first - i;
		cell.second = pos.second;
		if (App->pathfinding->IsWalkable(cell))
			list_to_fill.list.push_back(PathNode(-1, -1, cell, this));
	}
	// east
	for (int i = 1; i <= Movment_Points; i++) {
		cell.first = pos.first;
		cell.second = pos.second + i;
		if (App->pathfinding->IsWalkable(cell))
			list_to_fill.list.push_back(PathNode(-1, -1, cell, this));
	}
	// west
	for (int i = 1; i <= Movment_Points; i++) {
		cell.first = pos.first;
		cell.second = pos.second - i;
		if (App->pathfinding->IsWalkable(cell))
			list_to_fill.list.push_back(PathNode(-1, -1, cell, this));
	}
	return list_to_fill.list.size();
}

// PathNode -------------------------------------------------------------------------
// Calculates this tile score
// ----------------------------------------------------------------------------------
int PathNode::Score() const
{
	return g + h;
}

// PathNode -------------------------------------------------------------------------
// Calculate the F for a specific destination tile
// ----------------------------------------------------------------------------------
int PathNode::CalculateF(const std::pair<int, int>& destination)
{
	g = parent->g + 1;
	h = sqrt(((pos.first - destination.first) * (pos.first - destination.first)) + ((pos.second - destination.second) * (pos.second - destination.second)));

	return g + h;
}

// ----------------------------------------------------------------------------------
// Actual A* algorithm: return number of steps in the creation of the path or -1 ----
// ----------------------------------------------------------------------------------
int ModulePathfinding::CreatePath(const std::pair<int, int>& origin, const std::pair<int, int>& destination)
{
	last_path.clear();

	//if (!IsWalkable(origin) || !IsWalkable(destination)) return -1;

	PathList open;
	PathList close;

	PathNode node_origin;
	node_origin.g = 0;
	node_origin.h = (((origin.first - destination.first) * (origin.first - destination.first)) + ((origin.second - destination.second) * (origin.second - destination.second)));
	node_origin.pos = origin;
	node_origin.parent = nullptr;
	open.list.push_back(node_origin);

	while (open.list.size() > 0)
	{
		const PathNode* current_node = open.GetNodeLowestScore();
		// TODO 3: Move the lowest score cell from open list to the closed list
		close.list.push_back(*current_node);



		// TODO 4: If we just added the destination, we are done!
		// Backtrack to create the final path
		// Use the Pathnode::parent and Flip() the path when you are finish
		if (current_node->pos == destination)
		{
			const PathNode* node = nullptr;
			for (node = &(*current_node); node->pos != origin; node = node->parent) {
				last_path.push_back(node->pos);
			}
			last_path.push_back(node->pos);
			std::reverse(last_path.begin(), last_path.end());
			break;
		}

		std::list<PathNode>::iterator it = open.list.begin();
		while (it != open.list.end()) {

			if (&(*it) == &(*current_node))
				break;
			++it;
		}
		open.list.erase(it);

		// TODO 5: Fill a list of all adjancent nodes
		PathList childs;
		close.list.back().FindWalkableAdjacents(childs);


		// TODO 6: Iterate adjancent nodes:					
		std::list<PathNode>::iterator child_node = childs.list.begin();
		while (child_node != childs.list.end())
		{
			if (!close.Find((*child_node).pos) != NULL)	// ignore nodes in the closed list
			{
				(*child_node).CalculateF(destination); // If it is NOT found, calculate its F and add it to the open list
				if (open.Find((*child_node).pos) != NULL) // If it is already in the open list, check if it is a better path (compare G)
				{
					if ((*open.Find((*child_node).pos)).g > (*child_node).g)
					{
						PathNode old_node = *open.Find((*child_node).pos); // If it is a better path, Update the parent
						old_node.parent = (*child_node).parent;
					}
				}
				else open.list.push_back(*child_node);
			}
			++child_node;
		}
	}




	return -1;
}

int ModulePathfinding::CreatePathBishop(const std::pair<int, int>& origin, const std::pair<int, int>& destination, int Movment)
{
	last_path.clear();

	//if (!IsWalkable(origin) || !IsWalkable(destination)) return -1;

	PathList open;
	PathList close;

	PathNode node_origin;
	node_origin.g = 0;
	node_origin.h = (((origin.first - destination.first) * (origin.first - destination.first)) + ((origin.second - destination.second) * (origin.second - destination.second)));
	node_origin.pos = origin;
	node_origin.parent = nullptr;
	open.list.push_back(node_origin);

	while (open.list.size() > 0)
	{
		const PathNode* current_node = open.GetNodeLowestScore();
		// TODO 3: Move the lowest score cell from open list to the closed list
		close.list.push_back(*current_node);



		// TODO 4: If we just added the destination, we are done!
		// Backtrack to create the final path
		// Use the Pathnode::parent and Flip() the path when you are finish
		if ((current_node->pos.second == destination.second + 1 && current_node->pos.first == destination.first) || (current_node->pos.second == destination.second - 1 && current_node->pos.first == destination.first) || (current_node->pos.second == destination.second - 1 && current_node->pos.first == destination.first + 1) || (current_node->pos.second == destination.second + 1 && current_node->pos.first == destination.first - 1))
		{
			const PathNode* node = nullptr;
			for (node = &(*current_node); node->pos != origin; node = node->parent) {
				last_path.push_back(node->pos);
			}
			last_path.push_back(node->pos);
			std::reverse(last_path.begin(), last_path.end());
			break;
		}

		std::list<PathNode>::iterator it = open.list.begin();
		while (it != open.list.end()) {

			if (&(*it) == &(*current_node))
				break;
			++it;
		}
		open.list.erase(it);

		// TODO 5: Fill a list of all adjancent nodes
		PathList childs;
		close.list.back().FindWalkableAdjacentsBishop(childs, Movment);


		// TODO 6: Iterate adjancent nodes:					
		std::list<PathNode>::iterator child_node = childs.list.begin();
		while (child_node != childs.list.end())
		{
			if (!close.Find((*child_node).pos) != NULL)	// ignore nodes in the closed list
			{
				(*child_node).CalculateF(destination); // If it is NOT found, calculate its F and add it to the open list
				if (open.Find((*child_node).pos) != NULL) // If it is already in the open list, check if it is a better path (compare G)
				{
					if ((*open.Find((*child_node).pos)).g > (*child_node).g)
					{
						PathNode old_node = *open.Find((*child_node).pos); // If it is a better path, Update the parent
						old_node.parent = (*child_node).parent;
					}
				}
				else open.list.push_back(*child_node);
			}
			++child_node;
		}
	}




	return -1;
}

int ModulePathfinding::CreatePathTower(const std::pair<int, int>& origin, const std::pair<int, int>& destination, int Movment)
{
	last_path.clear();

	//if (!IsWalkable(origin) || !IsWalkable(destination)) return -1;

	PathList open;
	PathList close;

	PathNode node_origin;
	node_origin.g = 0;
	node_origin.h = (((origin.first - destination.first) * (origin.first - destination.first)) + ((origin.second - destination.second) * (origin.second - destination.second)));
	node_origin.pos = origin;
	node_origin.parent = nullptr;
	open.list.push_back(node_origin);

	while (open.list.size() > 0)
	{
		const PathNode* current_node = open.GetNodeLowestScore();
		// TODO 3: Move the lowest score cell from open list to the closed list
		close.list.push_back(*current_node);



		// TODO 4: If we just added the destination, we are done!
		// Backtrack to create the final path
		// Use the Pathnode::parent and Flip() the path when you are finish
		if (current_node->pos == destination)
		{
			const PathNode* node = nullptr;
			for (node = &(*current_node); node->pos != origin; node = node->parent) {
				last_path.push_back(node->pos);
			}
			last_path.push_back(node->pos);
			std::reverse(last_path.begin(), last_path.end());
			break;
		}

		std::list<PathNode>::iterator it = open.list.begin();
		while (it != open.list.end()) {

			if (&(*it) == &(*current_node))
				break;
			++it;
		}
		open.list.erase(it);

		// TODO 5: Fill a list of all adjancent nodes
		PathList childs;
		close.list.back().FindWalkableAdjacentsTower(childs, Movment);


		// TODO 6: Iterate adjancent nodes:					
		std::list<PathNode>::iterator child_node = childs.list.begin();
		while (child_node != childs.list.end())
		{
			if (!close.Find((*child_node).pos) != NULL)	// ignore nodes in the closed list
			{
				(*child_node).CalculateF(destination); // If it is NOT found, calculate its F and add it to the open list
				if (open.Find((*child_node).pos) != NULL) // If it is already in the open list, check if it is a better path (compare G)
				{
					if ((*open.Find((*child_node).pos)).g > (*child_node).g)
					{
						PathNode old_node = *open.Find((*child_node).pos); // If it is a better path, Update the parent
						old_node.parent = (*child_node).parent;
					}
				}
				else open.list.push_back(*child_node);
			}
			++child_node;
		}
	}




	return -1;
}
