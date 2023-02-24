#pragma once

#include <cassert>
#include <chrono>
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

  static constexpr auto REPORT_INTERVAL = std::chrono::milliseconds(100);
  static constexpr auto MAX_RUN_TIME = std::chrono::seconds(30);

  void run(const AIRequest &request, Callback callback) {
    // assert(request.state.target_amount > 0);
    ObjectiveFirstReachTarget objective;

    // assert(request.state.target_amount == 0);
    // ObjectiveFirstFinishAllUpgrades objective;

    HitCountSlidingWindow<HIT_COUNT_WINDOW_SIZE> new_node_count;

    std::mt19937 rnd;
    rnd.seed(request.rand_seed);

    Node root;
    AIResult result;
    std::vector<Action> history;
    int iteration = 0;
    std::chrono::steady_clock clock;
    const auto start = clock.now();
    const auto until = start + MAX_RUN_TIME;
    auto last_report = start;
    while (true) {
      ++iteration;

      const auto now = clock.now();
      if ((now - last_report) > REPORT_INTERVAL) {
        callback(AIProgressInfo{
            .iteration = iteration,
            .new_node_count = new_node_count.getHitCount(),
            .new_node_count_max = new_node_count.size(),
        });
        last_report = now;
      }
      if (now > until) {
        break;
      }
      if (iteration > new_node_count.size() &&
          new_node_count.getHitCount() == 0) {
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
