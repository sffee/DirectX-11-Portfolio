#pragma once
#include "StageBase.h"

class Stage2 : public StageBase
{
public:
	Stage2();
	~Stage2();

public:
	Stage2(const Stage2& _Other) = delete;
	Stage2(const Stage2&& _Other) = delete;

public:
	Stage2 operator=(const Stage2& _Other) = delete;
	Stage2 operator=(const Stage2&& _Other) = delete;

public:
	void Start() override;
	void SceneChangeStart() override;
};