#pragma once
#include <sdbusplus/async/client.hpp>
#include <sdbusplus/async/execution.hpp>
#include <type_traits>
#include <variant>

#include <xyz/openbmc_project/Example/Simple/Hello/common.hpp>

namespace sdbusplus::client::xyz::openbmc_project::example::simple
{

namespace details
{
// forward declaration
template <typename Client, typename Proxy>
class Hello;
} // namespace details

/** Alias class so we can use the client in both a client_t aggregation
 *  and individually.
 *
 *  sdbusplus::async::client_t<Hello>() or
 *  Hello() both construct an equivalent instance.
 */
template <typename Client = void, typename Proxy = void>
struct Hello :
    public std::conditional_t<std::is_void_v<Client>,
                              sdbusplus::async::client_t<details::Hello>,
                              details::Hello<Client, Proxy>>
{
    template <typename... Args>
    Hello(Args&&... args) :
        std::conditional_t<std::is_void_v<Client>,
                           sdbusplus::async::client_t<details::Hello>,
                           details::Hello<Client, Proxy>>(
            std::forward<Args>(args)...)
    {}
};

namespace details
{

template <typename Client, typename Proxy>
class Hello :
    public sdbusplus::common::xyz::openbmc_project::example::simple::Hello,
    private sdbusplus::async::client::details::client_context_friend
{
  public:
    friend Client;
    template <typename, typename>
    friend struct sdbusplus::client::xyz::openbmc_project::example::simple::Hello;

    // Delete default constructor as these should only be constructed
    // indirectly through sdbusplus::async::client_t.
    Hello() = delete;

    /** @brief Ping
     *  Return the supplied message.
     *
     *  @param[in] message - Message to return.
     *
     *  @return reply[std::string] - Returned message.
     */
    auto ping(std::string message)
    {
        return proxy.template call<std::string>(context(), "Ping", message);
    }

    /** Get value of Enabled
     *  Whether this example object is enabled.
     */
    auto enabled()
    {
        return proxy.template get_property<bool>(context(), "Enabled");
    }

    /** Set value of Enabled
     *  Whether this example object is enabled.
     */
    auto enabled(auto value)
    {
        return proxy.template set_property<bool>(
            context(), "Enabled", std::forward<decltype(value)>(value));
    }


    auto properties()
    {
        return proxy.template get_all_properties<PropertiesVariant>(context()) |
               sdbusplus::async::execution::then([](auto&& v) {
                   properties_t result;
                   for (const auto& [property, value] : v)
                   {
                       std::visit(
                           [&](auto v) {
                               if (property == "Enabled")
                               {
                                   if constexpr (std::is_same_v<
                                                     std::decay_t<decltype(v)>,
                                                     bool>)
                                   {
                                       result.enabled = v;
                                       return;
                                   }
                                   else
                                   {
                                       throw exception::UnpackPropertyError(
                                           property,
                                           UnpackErrorReason::wrongType);
                                   }
                               }
                           },
                           value);
                   }
                   return result;
               });
    }

  private:
    // Conversion constructor from proxy used by client_t.
    explicit constexpr Hello(Proxy p) :
        proxy(p.interface(interface))
    {}

    sdbusplus::async::context& context()
    {
        return sdbusplus::async::client::details::client_context_friend::
            context<Client, Hello>(this);
    }

    decltype(std::declval<Proxy>().interface(interface)) proxy = {};
};

} // namespace details

} // namespace sdbusplus::client::xyz::openbmc_project::example::simple

