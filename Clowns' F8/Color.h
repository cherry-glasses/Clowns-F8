#ifndef _Color_H_
#define _Color_H_

struct Color
{
	float r, g, b, a;

	Color() : r(0.0f), g(0.0f), b(0.0f), a(1.0f)
	{}

	Color(float r, float g, float b, float a = 1.0f) : r(r), g(g), b(b), a(a)
	{}

	void Set(float r, float g, float b, float a = 1.0f)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	float* operator & ()
	{
		return (float*)this;
	}
};

extern Color Red;
extern Color Green;
extern Color Blue;
extern Color Black;
extern Color White;
extern Color Cian;
extern Color Yellow;
extern Color Pink;
extern Color DarkGray;
extern Color Scene_1_color;
extern Color Scene_2_color;
extern Color Scene_3_color;
extern Color Scene_4_color;
extern Color Defend_color;
extern Color Crit_color;
extern Color Damaged_color;
extern Color Alpha_color;

#endif
