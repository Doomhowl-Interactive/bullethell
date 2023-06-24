#pragma once
#include "basalt.h"

#ifdef __cplusplus
extern "C"
{
#endif

    void InitBulletMLTest();
    void UpdateBulletMLTest(float delta);
    void RenderBulletMLTest(Texture texture);

#ifdef __cplusplus
}
#endif