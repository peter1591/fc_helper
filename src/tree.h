#pragma once

#include <array>
#include <memory>
#include <variant>
#include <random>

#include "action.h"
#include "state.h"
#include "engine.h"

struct Node {
	static constexpr size_t MAX_CHILDREN = (int)Action::MAX_ACTION;

	// Instantiated if traversed; nullptr otherwise.
	using ValidChild = std::unique_ptr<Node>;
	struct InvalidChild {};  // if action couldn't be applied
	using Child = std::variant<ValidChild, InvalidChild>;

	std::array<Child, MAX_CHILDREN> children;
};

namespace tree_details {

bool childExplored(const Node::Child& child) {
	if (std::holds_alternative<Node::InvalidChild>(child)) {
		return false;
	}
	return std::get<Node::ValidChild>(child) != nullptr;
}

}

// Find an unexplored action, and perform that action.
//
// Returns nullptr if all actions are explored, or a (new) invalid action is
//   explored.
// Returns a pointer to the new node if explored.
std::pair<Action, Node*> explore(State& state, Node& node) {
	for (size_t i=0; i<Node::MAX_CHILDREN; ++i) {
		const Action action = (Action)i;
		auto& child = node.children[i];

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

// Traverse the tree and find an unexplored action, and explore it.
Action traverse(State& state, Node& root, std::mt19937& rnd, std::vector<Action>& history, int max_depth) {
	Node* node = &root;
	while (true) {
		// If there's an unexplored action, explore it.
		const auto [action, new_node] = explore(state, *node);
		if (new_node) {
			return action;
		}
		if (action != Action::MAX_ACTION) {
			// An invalid action was applied. state is corrupted. exit
			return Action::MAX_ACTION;
		}

		if (max_depth == 0) {
			performAction(state, Action::END);
			return Action::END;
		}

		// If not, randomly choose an (explored) action.
		const int i = rnd() % (int)Action::MAX_NON_END_ACTION;
		const auto& child = node->children[i];
		if (std::holds_alternative<Node::InvalidChild>(child)) {
			// unfortunately chose an invalid action; exit
			return Action::MAX_ACTION;
		}
		performAction(state, (Action)i);
		history.push_back((Action)i);
		node = std::get<Node::ValidChild>(child).get();
		assert(node != nullptr);

		max_depth--;
	}
}
