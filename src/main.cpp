#include <climits>
#include <iostream>
#include <random>

#include "engine.h"
#include "tree.h"

int main(void) {
  State state;

  state.upgrade_amount.next_cost_multipler = 1.08;
  state.upgrade_amount.multiply.multiply = 1.2;

  state.upgrade_time.next_cost_multipler = 1.08;

  state.current_amount = 10000000;
  state.target_amount = 200000000;
  state.income.amount = 179580;
  state.income.interval = 30;
  state.upgrade_amount.cost = 651000;
  state.upgrade_amount.income_increase = 2628;
  state.upgrade_amount.multiply.upgrades = 2;
  state.upgrade_time.cost = 400000;
  state.upgrade_time.income_shorten = 0.2;

  printState(state);

  std::mt19937 rnd;
  rnd.seed(42);

  Node root;

  constexpr int max_depth = 20;
  std::vector<Action> history;

  struct BestInfo {
    int elapsed_time = INT_MAX;
    std::vector<Action> history;
  } best;

  while (true) {
    history.clear();
    State current_state = state;
    const auto last_action =
        traverse(current_state, root, rnd, history, max_depth);

    if (last_action == Action::END) {
      if (current_state.elapsed_time < best.elapsed_time) {
        std::cout << "found better strategy" << std::endl;

        best.elapsed_time = current_state.elapsed_time;
        best.history = history;

        std::cout << "steps: ";
        for (const auto action : best.history) {
          std::cout << actionStr(action) << ", ";
        }
        std::cout << std::endl;
        printState(current_state);
      }
    }
  }

  return 0;
}
