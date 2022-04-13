#pragma once

class HeadHunter1
{
public:
	HeadHunter1();
	~HeadHunter1();

public:
	HeadHunter1(const HeadHunter1& _Other) = delete;
	HeadHunter1(const HeadHunter1&& _Other) = delete;

public:
	HeadHunter1 operator=(const HeadHunter1& _Other) = delete;
	HeadHunter1 operator=(const HeadHunter1&& _Other) = delete;
};