#include <chrono>

class CallbackInfrastructure
{
public:
    template <typename A, typename B>
    int registerCallback(A duration, B callback) { return 0; }
    void deregisterCallback(int id){};
};

using Instant = std::chrono::time_point<std::chrono::system_clock>;
using Duration = std::chrono::system_clock::duration;
auto Now() { return std::chrono::system_clock::now(); }

class CallbackInfrastructureBuilder
{
public:
    CallbackInfrastructureBuilder &anInstance() { return *this; }
    CallbackInfrastructureBuilder &WithDefaultTick() { return *this; }
    CallbackInfrastructure Build() { return CallbackInfrastructure{}; }
};
