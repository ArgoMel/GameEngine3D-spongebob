#pragma once
#include "EngineSetting.h"

class CPlayerSkill : public CEngineSetting
{
private:
    void SpongebobAttack(class CGameObject* obj);
    void PatrickAttack(class CGameObject* obj);
    void SandyAttack(class CGameObject* obj);
    void SpongebobRClick(class CGameObject* obj);
    void PatrickRClick(class CGameObject* obj);
    void SandyRClick(class CGameObject* obj);
public:
    CPlayerSkill();
    virtual ~CPlayerSkill();
    virtual void Init();
};