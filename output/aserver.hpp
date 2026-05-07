#pragma once
#include <sdbusplus/async/server.hpp>
#include <sdbusplus/server/interface.hpp>
#include <sdbusplus/server/transaction.hpp>

#include <type_traits>

#include <xyz/openbmc_project/Example/Simple/Hello/common.hpp>

namespace sdbusplus::aserver::xyz::openbmc_project::example::simple
{

namespace details
{
// forward declaration
template <typename Instance, typename Server>
class Hello;
} // namespace details

template <typename Instance, typename Server = void>
struct Hello :
    public std::conditional_t<
        std::is_void_v<Server>,
        sdbusplus::async::server_t<Instance, details::Hello>,
        details::Hello<Instance, Server>>
{
    template <typename... Args>
    Hello(Args&&... args) :
        std::conditional_t<
            std::is_void_v<Server>,
            sdbusplus::async::server_t<Instance, details::Hello>,
            details::Hello<Instance, Server>>(std::forward<Args>(args)...)
    {}
};

namespace details
{

namespace server_details = sdbusplus::async::server::details;

template <typename Instance, typename Server>
class Hello :
    public sdbusplus::common::xyz::openbmc_project::example::simple::Hello,
    protected server_details::server_context_friend
{
  public:
    explicit Hello(const char* path) :
        _xyz_openbmc_project_example_simple_Hello_interface(
            _context(), path, interface, _vtable, this)
    {}
    explicit Hello(const sdbusplus::message::object_path& path) :
        _xyz_openbmc_project_example_simple_Hello_interface(
            _context(), path, interface, _vtable, this)
    {}

    Hello(
            const char* path,
            [[maybe_unused]] Hello::properties_t props)
        : Hello(path)
    {
        enabled_ = props.enabled;
    }

    Hello(
        const sdbusplus::message::object_path& path,
        [[maybe_unused]] Hello::properties_t props) :
        Hello(path.str.c_str(), props)
    {}


    /** @brief Emit interface added */
    void emit_added()
    {
        _xyz_openbmc_project_example_simple_Hello_interface.emit_added();
    }

    /** @brief Emit interface removed */
    void emit_removed()
    {
        _xyz_openbmc_project_example_simple_Hello_interface.emit_removed();
    }

    /* Property access tags. */
    struct enabled_t
    {
        using value_type = bool;
        enabled_t() = default;
        explicit enabled_t(value_type) {}
    };

    /* Method tags. */
    struct ping_t
    {
        using value_types = std::tuple<std::string>;
        using return_type = std::string;
    };

    auto enabled() const
        requires server_details::has_get_property_nomsg<enabled_t, Instance>
    {
        return static_cast<const Instance*>(this)->get_property(enabled_t{});
    }
    auto enabled(sdbusplus::message_t& m) const
        requires server_details::has_get_property_msg<enabled_t, Instance>
    {
        return static_cast<const Instance*>(this)->get_property(enabled_t{}, m);
    }
    auto enabled() const noexcept
        requires (!server_details::has_get_property<enabled_t, Instance>)
    {
        static_assert(
            !server_details::has_get_property_missing_const<enabled_t,
                                                            Instance>,
            "Missing const on get_property(enabled_t)?");
        return enabled_;
    }

    template <bool EmitSignal = true, typename Arg = bool>
    void enabled(Arg&& new_value)
        requires server_details::has_set_property_nomsg<enabled_t, Instance,
                                                        bool>
    {
        bool changed = static_cast<Instance*>(this)->set_property(
            enabled_t{}, std::forward<Arg>(new_value));

        if (changed && EmitSignal)
        {
            _xyz_openbmc_project_example_simple_Hello_interface.property_changed("Enabled");
        }
    }

    template <bool EmitSignal = true, typename Arg = bool>
    void enabled(sdbusplus::message_t& m, Arg&& new_value)
        requires server_details::has_set_property_msg<enabled_t, Instance,
                                                      bool>
    {
        bool changed = static_cast<Instance*>(this)->set_property(
            enabled_t{}, m, std::forward<Arg>(new_value));

        if (changed && EmitSignal)
        {
            _xyz_openbmc_project_example_simple_Hello_interface.property_changed("Enabled");
        }
    }

    template <bool EmitSignal = true, typename Arg = bool>
    void enabled(Arg&& new_value)
        requires (!server_details::has_set_property<enabled_t, Instance,
                                                    bool>)
    {
        static_assert(
            !server_details::has_get_property<enabled_t, Instance>,
            "Cannot create default set-property for 'enabled_t' with get-property overload.");

        bool changed = (new_value != enabled_);
        enabled_ = std::forward<Arg>(new_value);

        if (changed && EmitSignal)
        {
            _xyz_openbmc_project_example_simple_Hello_interface.property_changed("Enabled");
        }
    }


  protected:
    bool enabled_ = false;

  private:
    /** @return the async context */
    sdbusplus::async::context& _context()
    {
        return server_details::server_context_friend::
            context<Server, Hello>(this);
    }

    sdbusplus::server::interface_t
        _xyz_openbmc_project_example_simple_Hello_interface;

    static constexpr auto _property_typeid_enabled =
        utility::tuple_to_array(message::types::type_id<bool>());
    static constexpr auto _method_typeid_p_ping =
        utility::tuple_to_array(message::types::type_id<std::string>());

    static constexpr auto _method_typeid_r_ping =
        utility::tuple_to_array(message::types::type_id<std::string>());

    static int _callback_get_enabled(
        sd_bus*, const char*, const char*, const char*,
        sd_bus_message* reply, void* context,
        sd_bus_error* error [[maybe_unused]])
    {
        auto self = static_cast<Hello*>(context);

        try
        {
            auto m = sdbusplus::message_t{reply};

            // Set up the transaction.
            sdbusplus::server::transaction::set_id(m);

            // Get property value and add to message.
            if constexpr (server_details::has_get_property_msg<enabled_t,
                                                               Instance>)
            {
                auto v = self->enabled(m);
                static_assert(std::is_convertible_v<decltype(v), bool>,
                              "Property doesn't convert to 'bool'.");
                m.append<bool>(std::move(v));
            }
            else
            {
                auto v = self->enabled();
                static_assert(std::is_convertible_v<decltype(v), bool>,
                              "Property doesn't convert to 'bool'.");
                m.append<bool>(std::move(v));
            }
        }
        catch (const std::exception&)
        {
            self->_context().get_bus().set_current_exception(
                std::current_exception());
            return -EINVAL;
        }

        return 1;
    }

    static int _callback_set_enabled(
        sd_bus*, const char*, const char*, const char*,
        sd_bus_message* value[[maybe_unused]], void* context [[maybe_unused]],
        sd_bus_error* error [[maybe_unused]])
    {
        auto self = static_cast<Hello*>(context);

        try
        {
            auto m = sdbusplus::message_t{value};

            // Set up the transaction.
            sdbusplus::server::transaction::set_id(m);

            auto new_value = m.unpack<bool>();

            // Get property value and add to message.
            if constexpr (server_details::has_set_property_msg<
                              enabled_t, Instance, bool>)
            {
                self->enabled(m, std::move(new_value));
            }
            else
            {
                self->enabled(std::move(new_value));
            }
        }
        catch (const std::exception&)
        {
            self->_context().get_bus().set_current_exception(
                std::current_exception());
            return -EINVAL;
        }

        return 1;
    }


    static int _callback_m_ping(sd_bus_message* msg, void* context,
                                     sd_bus_error* error [[maybe_unused]])
        requires (server_details::has_method<
                            ping_t, Instance, std::string>)
    {
        auto self = static_cast<Hello*>(context);
        auto self_i = static_cast<Instance*>(self);

        try
        {
            auto m = sdbusplus::message_t{msg};
            auto message = m.unpack<std::string>();

            constexpr auto has_method_msg =
                server_details::has_method_msg<
                    ping_t, Instance, std::string>;

            if constexpr (has_method_msg)
            {
                constexpr auto is_async = std::is_same_v<
                    sdbusplus::async::task<std::string>,
                    decltype(self_i->method_call(ping_t{}, m,
                                std::move(message)))>;

                if constexpr (!is_async)
                {
                    auto r = m.new_method_return();
                    r.append(self_i->method_call(ping_t{}, m,
                            std::move(message)));
                    r.method_return();
                }
                else
                {
                    auto fn = [](auto self, auto self_i,
                                 sdbusplus::message_t m,
                                 std::string message)
                            -> sdbusplus::async::task<>
                    {
                        try
                        {

                            auto r = m.new_method_return();
                            r.append(co_await self_i->method_call(
                                ping_t{}, m, std::move(message)));

                            r.method_return();
                            co_return;
                        }
                        catch(const std::exception&)
                        {
                            self->_context().get_bus().set_current_exception(
                                std::current_exception());
                            co_return;
                        }
                    };

                    self->_context().spawn(
                        std::move(fn(self, self_i, m, std::move(message))));
                }
            }
            else
            {
                constexpr auto is_async [[maybe_unused]] = std::is_same_v<
                    sdbusplus::async::task<std::string>,
                    decltype(self_i->method_call(ping_t{},
                                std::move(message)))>;

                if constexpr (!is_async)
                {
                    auto r = m.new_method_return();
                    r.append(self_i->method_call(ping_t{},
                            std::move(message)));
                    r.method_return();
                }
                else
                {
                    auto fn = [](auto self, auto self_i,
                                 sdbusplus::message_t m,
                                 std::string message)
                            -> sdbusplus::async::task<>
                    {
                        try
                        {

                            auto r = m.new_method_return();
                            r.append(co_await self_i->method_call(
                                ping_t{}, std::move(message)));

                            r.method_return();
                            co_return;
                        }
                        catch(const std::exception&)
                        {
                            self->_context().get_bus().set_current_exception(
                                std::current_exception());
                            co_return;
                        }
                    };

                    self->_context().spawn(
                        std::move(fn(self, self_i, m, std::move(message))));
                }
            }
        }
        catch(const std::exception&)
        {
            self->_context().get_bus().set_current_exception(
                std::current_exception());
            return -EINVAL;
        }

        return 1;
    }

    static constexpr sdbusplus::vtable_t _vtable[] = {
        vtable::start(),

        vtable::property("Enabled",
                         _property_typeid_enabled.data(),
                         _callback_get_enabled,
                         _callback_set_enabled,
                         vtable::property_::emits_change),
        vtable::method("Ping",
                       _method_typeid_p_ping.data(),
                       _method_typeid_r_ping.data(),
                       _callback_m_ping),

        vtable::end(),
    };
};

} // namespace details
} // namespace sdbusplus::aserver::xyz::openbmc_project::example::simple

