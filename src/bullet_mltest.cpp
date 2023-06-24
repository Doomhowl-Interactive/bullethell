#include <filesystem>
#include <string>
#include <iostream>

#include <bulletml/bulletmlparser.h>
#include <bulletml/bulletmlrunner.h>
#include <bulletml/bulletmlrunnerimpl.h>
#include <bulletml/bulletmlparser-tinyxml.h>

#include "basalt.h"
#include "bullet_mltest.h"

using namespace std;
namespace fs = std::filesystem;

void InitBulletMLTest()
{
    const string path = "../../assets/pat_test.xml";
    if (!fs::exists(path))
    {
        string msg = "BulletML_XML file " + path + " not found";
        SDL_LogError(0, "%s", msg.c_str());
        return;
    }

    auto parser = new BulletMLParserTinyXML(path.c_str());
    parser->parse();
}

void UpdateBulletMLTest(float delta)
{
}

void RenderBulletMLTest(Texture texture)
{
}