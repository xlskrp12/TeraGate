#pragma once
#include<vector>
#include"Prev.h"
#include"SceneIngame.h"
#include"SceneIntro.h"
class SceneManager
{
public:
    SceneManager();
    ~SceneManager();
private:
    std::vector<Scene*> _scene;
};

