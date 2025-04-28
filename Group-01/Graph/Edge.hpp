#pragma once
#include "../Util/Util.hpp"
#include "Vertex.hpp"
#include <iostream>
#include <string>

namespace cse {
  class Graph; // Forward declaration

  class Edge : public FileSerializable {
  private:
    std::string id;
    std::shared_ptr<cse::Vertex> from;
    std::shared_ptr<cse::Vertex> to;

    // Helper for parsing edge properties
    struct EdgeProperties {
      bool bidirectional;
      std::string from_id;
      std::string to_id;
    };

    static EdgeProperties ParseProperties(std::istream &f, size_t indent_level) {
      auto properties = FileUtil::GetProperties(f, indent_level + cse::BASE_INDENTATION);
      assert(properties.size() == 3);
      assert(properties.at(0).first == "bidirectional");
      assert(properties.at(1).first == "from");
      assert(properties.at(2).first == "to");

      return EdgeProperties{std::stoi(properties.at(0).second) != 0, properties.at(1).second, properties.at(2).second};
    }

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
    Edge(const std::string id, std::shared_ptr<cse::Vertex> &from, std::shared_ptr<cse::Vertex> &to)
        : id(id), from(from), to(to) {};

    virtual bool IsBidirectional() { return false; };
    virtual bool IsConnected(std::shared_ptr<cse::Vertex> const &v1, std::shared_ptr<cse::Vertex> const &v2) {
      return v1 == from && v2 == to;
    };

    std::shared_ptr<cse::Vertex> &GetFrom() { return from; };
    std::shared_ptr<cse::Vertex> &GetTo() { return to; };
    std::string GetId() const override { return id; };

    static void CreateFromFile(std::istream &f, size_t indent_level, Graph &graph);
  };

  class BidirectionalEdge : public cse::Edge {
  public:
    BidirectionalEdge() = delete;
    virtual ~BidirectionalEdge() = default;
    BidirectionalEdge(const std::string id, std::shared_ptr<cse::Vertex> &from, std::shared_ptr<cse::Vertex> &to)
        : cse::Edge(id, from, to) {};
    // TODO @lspecht: Handle GetFrom and GetTo on bidirectional case
    bool IsBidirectional() override { return true; };
    bool IsConnected(std::shared_ptr<cse::Vertex> const &v1, std::shared_ptr<cse::Vertex> const &v2) override {
      return Edge::IsConnected(v1, v2) || Edge::IsConnected(v2, v1);
    };
  };
} // namespace cse
