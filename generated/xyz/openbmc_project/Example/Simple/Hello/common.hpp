#pragma once
#include <algorithm>
#include <array>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>

#include <sdbusplus/exception.hpp>
#include <sdbusplus/message.hpp>
#include <sdbusplus/utility/dedup_variant.hpp>


namespace sdbusplus::common::xyz::openbmc_project::example::simple
{

struct Hello
{
    static constexpr auto interface = "xyz.openbmc_project.Example.Simple.Hello";


    struct properties_t
    {
        bool enabled = false;
    };

    struct property_names
    {
        static constexpr auto enabled = "Enabled";
    };

    using PropertiesVariant = sdbusplus::utility::dedup_variant_t<
        bool>;

    struct method_names
    {
        static constexpr auto ping = "Ping";
    };



};



} // sdbusplus::common::xyz::openbmc_project::example::simple

namespace sdbusplus::message::details
{
} // namespace sdbusplus::message::details

