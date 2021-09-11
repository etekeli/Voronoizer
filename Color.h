#pragma once

class Color {
public:
	float r, g, b, a;
	Color() {}
	~Color() {}

	Color(int r, int g, int b, int a) {
		this->r = (float)r/255;
		this->g = (float)g/255;
		this->b = (float)b/255;
		this->a = (float)a/255;
	}
};