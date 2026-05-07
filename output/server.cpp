#include <exception>
#include <map>
#include <sdbusplus/sdbus.hpp>
#include <sdbusplus/sdbuspp_support/server.hpp>
#include <sdbusplus/server.hpp>
#include <string>
#include <tuple>

#include <xyz/openbmc_project/Example/Simple/Hello/server.hpp>

namespace sdbusplus::server::xyz::openbmc_project::example::simple
{

int Hello::_callback_Ping(
        sd_bus_message* msg, void* context, sd_bus_error* error)
{
    auto o = static_cast<Hello*>(context);

    try
    {
        return sdbusplus::sdbuspp::method_callback(
                msg, o->get_bus().getInterface(), error,
                std::function(
                    [=](std::string&& message)
                    {
                        return o->ping(
                                message);
                    }
                ));
    }
    catch (const std::exception&)
    {
        o->get_bus().set_current_exception(std::current_exception());
        return 1;
    }
}

namespace details
{
namespace Hello
{
static const auto _param_Ping =
        utility::tuple_to_array(message::types::type_id<
                std::string>());
static const auto _return_Ping =
        utility::tuple_to_array(message::types::type_id<
                std::string>());
}
}


auto Hello::enabled() const ->
        bool
{
    return _enabled;
}

int Hello::_callback_get_Enabled(
        sd_bus* /*bus*/, const char* /*path*/, const char* /*interface*/,
        const char* /*property*/, sd_bus_message* reply, void* context,
        sd_bus_error* error)
{
    auto o = static_cast<Hello*>(context);

    try
    {
        return sdbusplus::sdbuspp::property_callback(
                reply, o->get_bus().getInterface(), error,
                std::function(
                    [=]()
                    {
                        return o->enabled();
                    }
                ));
    }
    catch (const std::exception&)
    {
        o->get_bus().set_current_exception(std::current_exception());
        return 1;
    }
}

auto Hello::enabled(bool value,
                                         bool skipSignal) ->
        bool
{
    if (_enabled != value)
    {
        _enabled = value;
        if (!skipSignal)
        {
            _xyz_openbmc_project_example_simple_Hello_interface.property_changed("Enabled");
        }
    }

    return _enabled;
}

auto Hello::enabled(bool val) ->
        bool
{
    return enabled(val, false);
}

int Hello::_callback_set_Enabled(
        sd_bus* /*bus*/, const char* /*path*/, const char* /*interface*/,
        const char* /*property*/, sd_bus_message* value, void* context,
        sd_bus_error* error)
{
    auto o = static_cast<Hello*>(context);

    try
    {
        return sdbusplus::sdbuspp::property_callback(
                value, o->get_bus().getInterface(), error,
                std::function(
                    [=](bool&& arg)
                    {
                        o->enabled(std::move(arg));
                    }
                ));
    }
    catch (const std::exception&)
    {
        o->get_bus().set_current_exception(std::current_exception());
        return 1;
    }
}

namespace details
{
namespace Hello
{
static const auto _property_Enabled =
    utility::tuple_to_array(message::types::type_id<
            bool>());
}
}


void Hello::setPropertyByName(const std::string& _name,
                                     const PropertiesVariant& val,
                                     bool skipSignal)
{
    if (_name == "Enabled")
    {
        auto& v = std::get<bool>(val);
        enabled(v, skipSignal);
        return;
    }
}

auto Hello::getPropertyByName(const std::string& _name) ->
        PropertiesVariant
{
    if (_name == "Enabled")
    {
        return enabled();
    }

    return PropertiesVariant();
}



const vtable_t Hello::_vtable[] = {
    vtable::start(),

    vtable::method("Ping",
                   details::Hello::_param_Ping.data(),
                   details::Hello::_return_Ping.data(),
                   _callback_Ping),

    vtable::property("Enabled",
                     details::Hello::_property_Enabled.data(),
                     _callback_get_Enabled,
                     _callback_set_Enabled,
                     vtable::property_::emits_change),

    vtable::end()
};

} // namespace sdbusplus::server::xyz::openbmc_project::example::simple

