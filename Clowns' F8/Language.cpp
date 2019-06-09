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
		keyboard = config.child("main_menu_scene").child("keyboard").attribute("value").as_string("");
		gamepad = config.child("main_menu_scene").child("gamepad").attribute("value").as_string("");
		character_stats = config.child("main_menu_scene").child("character_stats").attribute("value").as_string("");
		character_abilities = config.child("main_menu_scene").child("character_abilities").attribute("value").as_string("");
		abilities = config.child("main_menu_scene").child("abilities").attribute("value").as_string("");
		pause = config.child("main_menu_scene").child("pause").attribute("value").as_string("");
		select = config.child("main_menu_scene").child("select").attribute("value").as_string("");

		//credits
		credits_lines.push_back(config.child("main_menu_scene").child("credits0").attribute("value").as_string(""));
		credits_lines.push_back(config.child("main_menu_scene").child("credits1").attribute("value").as_string(""));
		credits_lines.push_back(config.child("main_menu_scene").child("credits2").attribute("value").as_string(""));
		credits_lines.push_back(config.child("main_menu_scene").child("credits3").attribute("value").as_string(""));
		credits_lines.push_back(config.child("main_menu_scene").child("credits4").attribute("value").as_string(""));
		credits_lines.push_back(config.child("main_menu_scene").child("credits5").attribute("value").as_string(""));
		credits_lines.push_back(config.child("main_menu_scene").child("credits6").attribute("value").as_string(""));
		credits_lines.push_back(config.child("main_menu_scene").child("credits7").attribute("value").as_string(""));
		credits_lines.push_back(config.child("main_menu_scene").child("credits8").attribute("value").as_string(""));
		credits_lines.push_back(config.child("main_menu_scene").child("credits9").attribute("value").as_string(""));
		credits_lines.push_back(config.child("main_menu_scene").child("credits10").attribute("value").as_string(""));
		credits_lines.push_back(config.child("main_menu_scene").child("credits11").attribute("value").as_string(""));
		credits_lines.push_back(config.child("main_menu_scene").child("credits12").attribute("value").as_string(""));
		credits_lines.push_back(config.child("main_menu_scene").child("credits13").attribute("value").as_string(""));

		resume = config.child("battle_scene").child("resume").attribute("value").as_string("");
		choose_map = config.child("battle_scene").child("choose_map").attribute("value").as_string("");
		battle_abilities = config.child("battle_scene").child("battle_abilities").attribute("value").as_string("");
		defend = config.child("battle_scene").child("defend").attribute("value").as_string("");

		win_accept = config.child("win_scene").child("press_accept").attribute("value").as_string("");

		map1 = config.child("choose_map_scene").child("map1").attribute("value").as_string("");
		map2 = config.child("choose_map_scene").child("map2").attribute("value").as_string("");
		map3 = config.child("choose_map_scene").child("map3").attribute("value").as_string("");
		map4 = config.child("choose_map_scene").child("map4").attribute("value").as_string("");
		save_game = config.child("choose_map_scene").child("save_game").attribute("value").as_string("");
		saved = config.child("choose_map_scene").child("saved").attribute("value").as_string("");
		main_menu = config.child("choose_map_scene").child("main_menu").attribute("value").as_string("");

		lines.push_back(config.child("intro_scene").child("line1").attribute("value").as_string(""));
		lines.push_back(config.child("intro_scene").child("line2").attribute("value").as_string(""));
		lines.push_back(config.child("intro_scene").child("line3").attribute("value").as_string(""));
		lines.push_back(config.child("intro_scene").child("line4").attribute("value").as_string(""));
		lines.push_back(config.child("intro_scene").child("line5").attribute("value").as_string(""));
		lines.push_back(config.child("intro_scene").child("line6").attribute("value").as_string(""));
		lines.push_back(config.child("intro_scene").child("line7").attribute("value").as_string(""));
		lines.push_back(config.child("intro_scene").child("line8").attribute("value").as_string(""));

		lines.push_back(config.child("intro_scene").child("line9").attribute("value").as_string(""));
		lines.push_back(config.child("intro_scene").child("line10").attribute("value").as_string(""));
		lines.push_back(config.child("intro_scene").child("line11").attribute("value").as_string(""));
		lines.push_back(config.child("intro_scene").child("line12").attribute("value").as_string(""));
		lines.push_back(config.child("intro_scene").child("line13").attribute("value").as_string(""));
		lines.push_back(config.child("intro_scene").child("line14").attribute("value").as_string(""));
		lines.push_back(config.child("intro_scene").child("line15").attribute("value").as_string(""));
		lines.push_back(config.child("intro_scene").child("line16").attribute("value").as_string(""));
		lines.push_back(config.child("intro_scene").child("line17").attribute("value").as_string(""));
		lines.push_back(config.child("intro_scene").child("line18").attribute("value").as_string(""));
		lines.push_back(config.child("intro_scene").child("line19").attribute("value").as_string(""));
		lines.push_back(config.child("intro_scene").child("line20").attribute("value").as_string(""));
		lines.push_back(config.child("intro_scene").child("line21").attribute("value").as_string(""));
		lines.push_back(config.child("intro_scene").child("line22").attribute("value").as_string(""));

		end_lines.push_back(config.child("ending_scene").child("line1").attribute("value").as_string(""));
		end_lines.push_back(config.child("ending_scene").child("line2").attribute("value").as_string(""));
		end_lines.push_back(config.child("ending_scene").child("line3").attribute("value").as_string(""));
		end_lines.push_back(config.child("ending_scene").child("line4").attribute("value").as_string(""));
		end_lines.push_back(config.child("ending_scene").child("line5").attribute("value").as_string(""));
		end_lines.push_back(config.child("ending_scene").child("line6").attribute("value").as_string(""));

		attack_sapphire = config.child("battle_scene").child("sapphire").attribute("attack").as_string("");
		attack_iris = config.child("battle_scene").child("iris").attribute("attack").as_string("");
		attack_storm = config.child("battle_scene").child("storm").attribute("attack").as_string("");
		attack_georgeb = config.child("battle_scene").child("georgeb").attribute("attack").as_string("");

		ability1_sapphire = config.child("battle_scene").child("sapphire").attribute("ability_1").as_string("");
		ability1_iris = config.child("battle_scene").child("iris").attribute("ability_1").as_string("");
		ability1_storm = config.child("battle_scene").child("storm").attribute("ability_1").as_string("");
		ability1_georgeb = config.child("battle_scene").child("georgeb").attribute("ability_1").as_string("");

		ability2_sapphire = config.child("battle_scene").child("sapphire").attribute("ability_2").as_string("");
		ability2_iris = config.child("battle_scene").child("iris").attribute("ability_2").as_string("");
		ability2_storm = config.child("battle_scene").child("storm").attribute("ability_2").as_string("");
		ability2_georgeb = config.child("battle_scene").child("georgeb").attribute("ability_2").as_string("");

		ability3_sapphire = config.child("battle_scene").child("sapphire").attribute("ability_3").as_string("");
		ability3_iris = config.child("battle_scene").child("iris").attribute("ability_3").as_string("");
		ability3_storm = config.child("battle_scene").child("storm").attribute("ability_3").as_string("");
		ability3_georgeb = config.child("battle_scene").child("georgeb").attribute("ability_3").as_string("");

		//ability info
		range = config.child("battle_scene").child("ability_info").attribute("range").as_string("");
		target = config.child("battle_scene").child("ability_info").attribute("target").as_string("");
		ability_type = config.child("battle_scene").child("ability_info").attribute("type").as_string("");
		effect = config.child("battle_scene").child("ability_info").attribute("effect").as_string("");
		amount = config.child("battle_scene").child("ability_info").attribute("amount").as_string("");

		//range values

		range_val.push_back(config.child("battle_scene").child("iris_info").child("attack").attribute("range").as_string(""));
		range_val.push_back(config.child("battle_scene").child("iris_info").child("ability1").attribute("range").as_string(""));
		range_val.push_back(config.child("battle_scene").child("iris_info").child("ability2").attribute("range").as_string(""));
		range_val.push_back(config.child("battle_scene").child("iris_info").child("ability3").attribute("range").as_string(""));

		range_val.push_back(config.child("battle_scene").child("georgeb_info").child("attack").attribute("range").as_string(""));
		range_val.push_back(config.child("battle_scene").child("georgeb_info").child("ability1").attribute("range").as_string(""));
		range_val.push_back(config.child("battle_scene").child("georgeb_info").child("ability2").attribute("range").as_string(""));
		range_val.push_back(config.child("battle_scene").child("georgeb_info").child("ability3").attribute("range").as_string(""));

		range_val.push_back(config.child("battle_scene").child("sapphire_info").child("attack").attribute("range").as_string(""));
		range_val.push_back(config.child("battle_scene").child("sapphire_info").child("ability1").attribute("range").as_string(""));
		range_val.push_back(config.child("battle_scene").child("sapphire_info").child("ability2").attribute("range").as_string(""));
		range_val.push_back(config.child("battle_scene").child("sapphire_info").child("ability3").attribute("range").as_string(""));

		range_val.push_back(config.child("battle_scene").child("storm_info").child("attack").attribute("range").as_string(""));
		range_val.push_back(config.child("battle_scene").child("storm_info").child("ability1").attribute("range").as_string(""));
		range_val.push_back(config.child("battle_scene").child("storm_info").child("ability2").attribute("range").as_string(""));
		range_val.push_back(config.child("battle_scene").child("storm_info").child("ability3").attribute("range").as_string(""));

		//target values

		target_val.push_back(config.child("battle_scene").child("iris_info").child("attack").attribute("target").as_string(""));
		target_val.push_back(config.child("battle_scene").child("iris_info").child("ability1").attribute("target").as_string(""));
		target_val.push_back(config.child("battle_scene").child("iris_info").child("ability2").attribute("target").as_string(""));
		target_val.push_back(config.child("battle_scene").child("iris_info").child("ability3").attribute("target").as_string(""));

		target_val.push_back(config.child("battle_scene").child("georgeb_info").child("attack").attribute("target").as_string(""));
		target_val.push_back(config.child("battle_scene").child("georgeb_info").child("ability1").attribute("target").as_string(""));
		target_val.push_back(config.child("battle_scene").child("georgeb_info").child("ability2").attribute("target").as_string(""));
		target_val.push_back(config.child("battle_scene").child("georgeb_info").child("ability3").attribute("target").as_string(""));

		target_val.push_back(config.child("battle_scene").child("sapphire_info").child("attack").attribute("target").as_string(""));
		target_val.push_back(config.child("battle_scene").child("sapphire_info").child("ability1").attribute("target").as_string(""));
		target_val.push_back(config.child("battle_scene").child("sapphire_info").child("ability2").attribute("target").as_string(""));
		target_val.push_back(config.child("battle_scene").child("sapphire_info").child("ability3").attribute("target").as_string(""));

		target_val.push_back(config.child("battle_scene").child("storm_info").child("attack").attribute("target").as_string(""));
		target_val.push_back(config.child("battle_scene").child("storm_info").child("ability1").attribute("target").as_string(""));
		target_val.push_back(config.child("battle_scene").child("storm_info").child("ability2").attribute("target").as_string(""));
		target_val.push_back(config.child("battle_scene").child("storm_info").child("ability3").attribute("target").as_string(""));

		//type values

		ability_type_val.push_back(config.child("battle_scene").child("iris_info").child("attack").attribute("type").as_string(""));
		ability_type_val.push_back(config.child("battle_scene").child("iris_info").child("ability1").attribute("type").as_string(""));
		ability_type_val.push_back(config.child("battle_scene").child("iris_info").child("ability2").attribute("type").as_string(""));
		ability_type_val.push_back(config.child("battle_scene").child("iris_info").child("ability3").attribute("type").as_string(""));

		ability_type_val.push_back(config.child("battle_scene").child("georgeb_info").child("attack").attribute("type").as_string(""));
		ability_type_val.push_back(config.child("battle_scene").child("georgeb_info").child("ability1").attribute("type").as_string(""));
		ability_type_val.push_back(config.child("battle_scene").child("georgeb_info").child("ability2").attribute("type").as_string(""));
		ability_type_val.push_back(config.child("battle_scene").child("georgeb_info").child("ability3").attribute("type").as_string(""));

		ability_type_val.push_back(config.child("battle_scene").child("sapphire_info").child("attack").attribute("type").as_string(""));
		ability_type_val.push_back(config.child("battle_scene").child("sapphire_info").child("ability1").attribute("type").as_string(""));
		ability_type_val.push_back(config.child("battle_scene").child("sapphire_info").child("ability2").attribute("type").as_string(""));
		ability_type_val.push_back(config.child("battle_scene").child("sapphire_info").child("ability3").attribute("type").as_string(""));

		ability_type_val.push_back(config.child("battle_scene").child("storm_info").child("attack").attribute("type").as_string(""));
		ability_type_val.push_back(config.child("battle_scene").child("storm_info").child("ability1").attribute("type").as_string(""));
		ability_type_val.push_back(config.child("battle_scene").child("storm_info").child("ability2").attribute("type").as_string(""));
		ability_type_val.push_back(config.child("battle_scene").child("storm_info").child("ability3").attribute("type").as_string(""));

		//effect values

		effect_val.push_back(config.child("battle_scene").child("iris_info").child("attack").attribute("effect").as_string(""));
		effect_val.push_back(config.child("battle_scene").child("iris_info").child("ability1").attribute("effect").as_string(""));
		effect_val.push_back(config.child("battle_scene").child("iris_info").child("ability2").attribute("effect").as_string(""));
		effect_val.push_back(config.child("battle_scene").child("iris_info").child("ability3").attribute("effect").as_string(""));

		effect_val.push_back(config.child("battle_scene").child("georgeb_info").child("attack").attribute("effect").as_string(""));
		effect_val.push_back(config.child("battle_scene").child("georgeb_info").child("ability1").attribute("effect").as_string(""));
		effect_val.push_back(config.child("battle_scene").child("georgeb_info").child("ability2").attribute("effect").as_string(""));
		effect_val.push_back(config.child("battle_scene").child("georgeb_info").child("ability3").attribute("effect").as_string(""));

		effect_val.push_back(config.child("battle_scene").child("sapphire_info").child("attack").attribute("effect").as_string(""));
		effect_val.push_back(config.child("battle_scene").child("sapphire_info").child("ability1").attribute("effect").as_string(""));
		effect_val.push_back(config.child("battle_scene").child("sapphire_info").child("ability2").attribute("effect").as_string(""));
		effect_val.push_back(config.child("battle_scene").child("sapphire_info").child("ability3").attribute("effect").as_string(""));

		effect_val.push_back(config.child("battle_scene").child("storm_info").child("attack").attribute("effect").as_string(""));
		effect_val.push_back(config.child("battle_scene").child("storm_info").child("ability1").attribute("effect").as_string(""));
		effect_val.push_back(config.child("battle_scene").child("storm_info").child("ability2").attribute("effect").as_string(""));
		effect_val.push_back(config.child("battle_scene").child("storm_info").child("ability3").attribute("effect").as_string(""));

		//amount values

		amount_val.push_back(config.child("battle_scene").child("iris_info").child("attack").attribute("amount").as_string(""));
		amount_val.push_back(config.child("battle_scene").child("iris_info").child("ability1").attribute("amount").as_string(""));
		amount_val.push_back(config.child("battle_scene").child("iris_info").child("ability2").attribute("amount").as_string(""));
		amount_val.push_back(config.child("battle_scene").child("iris_info").child("ability3").attribute("amount").as_string(""));

		amount_val.push_back(config.child("battle_scene").child("georgeb_info").child("attack").attribute("amount").as_string(""));
		amount_val.push_back(config.child("battle_scene").child("georgeb_info").child("ability1").attribute("amount").as_string(""));
		amount_val.push_back(config.child("battle_scene").child("georgeb_info").child("ability2").attribute("amount").as_string(""));
		amount_val.push_back(config.child("battle_scene").child("georgeb_info").child("ability3").attribute("amount").as_string(""));

		amount_val.push_back(config.child("battle_scene").child("sapphire_info").child("attack").attribute("amount").as_string(""));
		amount_val.push_back(config.child("battle_scene").child("sapphire_info").child("ability1").attribute("amount").as_string(""));
		amount_val.push_back(config.child("battle_scene").child("sapphire_info").child("ability2").attribute("amount").as_string(""));
		amount_val.push_back(config.child("battle_scene").child("sapphire_info").child("ability3").attribute("amount").as_string(""));

		amount_val.push_back(config.child("battle_scene").child("storm_info").child("attack").attribute("amount").as_string(""));
		amount_val.push_back(config.child("battle_scene").child("storm_info").child("ability1").attribute("amount").as_string(""));
		amount_val.push_back(config.child("battle_scene").child("storm_info").child("ability2").attribute("amount").as_string(""));
		amount_val.push_back(config.child("battle_scene").child("storm_info").child("ability3").attribute("amount").as_string(""));

		//tutorial

		lets_do_it_tutorial = config.child("battle_scene").child("lets").attribute("value").as_string("");
		greatings_tutorial = config.child("battle_scene").child("greating").attribute("value").as_string("");
		movemnent_tutorial = config.child("battle_scene").child("move").attribute("value").as_string("");
		defens_tutorial = config.child("battle_scene").child("def").attribute("value").as_string("");
		greatings_2 = config.child("battle_scene").child("greating_2").attribute("value").as_string("");
		greatings_3 = config.child("battle_scene").child("greating_3").attribute("value").as_string("");
		movemnent_2 = config.child("battle_scene").child("move_2").attribute("value").as_string("");
		movemnent_3 = config.child("battle_scene").child("move_3").attribute("value").as_string("");
		movemnent_4 = config.child("battle_scene").child("move_4").attribute("value").as_string("");
		movemnent_5 = config.child("battle_scene").child("move_5").attribute("value").as_string("");
		movemnent_6 = config.child("battle_scene").child("move_6").attribute("value").as_string("");
		defens_2 = config.child("battle_scene").child("def_2").attribute("value").as_string("");
		defens_3 = config.child("battle_scene").child("def_3").attribute("value").as_string("");
		defens_4 = config.child("battle_scene").child("def_4").attribute("value").as_string("");
		defens_5 = config.child("battle_scene").child("def_5").attribute("value").as_string("");
		defens_6 = config.child("battle_scene").child("def_6").attribute("value").as_string("");

	}
}

void Language::change_language()
{
	lines.clear();
	end_lines.clear();
	range_val.clear();
	target_val.clear();
	ability_type_val.clear();
	effect_val.clear();
	amount_val.clear();
	credits_lines.clear();
}