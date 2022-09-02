#ifndef EDITOR_H
#define EDITOR_H

#include "App.h"

class Editor: public App {
private:

	void init() override;
	void setScene(size_t id) override;

public:

	Editor();
	~Editor();
};

#endif
