#include <iostream>
#include <random>

#include "ai/ai.h"
#include "ai/numbers.h"

void setTargetForSeconds(double secs, State &state) {
  assert(state.income.amount >= 0); // must be set before
  assert(state.income.interval > 0);
  const double income = state.income.amount / state.income.interval;
  state.target_amount = (income + state.income.other_income_per_sec) * secs;
}

double getIncomePerSec(std::function<void(State &)> stateFunc) {
  State s;
  stateFunc(s);
  return s.income.amount / s.income.interval;
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
  const double UNIT_PRICE = 13.41_aa / 5.52_M;

  state.upgrade_amount.next_cost_multipler = 1.08;
  state.upgrade_time.next_cost_multipler = 1.08;

  state.current_amount = 0;
  state.income.amount = 1.52_M * UNIT_PRICE;
  state.income.interval = 2.9;
  setTargetForSeconds(3600, state);

  state.upgrade_amount.cost = 2.69_bb;
  state.upgrade_amount.onetime_cost = 1488.89_T;
  state.upgrade_amount.income_increase = 215 * UNIT_PRICE;
  state.upgrade_amount.multiply.upgrades = 9;
  state.upgrade_amount.multiply.multiply = 1.2;
  state.upgrade_amount.availables = state.upgrade_amount.multiply.upgrades + 1;

  state.upgrade_time.cost = 50.00_aa;
  state.upgrade_time.income_shorten = 0.030;
  state.upgrade_time.availables = 10;
}

void setupTuna(State &state);
void setupSardin(State &state) {
  const double UNIT_PRICE = 139.97_aa / 432.0_K;

  state.upgrade_amount.next_cost_multipler = 1.08;
  state.upgrade_time.next_cost_multipler = 1.08;

  state.current_amount = 0;
  state.income.amount = 301.98_K * UNIT_PRICE;
  state.income.interval = 7.3;
  //state.income.other_income_per_sec = getIncomePerSec(setupLobster) + getIncomePerSec(setupTuna);
  setTargetForSeconds(3600 * 1, state);
  // state.target_amount = 50.00_bb;

  state.upgrade_amount.cost = 1.25_bb;
  state.upgrade_amount.onetime_cost = 0;
  state.upgrade_amount.income_increase = 3204 * UNIT_PRICE;
  state.upgrade_amount.multiply.upgrades = 9;
  state.upgrade_amount.multiply.multiply = 1.2;
  state.upgrade_amount.availables = state.upgrade_amount.multiply.upgrades + 1;

  state.upgrade_time.cost = 1.12_bb;
  state.upgrade_time.income_shorten = 0.070;
  state.upgrade_time.availables = 10;
}

void setupTuna(State &state) {
  const double UNIT_PRICE = 829.44_aa / 7680;

  state.upgrade_amount.next_cost_multipler = 1.08;
  state.upgrade_time.next_cost_multipler = 1.08;

  state.current_amount = 0;
  state.income.amount = 7520 * UNIT_PRICE;
  state.income.interval = 19.4;
  state.income.other_income_per_sec = getIncomePerSec(setupLobster) + getIncomePerSec(setupSardin);
  setTargetForSeconds(3600 * 20, state);
  // state.target_amount = 5000.00_bb;

  state.upgrade_amount.cost = 8.75_bb;
  state.upgrade_amount.onetime_cost = 262.5_bb;
  state.upgrade_amount.income_increase = 95.1 * UNIT_PRICE;
  state.upgrade_amount.multiply.upgrades = 5;
  state.upgrade_amount.multiply.multiply = 1.2;
  state.upgrade_amount.availables = state.upgrade_amount.multiply.upgrades + 1;

  state.upgrade_time.cost = 150.0_bb;
  state.upgrade_time.income_shorten = 0.4;
  state.upgrade_time.availables = 10;
}

int main(void) {
  State initialState;

  // setupWeed(state);
  // setupOyster(state);
  // setupCrab(state);
  // setupLobster(state);
  //setupSardin(state);
  setupTuna(initialState);

  printState(initialState);

  AI ai;
  ai.run(
      AIRequest{
          .state = initialState,
          .rand_seed = 42,
          .iteration_report_interval = 100000,
      },
      [&](const AIResult &result) {
        if (std::holds_alternative<BestStrategy>(result)) {
          const auto &best = std::get<BestStrategy>(result);
          std::cout << "found better strategy:" << std::endl;
					std::cout << "initial state:" << std::endl;
					printState(initialState);
          printBestStrategy(best);
        }
        if (std::holds_alternative<AIProgressInfo>(result)) {
          const auto &progress = std::get<AIProgressInfo>(result);
          const int new_node_chance_percent = (double)progress.new_node_count /
                                              progress.new_node_count_max * 100;
          std::cout << "\riteration: " << progress.iteration
                    << ", explored new node chance: " << new_node_chance_percent
                    << "%      ";
        }
      });

  return 0;
}
