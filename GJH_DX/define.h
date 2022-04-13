#pragma once

#include <GJHGameEngineInput.h>
#include <GJHGameEngineTime.h>

#define SAFE_DELETE(p) if (p) { delete p; p = nullptr; }

#define DELTATIME GJHGameEngineTime::FDeltaTime

#define CREATEKEY GJHGameEngineInput::GetInst().CreateKey

#define KEYDOWN GJHGameEngineInput::Down
#define KEYUP GJHGameEngineInput::Up
#define KEYPRESS GJHGameEngineInput::Press
#define KEYFREE GJHGameEngineInput::Free

#define SOUNDPLAY GJHGameEngineSound::Play
#define SOUNDSTOP GJHGameEngineSound::Stop
#define SOUNDFADEOUT GJHGameEngineSound::FadeOut

#define CAMSIZEX 640.f
#define CAMSIZEY 360.f