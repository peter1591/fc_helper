#include <climits>
#include <vector>

#include "action.h"
#include "state.h"

class ObjectiveFirstReachTarget {
public:
  // Return true iff this is the current best.
  bool check(const State &state, const std::vector<Action> &history) {
		if (state.current_amount < state.target_amount) {
			return false;
		}

    if (state.elapsed_time >= elapsed_time_) {
      return false;
    }

    elapsed_time_ = state.elapsed_time;
    history_ = history;
    return true;
  }

  void printBest() const {
    std::cout << "elapsed_time: " << elapsed_time_ << std::endl;
    std::cout << "steps: ";
    for (const auto action : history_) {
      std::cout << actionStr(action) << ", ";
    }
    std::cout << std::endl;
    std::cout << std::endl;
  }

private:
  int elapsed_time_ = INT_MAX;
  std::vector<Action> history_;
};

class ObjectiveFirstFinishAllUpgrades {
public:
  bool check(const State &state, const std::vector<Action> &history) {
    if (state.upgrade_amount.availables > 0) {
      return false;
    }
    if (state.upgrade_time.availables > 0) {
      return false;
    }

		if (state.elapsed_time >= elapsed_time_) {
			return false;
		}
		
		elapsed_time_ = state.elapsed_time;
		history_ = history;
		return true;
  }

  void printBest() const {
    std::cout << "elapsed_time: " << elapsed_time_ << std::endl;
    std::cout << "steps: ";
    for (const auto action : history_) {
      std::cout << actionStr(action) << ", ";
    }
    std::cout << std::endl;
    std::cout << std::endl;
  }

private:
	int elapsed_time_ = INT_MAX;
  std::vector<Action> history_;
};
