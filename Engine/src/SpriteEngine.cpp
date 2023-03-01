#include <SpriteEngine.h>   
#include <data/engineData.h>

int main()
{
    s2d::UIProjectSelector selector;

    while (selector.isOpen())
    {
        selector.update();  
    }

    // search where the error happens when u selet a invalid project
    if (s2d::EngineData::s_pathToUserProject == "")
    {
        // Closing the engine since the user doesnt want to open a project;
        return -1;
    }

    s2d::GameEngine engine;

    s2d::Sprite* spr = s2d::Sprite::activeSprites[0];

    std::string s1 = s2d::EngineData::s_pathToUserProject + "\\assets\\Sprites\\big.png";
    std::string s3 = s2d::EngineData::s_pathToUserProject + "\\assets\\Sprites\\sprite#.png";
    std::string s2 = s2d::EngineData::s_pathToUserProject + "\\assets\\Sprites\\down.png";

    s2d::KeyFrame a = s2d::KeyFrame(s3, 2);
    s2d::KeyFrame x = s2d::KeyFrame(s1, 5);

    std::vector<s2d::KeyFrame> path = { a, x };

    spr->animator.createAnimation("name", path, 100, 1);

    while (engine.ptr_renderWindow->isOpen())
    {
        if (s2d::Input::onKeyPress(s2d::KeyBoardCode::A))
        {
            spr->animator.play("name");
        }
        if (s2d::Input::onKeyPress(s2d::KeyBoardCode::B))
        {
            spr->animator.stop("name");
        }
        engine.update();
    }

    return 0;
}

