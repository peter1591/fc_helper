#include <climits>
#include <vector>

#include "action.h"
#include "state.h"

struct BestStrategy {
  State state;
  std::vector<Action> actions;
};

void printBestStrategy(const BestStrategy &v) {
  std::cout << "steps: ";
  for (const auto action : v.actions) {
    std::cout << actionStr(action) << ", ";
  }
  std::cout << std::endl;
  printState(v.state);
  std::cout << std::endl;
}

class ObjectiveFirstReachTarget {
public:
  ObjectiveFirstReachTarget() { state_.elapsed_time = INT_MAX; }

  // Return true iff this is the current best.
  bool check(const State &state, const std::vector<Action> &history) {
    if (state.current_amount < state.target_amount) {
      return false;
    }

    if (state.elapsed_time >= state_.elapsed_time) {
      return false;
    }

    state_ = state;
    history_ = history;
    return true;
  }

  BestStrategy getBest() const {
    return {
        .state = state_,
        .actions = history_,
    };
  }

private:
  State state_;
  std::vector<Action> history_;
};

class ObjectiveFirstFinishAllUpgrades {
public:
  ObjectiveFirstFinishAllUpgrades() { state_.elapsed_time = INT_MAX; }

  bool check(const State &state, const std::vector<Action> &history) {
    if (state.upgrade_amount.availables > 0) {
      return false;
    }
    if (state.upgrade_time.availables > 0) {
      return false;
    }

    if (state.elapsed_time >= state_.elapsed_time) {
      return false;
    }

    state_ = state;
    history_ = history;
    return true;
  }

  BestStrategy getBest() const {
    return {
        .state = state_,
        .actions = history_,
    };
  }

private:
  State state_;
  std::vector<Action> history_;
};
