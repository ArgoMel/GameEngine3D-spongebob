#include "EngineShareSetting.h"
#include "Engine.h"
#include "DefaultSetting.h"
#include "PlayerSkill.h"

void CEngineShareSetting::Setting()
{
	CEngine::GetInst()->SetEngineSetting<CDefaultSetting>();
	CEngine::GetInst()->SetEngineSetting<CPlayerSkill>();
}
