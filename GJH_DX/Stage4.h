#pragma once
#include "StageBase.h"

class Stage4 : public StageBase
{
public:
	Stage4();
	~Stage4();

public:
	Stage4(const Stage4& _Other) = delete;
	Stage4(const Stage4&& _Other) = delete;

public:
	Stage4 operator=(const Stage4& _Other) = delete;
	Stage4 operator=(const Stage4&& _Other) = delete;

public:
	void Start() override;
};