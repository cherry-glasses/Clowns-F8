#include "Application.h"
#include "Language.h"

Language::Language()
{
	SetLanguage(ENGLISH);
}

void Language::SetLanguage(LANGUAGE_TYPE _language)
{
	type = _language;
	pugi::xml_document document;
	pugi::xml_parse_result result;
	switch (_language)
	{
	case ENGLISH:
		result = document.load_file("Languages/_EN.xml");
		break;
	case SPANISH:
		result = document.load_file("Languages/_ES.xml");
		break;
	default:
		result = document.load_file("Languages/_EN.xml");
		break;
	}
	if (result != NULL)
	{
		pugi::xml_node	config = document.child("language");

		new_game = config.child("main_menu_scene").child("new_game").attribute("value").as_string("");
		load_game = config.child("main_menu_scene").child("load_game").attribute("value").as_string("");
		options = config.child("main_menu_scene").child("options").attribute("value").as_string("");
		credits = config.child("main_menu_scene").child("credits").attribute("value").as_string("");
		exit = config.child("main_menu_scene").child("exit").attribute("value").as_string("");
		choose_language = config.child("main_menu_scene").child("choose_language").attribute("value").as_string("");
		english = config.child("main_menu_scene").child("english").attribute("value").as_string("");
		spanish = config.child("main_menu_scene").child("spanish").attribute("value").as_string("");
		volume = config.child("main_menu_scene").child("volume").attribute("value").as_string("");
		controls = config.child("main_menu_scene").child("controls").attribute("value").as_string("");
		back = config.child("main_menu_scene").child("back").attribute("value").as_string("");
		accept = config.child("main_menu_scene").child("accept").attribute("value").as_string("");
		decline = config.child("main_menu_scene").child("decline").attribute("value").as_string("");
		character_stats = config.child("main_menu_scene").child("character_stats").attribute("value").as_string("");
		character_abilities = config.child("main_menu_scene").child("character_abilities").attribute("value").as_string("");
		abilities = config.child("main_menu_scene").child("abilities").attribute("value").as_string("");
		pause = config.child("main_menu_scene").child("pause").attribute("value").as_string("");
		select = config.child("main_menu_scene").child("select").attribute("value").as_string("");

		
		resume = config.child("battle_scene").child("resume").attribute("value").as_string("");
		main_menu = config.child("battle_scene").child("main_menu").attribute("value").as_string("");
		battle_abilities = config.child("battle_scene").child("battle_abilities").attribute("value").as_string("");
		defend = config.child("battle_scene").child("defend").attribute("value").as_string("");

		win_accept = config.child("win_scene").child("press_accept").attribute("value").as_string("");

		map1 = config.child("choose_map_scene").child("map1").attribute("value").as_string("");
		map2 = config.child("choose_map_scene").child("map2").attribute("value").as_string("");
		map3 = config.child("choose_map_scene").child("map3").attribute("value").as_string("");

	}
	


}