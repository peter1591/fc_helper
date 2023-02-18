#pragma once

#include <array>
#include <memory>
#include <random>
#include <variant>

#include "action.h"
#include "engine.h"
#include "state.h"

struct Node {
  static constexpr size_t MAX_CHILDREN = (int)Action::MAX_ACTION;

  // Instantiated if traversed; nullptr otherwise.
  using ValidChild = std::unique_ptr<Node>;
  struct InvalidChild {}; // if action couldn't be applied
  using Child = std::variant<ValidChild, InvalidChild>;

  std::array<Child, MAX_CHILDREN> children;
};

namespace tree_details {

bool childExplored(const Node::Child &child) {
  if (std::holds_alternative<Node::InvalidChild>(child)) {
    return true;
  }
  return std::get<Node::ValidChild>(child) != nullptr;
}

// Find an unexplored action, and perform that action.
//
// Returns nullptr if all actions are explored, or a (new) invalid action is
//   explored.
// Returns a pointer to the new node if explored.
std::pair<Action, Node *> explore(State &state, Node &node) {
  for (size_t i = 0; i < Node::MAX_CHILDREN; ++i) {
    const Action action = (Action)i;
    auto &child = node.children[i];

    if (childExplored(child)) {
      // explored before
      continue;
    }

    const bool result = performAction(state, action);
    if (result) {
      child = std::make_unique<Node>();
      return {action, std::get<Node::ValidChild>(child).get()};
    } else {
      // this is an invalid action, mark it as so.
      //
      // we can't try next action, because `state` is now corrupted.
      child = Node::InvalidChild{};
      return {action, nullptr};
    }
  }

  return {Action::MAX_ACTION, nullptr};
}

} // namespace tree_details

struct TraverseResult {
  bool explored_new_node = false;
  bool state_corrupted = false;
};

// Traverse the tree and find an unexplored action, and explore it.
//
// Returns false iff an invalid action is applied and state is now corrupted.
TraverseResult traverse(State &state, Node &root, std::mt19937 &rnd,
                        std::vector<Action> &history, int max_depth) {
  Node *node = &root;
  while (true) {
    const auto [action, new_node] = tree_details::explore(state, *node);
    if (new_node) {
      // Found an unexplored action, and applied it.
      history.push_back(action);
      return {
          .explored_new_node = true,
          .state_corrupted = false,
      };
    }

    assert(new_node == nullptr);
    if (action != Action::MAX_ACTION) {
      // Unfortunately an invalid action was found. State is corrupted and we
      // need to exit.
      return {
          .explored_new_node = false,
          .state_corrupted = true,
      };
    }

    if (max_depth == 0) {
      performAction(state, Action::END);
      history.push_back(Action::END);
      return {
          .explored_new_node = false,
          .state_corrupted = false,
      };
    }

    // If not, randomly choose an (explored) action.
    const int i = rnd() % (int)Action::MAX_NON_END_ACTION;
    const auto &child = node->children[i];
    if (std::holds_alternative<Node::InvalidChild>(child)) {
      // unfortunately randomly chose an invalid action; exit
      return {
          .explored_new_node = false,
          .state_corrupted = true,
      };
    }
    performAction(state, (Action)i);
    history.push_back((Action)i);
    node = std::get<Node::ValidChild>(child).get();
    assert(node != nullptr);

    max_depth--;
  }
}
