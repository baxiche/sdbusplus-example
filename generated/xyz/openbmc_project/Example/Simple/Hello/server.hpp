#pragma once
#include <limits>
#include <map>
#include <sdbusplus/sdbus.hpp>
#include <sdbusplus/server.hpp>
#include <string>
#include <systemd/sd-bus.h>

#include <xyz/openbmc_project/Example/Simple/Hello/common.hpp>

namespace sdbusplus::server::xyz::openbmc_project::example::simple
{

class Hello :
    public sdbusplus::common::xyz::openbmc_project::example::simple::Hello
{
    public:
        /* Define all of the basic class operations:
         *     Not allowed:
         *         - Default constructor to avoid nullptrs.
         *         - Copy operations due to internal unique_ptr.
         *         - Move operations due to 'this' being registered as the
         *           'context' with sdbus.
         *     Allowed:
         *         - Destructor.
         */
        Hello() = delete;
        Hello(const Hello&) = delete;
        Hello& operator=(const Hello&) = delete;
        Hello(Hello&&) = delete;
        Hello& operator=(Hello&&) = delete;
        virtual ~Hello() = default;

        /** @brief Constructor to put object onto bus at a dbus path.
         *  @param[in] bus - Bus to attach to.
         *  @param[in] path - Path to attach at.
         */
        Hello(bus_t& bus, const char* path) :
            _xyz_openbmc_project_example_simple_Hello_interface(
                bus, path, interface, _vtable, this),
            _sdbusplus_bus(bus) {}

        Hello(bus_t& bus, const sdbusplus::message::object_path& path) :
            Hello(bus, path.str.c_str()) {}

        /** @brief Constructor to initialize the object from a map of
         *         properties.
         *
         *  @param[in] bus - Bus to attach to.
         *  @param[in] path - Path to attach at.
         *  @param[in] vals - Map of property name to value for initialization.
         */
        Hello(bus_t& bus, const char* path,
                     const std::map<std::string, PropertiesVariant>& vals,
                     bool skipSignal = false) :
            Hello(bus, path)
        {
            for (const auto& v : vals)
            {
                setPropertyByName(v.first, v.second, skipSignal);
            }
        }

        Hello(bus_t& bus, const sdbusplus::message::object_path& path,
                     const std::map<std::string, PropertiesVariant>& vals,
                     bool skipSignal = false) :
            Hello(bus, path.str.c_str(), vals, skipSignal) {}

        /** @brief Implementation for Ping
         *  Return the supplied message.
         *
         *  @param[in] message - Message to return.
         *
         *  @return reply[std::string] - Returned message.
         */
        virtual std::string ping(
            std::string message) = 0;
        /** Get value of Enabled */
        virtual bool enabled() const;
        /** Set value of Enabled with option to skip sending signal */
        virtual bool enabled(bool value,
               bool skipSignal);
        /** Set value of Enabled */
        virtual bool enabled(bool value);

        /** @brief Sets a property by name.
         *  @param[in] _name - A string representation of the property name.
         *  @param[in] val - A variant containing the value to set.
         */
        void setPropertyByName(const std::string& _name,
                               const PropertiesVariant& val,
                               bool skipSignal = false);

        /** @brief Gets a property by name.
         *  @param[in] _name - A string representation of the property name.
         *  @return - A variant containing the value of the property.
         */
        PropertiesVariant getPropertyByName(const std::string& _name);



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

        /** @return the bus instance */
        bus_t& get_bus()
        {
            return  _sdbusplus_bus;
        }

    private:
        /** @brief sd-bus callback for Ping
         */
        static int _callback_Ping(
            sd_bus_message*, void*, sd_bus_error*);

        /** @brief sd-bus callback for get-property 'Enabled' */
        static int _callback_get_Enabled(
            sd_bus*, const char*, const char*, const char*,
            sd_bus_message*, void*, sd_bus_error*);
        /** @brief sd-bus callback for set-property 'Enabled' */
        static int _callback_set_Enabled(
            sd_bus*, const char*, const char*, const char*,
            sd_bus_message*, void*, sd_bus_error*);

        static const vtable_t _vtable[];
        sdbusplus::server::interface_t
                _xyz_openbmc_project_example_simple_Hello_interface;
        bus_t&  _sdbusplus_bus;

        bool _enabled = false;
};

} // namespace sdbusplus::server::xyz::openbmc_project::example::simple

#ifndef SDBUSPP_REMOVE_DEPRECATED_NAMESPACE
namespace sdbusplus::xyz::openbmc_project::Example::Simple::server {

using sdbusplus::server::xyz::openbmc_project::example::simple::Hello;

} // namespace sdbusplus::xyz::openbmc_project::Example::Simple::server
#endif

