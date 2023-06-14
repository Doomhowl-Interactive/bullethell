#include <memory>

template <typename T> class Service {
   public:
    static std::shared_ptr<T> Initialize();

   protected:
    explicit Service(const std::string& name);
    static std::shared_ptr<T> Instance();
    ~Service();

   private:
    static std::weak_ptr<T> instance;
    std::string name;
};
