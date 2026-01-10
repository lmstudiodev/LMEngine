#pragma once
#include "Global.h"

class Resource
{
public:
	Resource(const wchar_t* full_path);
	virtual ~Resource();

protected:
	std::wstring m_fullPath;
};

