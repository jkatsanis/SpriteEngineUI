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
    std::string s2 = s2d::EngineData::s_pathToUserProject + "\\assets\\Sprites\\down.png";

    // After 200ms to this
    s2d::KeyFrame x = s2d::KeyFrame(s1, 3);

    // s1 stuck 200ms 
    s2d::KeyFrame y = s2d::KeyFrame(s2, 5);
    s2d::KeyFrame z = s2d::KeyFrame(s2, 2);
    // s2d::KeyFrame z = s2d::KeyFrame(s2, 0);

     // Directly to start

    std::vector<s2d::KeyFrame> path = { x, y, z };

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

