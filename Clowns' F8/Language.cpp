#include "Application.h"
#include "Language.h"

Language::Language()
{
	SetLanguage(ENGLISH);
}

void Language::SetLanguage(LANGUAGE_TYPE _language)
{
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
	}
	


}