#include <sdbusplus/bus.hpp>
#include <sdbusplus/server.hpp>
#include <xyz/openbmc_project/Example/Simple/Hello/server.hpp>

#include <string>

namespace
{

constexpr auto serviceName = "xyz.openbmc_project.Example.Simple";
constexpr auto objectPath = "/xyz/openbmc_project/example/simple/hello";

using HelloInterface =
    sdbusplus::server::object_t<sdbusplus::server::xyz::openbmc_project::
                                    example::simple::Hello>;

class HelloService : public HelloInterface
{
  public:
    HelloService(sdbusplus::bus_t& bus, const char* path) :
        HelloInterface(bus, path)
    {}

    std::string ping(std::string message) override
    {
        return "pong: " + message;
    }
};

} // namespace

int main()
{
    auto bus = sdbusplus::bus::new_default();

    sdbusplus::server::manager_t manager{bus, "/xyz/openbmc_project/example"};
    HelloService hello{bus, objectPath};

    hello.enabled(true);
    bus.request_name(serviceName);

    bus.process_loop();
}
