#include "imgui.h"

class UI_Element {
public:
	char* name;
	bool active;

public:
	UI_Element(char* name, bool active);
	~UI_Element();

	void Draw();
	void Activate();
};