#pragma once
#include <memory>
#include <string>
#include <map>
#include "../Error/Position.h"
#include "../Frontend/Node/NodeType.h"
using namespace Coda::Frontend;
using namespace Coda::Error;

class INode {
public:
    virtual ~INode() = default;

public:
    virtual NodeType getType() const = 0;
    virtual std::string getValue() const = 0;
    virtual std::shared_ptr<INode> getLeft() const = 0;
    virtual std::shared_ptr<INode> getRight() const = 0;
    virtual std::map<std::string, std::shared_ptr<INode>> getProperties() const = 0;
    virtual Position getStartPosition() const = 0;
    virtual Position getEndPosition() const = 0;
};