#pragma once

class CEngineSetting
{
protected:
	friend class CEngine;
	CEngineSetting();
	virtual ~CEngineSetting();
public:
	virtual void Init();
};

