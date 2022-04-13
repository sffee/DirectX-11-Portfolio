#pragma once
#include "StageBase.h"

class Stage1 : public StageBase
{
public:
	Stage1();
	~Stage1();

public:
	Stage1(const Stage1& _Other) = delete;
	Stage1(const Stage1&& _Other) = delete;

public:
	Stage1 operator=(const Stage1& _Other) = delete;
	Stage1 operator=(const Stage1&& _Other) = delete;

public:
	void Start() override;
	void SceneChangeStart() override;
};