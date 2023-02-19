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

struct ObjectiveBase {
	ObjectiveBase() { best.state.elapsed_time = INT_MAX; }

	BestStrategy best;
};

struct ObjectiveFirstReachTarget : ObjectiveBase {
  // Return true iff this is the current best.
  bool check(const State &state, const std::vector<Action> &history) {
    if (state.current_amount < state.target_amount) {
      return false;
    }

    if (state.elapsed_time >= best.state.elapsed_time) {
      return false;
    }

		best.actions = history;
		best.state = state;
		return true;
  }
};

struct ObjectiveFirstFinishAllUpgrades : ObjectiveBase {
  bool check(const State &state, const std::vector<Action> &history) {
    if (state.upgrade_amount.availables > 0) {
      return false;
    }
    if (state.upgrade_time.availables > 0) {
      return false;
    }

    if (state.elapsed_time >= best.state.elapsed_time) {
      return false;
    }

		best.state = state;
		best.actions = history;
    return true;
  }
};
