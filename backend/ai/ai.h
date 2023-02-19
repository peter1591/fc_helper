#pragma once

#include <cassert>
#include <functional>
#include <variant>
#include <vector>

#include "action.h"
#include "objective.h"
#include "sliding_window.h"
#include "state.h"
#include "tree.h"

struct AIRequest {
  State state;

  int rand_seed = 42;
  int iteration_report_interval = 100000;
};

struct AIProgressInfo {
  int iteration;

  int new_node_count;
  int new_node_count_max;
};

using AIResult = std::variant<BestStrategy, AIProgressInfo>;

class AI {
public:
  using Callback = std::function<void(const AIResult &)>;

  static constexpr int HIT_COUNT_WINDOW_SIZE = 1000000;
  static constexpr int TREE_MAX_DEPTH = 40;
	static constexpr int MAX_ITERATIONS = 10000000;

  void run(const AIRequest &request, Callback callback) {
    assert(request.state.target_amount > 0);
    ObjectiveFirstReachTarget objective;

    //assert(request.state.target_amount == 0);
    //ObjectiveFirstFinishAllUpgrades objective;

    HitCountSlidingWindow<HIT_COUNT_WINDOW_SIZE> new_node_count;

    std::mt19937 rnd;
    rnd.seed(request.rand_seed);

    Node root;
    AIResult result;
    std::vector<Action> history;
    int iteration = 0;
    while (true) {
      ++iteration;

      if (iteration % request.iteration_report_interval == 0) {
        callback(AIProgressInfo{
            .iteration = iteration,
            .new_node_count = new_node_count.getHitCount(),
            .new_node_count_max = new_node_count.size(),
        });
      }
			if (iteration > MAX_ITERATIONS) {
				break;
			}

      history.clear();
      State current_state = request.state;

      const auto ret =
          traverse(current_state, root, rnd, history, TREE_MAX_DEPTH);
      if (ret.state_corrupted) {
        new_node_count.add(false);
        continue; // invalid action found; state corrupted; restart.
      }
      new_node_count.add(ret.explored_new_node);

      if (objective.check(current_state, history)) {
				callback(objective.best);
      }
    }
  }
};
