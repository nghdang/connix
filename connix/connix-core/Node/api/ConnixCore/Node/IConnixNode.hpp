#pragma once

#include <memory>
#include <string>

#include "ConnixCore/Common/IConnixConfiguration.hpp"
#include "ConnixCore/Node/ConnixNodeRole.hpp"
#include "ConnixCore/Node/ConnixTransportType.hpp"

namespace ConnixCore {
namespace Node {

class IConnixNode
{
public:
    virtual ~IConnixNode() = default;

    virtual std::string getName() const = 0;

    virtual ConnixCore::Node::ConnixNodeRole getRole() const = 0;

    virtual ConnixCore::Node::ConnixTransportType getTransportType() const = 0;

    virtual std::shared_ptr<const ConnixCore::Common::IConnixConfiguration>
    getConfiguration() const = 0;
};

} // namespace Node
} // namespace ConnixCore
