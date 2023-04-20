#pragma once
#include "Client3D.h"

class CClient3DManager
{
public:
	CClient3DManager();
	~CClient3DManager();
	bool Init(HINSTANCE hInst);
	int Run();
};

