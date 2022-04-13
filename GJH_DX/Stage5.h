#pragma once
#include "StageBase.h"

class Stage5 : public StageBase
{
public:
	Stage5();
	~Stage5();

public:
	Stage5(const Stage5& _Other) = delete;
	Stage5(const Stage5&& _Other) = delete;

public:
	Stage5 operator=(const Stage5& _Other) = delete;
	Stage5 operator=(const Stage5&& _Other) = delete;

public:
	void Start() override;
};