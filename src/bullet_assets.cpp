#include "bullet_assets.hpp"
using namespace std;

Texture Assets::RequestTexture(const string& name)
{
    auto assets = Instance();
    if (assets->textures.find(name) != assets->textures.end()) {
        return assets->textures.at(name);
    }

    // Load texture from disk
    string path = ASSETS_PATH + name + ".png";
    Texture loaded = LoadTexture(path.c_str());
    assets->textures.insert({ name, loaded });
    return loaded;
}

Assets::~Assets()
{
    for (auto& [name, texture] : textures) {
        UnloadTexture(texture);
    }
}