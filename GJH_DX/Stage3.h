#pragma once
#include "StageBase.h"

class Stage3 : public StageBase
{
public:
	Stage3();
	~Stage3();

public:
	Stage3(const Stage3& _Other) = delete;
	Stage3(const Stage3&& _Other) = delete;

public:
	Stage3 operator=(const Stage3& _Other) = delete;
	Stage3 operator=(const Stage3&& _Other) = delete;

public:
	void Start() override;
	void SceneChangeStart() override;
};