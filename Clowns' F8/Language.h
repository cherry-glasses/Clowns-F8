#ifndef __Language_H_
#define __Language_H_

enum LANGUAGE_TYPE {
	ENGLISH,
	SPANISH
};

class Language
{
public:

	Language();
	~Language() {};

	void SetLanguage(LANGUAGE_TYPE language);

public:
	std::string new_game = "";
	std::string load_game = "";
	std::string options = "";
	std::string credits = "";
	std::string exit = "";
	std::string choose_language = "";
	std::string english = "";
	std::string spanish = "";
	std::string volume = "";
	std::string controls = "";
	std::string back = "";
	std::string accept = "";
	std::string decline = "";
	std::string character_stats = "";
	std::string character_abilities = "";
	std::string abilities = "";
	std::string pause = "";
	std::string select = "";

};


#endif //!__Language_H_