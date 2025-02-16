#pragma once
#include "Vertex.hpp"
#include <iostream>
#include <string>

#include "../Util/Util.hpp"

namespace cse {
  /**
   * A single direction edge
   */
  class Edge : public FileSerializable {
  private:
    std::string id;
    std::shared_ptr<cse::Vertex> from;
    std::shared_ptr<cse::Vertex> to;

  protected:
    std::string GetTypeName() const override { return "EDGE"; }
    std::vector<std::pair<std::string, SerializableProperty>> GetPropertyMap() override {
      std::vector<std::pair<std::string, SerializableProperty>> properties;
      properties.emplace_back("bidirectional", SerializableProperty([this](std::ostream &s) { s << IsBidirectional(); },
                                                                    [](const std::string &) {}));
      properties.emplace_back(
          "from", SerializableProperty([this](std::ostream &s) { s << from->GetId(); }, [](const std::string &) {}));
      properties.emplace_back(
          "to", SerializableProperty([this](std::ostream &s) { s << to->GetId(); }, [](const std::string &) {}));
      return properties;
    }
    void SetId(std::string newId) override { id = newId; };

  public:
    Edge() = delete;
    virtual ~Edge() = default;
    Edge(std::string id, std::shared_ptr<cse::Vertex> &from, std::shared_ptr<cse::Vertex> &to)
        : id(id), from(from), to(to) {};

    virtual bool IsBidirectional() { return false; };
    virtual bool IsConnected(std::shared_ptr<cse::Vertex> const &v1, std::shared_ptr<cse::Vertex> const &v2) {
      return v1 == from && v2 == to;
    };

    std::shared_ptr<cse::Vertex> &GetFrom() { return from; };
    std::shared_ptr<cse::Vertex> &GetTo() { return to; };
    std::string GetId() const override { return id; };
  };

  class BidirectionalEdge : public cse::Edge {
  public:
    BidirectionalEdge() = delete;
    virtual ~BidirectionalEdge() = default;
    BidirectionalEdge(std::string id, std::shared_ptr<cse::Vertex> &from, std::shared_ptr<cse::Vertex> &to)
        : cse::Edge(id, from, to) {};
    // TODO @lspecht: Handle GetFrom and GetTo on bidirectional case
    bool IsBidirectional() override { return true; };
    bool IsConnected(std::shared_ptr<cse::Vertex> const &v1, std::shared_ptr<cse::Vertex> const &v2) override {
      return Edge::IsConnected(v1, v2) || Edge::IsConnected(v2, v1);
    };
  };
} // namespace cse
