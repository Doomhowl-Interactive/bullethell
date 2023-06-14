#include <string>

#include <unordered_map>

#include "bullet_common.hpp"
#include "bullet_service.hpp"

class Assets : public Service<Assets> {
   public:
    static Texture RequestTexture(const std::string& name);

   private:
    std::unordered_map<std::string, Texture> textures;
    Assets() : Service("Assets"){};
    ~Assets();
};
