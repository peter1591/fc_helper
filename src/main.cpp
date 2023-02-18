#include <iostream>
#include <random>

#include "engine.h"
#include "objective.h"
#include "sliding_window.h"
#include "tree.h"

long double operator""_K(long double v) { return v * 1e3; }
long double operator""_M(long double v) { return v * 1e6; }
long double operator""_B(long double v) { return v * 1e9; }
long double operator""_T(long double v) { return v * 1e12; }
long double operator""_aa(long double v) { return v * 1e15; }
long double operator""_bb(long double v) { return v * 1e18; }

void setTargetForSeconds(double v, State &state) {
  assert(state.income.amount >= 0); // must be set before
  assert(state.income.interval > 0);
  state.target_amount = state.income.amount * (v / state.income.interval);
}

void setupWeed(State &state) {
  constexpr double UNIT_PRICE = 10.0;

  state.upgrade_amount.next_cost_multipler = 1.08;
  state.upgrade_time.next_cost_multipler = 1.08;

  state.current_amount = 148000;
  state.target_amount = 1000000;
  state.income.amount = 5341 * UNIT_PRICE;
  state.income.interval = 2.6;

  state.upgrade_amount.cost = 43000;
  state.upgrade_amount.income_increase = 63.8 * UNIT_PRICE;
  state.upgrade_amount.multiply.upgrades = 9;
  state.upgrade_amount.multiply.multiply = 1.2;
  state.upgrade_amount.availables = 20;

  state.upgrade_time.cost = 11000;
  state.upgrade_time.income_shorten = 0.02;
  state.upgrade_time.availables = 20;
}

void setupOyster(State &state) {
  const double UNIT_PRICE = 3.0_K;

  state.upgrade_amount.next_cost_multipler = 1.08;
  state.upgrade_time.next_cost_multipler = 1.08;

  state.current_amount = 0;
  state.income.amount = 7816 * UNIT_PRICE;
  state.income.interval = 3.2;
  setTargetForSeconds(3600, state);
  // state.target_amount = 0.3_B;

  state.upgrade_amount.cost = 200.00_M;
  state.upgrade_amount.income_increase = 90.6 * UNIT_PRICE;
  state.upgrade_amount.multiply.upgrades = 7;
  state.upgrade_amount.multiply.multiply = 1.2;
  state.upgrade_amount.availables = 10;

  state.upgrade_time.cost = 188.24_M;
  state.upgrade_time.income_shorten = 0.032;
  state.upgrade_time.availables = 10;
}

void setupCrab(State &state) {
  const double UNIT_PRICE = 3.0_M;

  state.upgrade_amount.next_cost_multipler = 1.08;
  state.upgrade_time.next_cost_multipler = 1.08;

  state.current_amount = 0;
  state.income.amount = 10821 * UNIT_PRICE;
  state.income.interval = 5.1;
  // setTargetForSeconds(3600, state);
  state.target_amount = 10.0_T;

  state.upgrade_amount.cost = 35.29_B;
  state.upgrade_amount.income_increase = 55.0 * UNIT_PRICE;
  state.upgrade_amount.multiply.upgrades = 3;
  state.upgrade_amount.multiply.multiply = 1.2;
  state.upgrade_amount.availables = 10;

  state.upgrade_time.cost = 082.35_B;
  state.upgrade_time.income_shorten = 0.051;
  state.upgrade_time.availables = 10;
}

void setupLobster(State &state) {
  const double UNIT_PRICE = 0.468_B;

  state.upgrade_amount.next_cost_multipler = 1.08;
  state.upgrade_time.next_cost_multipler = 1.08;

  state.current_amount = 0;
  state.income.amount = 21122 * UNIT_PRICE;
  state.income.interval = 6.7;
  // setTargetForSeconds(3600, state);
  state.target_amount = 10.0_aa;

  state.upgrade_amount.cost = 26.92_T;
  state.upgrade_amount.onetime_cost = 1488.89_T;
  state.upgrade_amount.income_increase = 215 * UNIT_PRICE;
  state.upgrade_amount.multiply.upgrades = 9;
  state.upgrade_amount.multiply.multiply = 1.2;
  state.upgrade_amount.availables = state.upgrade_amount.multiply.upgrades + 1;

  state.upgrade_time.cost = 122.2_T;
  state.upgrade_time.income_shorten = 0.060;
  state.upgrade_time.availables = 10;
}

void setupSardin(State &state) {
  const double UNIT_PRICE = 28.1_aa / 146.6_K;

  state.upgrade_amount.next_cost_multipler = 1.08;
  state.upgrade_time.next_cost_multipler = 1.08;

  state.current_amount = 0;
  state.income.amount = 168.53_K * UNIT_PRICE;
  state.income.interval = 9.3;
  // setTargetForSeconds(3600, state);
  state.target_amount = 30.00_bb;

  state.upgrade_amount.cost = 337.50_aa;
  state.upgrade_amount.onetime_cost = 0;
  state.upgrade_amount.income_increase = 1922 * UNIT_PRICE;
  state.upgrade_amount.multiply.upgrades = 5;
  state.upgrade_amount.multiply.multiply = 1.2;
  state.upgrade_amount.availables = state.upgrade_amount.multiply.upgrades + 11;

  state.upgrade_time.cost = 162.50_aa;
  state.upgrade_time.income_shorten = 0.100;
  state.upgrade_time.availables = 10;
}

void setupTuna(State &state) {
  const double UNIT_PRICE = 491.52_aa / 7680;

  state.upgrade_amount.next_cost_multipler = 1.08;
  state.upgrade_time.next_cost_multipler = 1.08;

  state.current_amount = 0;
  state.income.amount = 484 * UNIT_PRICE;
  state.income.interval = 45.2;
  // setTargetForSeconds(3600, state);
  //state.target_amount = 5000.00_bb;

  state.upgrade_amount.cost = 1.120_bb;
  state.upgrade_amount.onetime_cost = 0;
  state.upgrade_amount.income_increase = 21 * UNIT_PRICE;
  state.upgrade_amount.multiply.upgrades = 1;
  state.upgrade_amount.multiply.multiply = 1.1;
  state.upgrade_amount.availables = state.upgrade_amount.multiply.upgrades + 11;

  state.upgrade_time.cost = 518.75_aa;
  state.upgrade_time.income_shorten = 0.9;
  state.upgrade_time.availables = 10;
}

int main(void) {
  State state;

  // setupWeed(state);
  // setupOyster(state);
  // setupCrab(state);
  // setupLobster(state);
  setupSardin(state);
  // setupTuna(state);

  printState(state);

  std::mt19937 rnd;
  rnd.seed(39);

  Node root;

  constexpr int max_depth = 20;
  std::vector<Action> history;

  assert(state.target_amount > 0);
  ObjectiveFirstReachTarget objective;

  //assert(state.target_amount == 0);
  //ObjectiveFirstFinishAllUpgrades objective;

  HitCountSlidingWindow<1000000> new_node_count;

  int iteration = 0;
  while (true) {
    ++iteration;
    if (iteration % 100000 == 0) {
      const int new_node_chance_percent =
          (double)new_node_count.getHitCount() / new_node_count.size() * 100;
      std::cout << "\riteration: " << iteration
				<< ", explored new node count: " << new_node_count.getHitCount()
                << ", explored new node chance: " << new_node_chance_percent
                << "%      ";
			if (new_node_chance_percent == 0) break;
    }

    history.clear();
    State current_state = state;

    const auto ret = traverse(current_state, root, rnd, history, max_depth);
    if (ret.state_corrupted) {
			new_node_count.add(false);
      continue; // invalid action found; state corrupted; restart.
    }
    new_node_count.add(ret.explored_new_node);

    if (objective.check(current_state, history)) {
      std::cout << "found better strategy" << std::endl;
      objective.printBest();
    }
  }

  return 0;
}
