#include <chrono>

using Instant = std::chrono::time_point<std::chrono::system_clock>;
using Duration = std::chrono::system_clock::duration;

class CallbackInfrastructure
{
public:
    template <typename B>
    int registerCallback(Duration duration, B callback) { return 0; }
    void deregisterCallback(int id){};
};

auto Now() { return std::chrono::system_clock::now(); }

class CallbackInfrastructureBuilder
{
public:
    CallbackInfrastructureBuilder &anInstance() { return *this; }
    CallbackInfrastructureBuilder &WithDefaultTick() { return *this; }
    CallbackInfrastructure Build() { return CallbackInfrastructure{}; }
};

class Worker
{
public:
    template <typename B>
    void schedule(Duration a, B b) {}
};

class Infomap
{
public:
    template <typename B>
    int add(Duration a, B b) { return 0; }
};
