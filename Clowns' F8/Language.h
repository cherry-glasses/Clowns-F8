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

};


#endif //!__Language_H_