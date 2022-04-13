#pragma once
#include "StageBase.h"

class Stage6 : public StageBase
{
public:
	Stage6();
	~Stage6();

public:
	Stage6(const Stage6& _Other) = delete;
	Stage6(const Stage6&& _Other) = delete;

public:
	Stage6 operator=(const Stage6& _Other) = delete;
	Stage6 operator=(const Stage6&& _Other) = delete;

public:
	void Start() override;
	void SceneChangeStart() override;
};