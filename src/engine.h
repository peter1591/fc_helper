#pragma once

#include <cassert>
#include <exception>
#include <iostream>

#include "action.h"
#include "state.h"

namespace engine_detail {
void waitUntil(State &state, const int until) {
  int rest = until - state.current_amount;
  if (rest <= 0) {
    return;
  }

  assert(state.income.amount > 0);
  assert(state.income.interval > 0);

  // round-up of `rest` / `income.amount`
  const int rounds = 1 + (rest - 1) / state.income.amount;
  state.current_amount += rounds * state.income.amount;
  state.elapsed_time += rounds * state.income.interval;
}

void performEnd(State &state) {
  engine_detail::waitUntil(state, state.target_amount);
}

void performUpgradeAmount(State &state) {
  assert(state.upgrade_amount.cost > 0);
  waitUntil(state, state.upgrade_amount.cost);

  state.current_amount -= state.upgrade_amount.cost;
  assert(state.current_amount > 0);

  state.upgrade_amount.cost =
      state.upgrade_amount.next_cost_multipler * state.upgrade_amount.cost;
  state.income.amount += state.upgrade_amount.income_increase;

  if (state.upgrade_amount.multiply.upgrades <= 0) {
    state.income.amount =
        state.upgrade_amount.multiply.multiply * state.income.amount;
    state.upgrade_amount.multiply.upgrades = 9;
  }
};

bool performUpgradeTime(State &state) {
  assert(state.upgrade_time.cost > 0);
  waitUntil(state, state.upgrade_time.cost);

  state.current_amount -= state.upgrade_time.cost;
  assert(state.current_amount > 0);

  state.income.interval -= state.upgrade_time.income_shorten;
  if (state.income.interval < 0) {
    return false;
  }
  state.upgrade_time.cost =
      state.upgrade_time.next_cost_multipler * state.upgrade_time.cost;
  return true;
}
} // namespace engine_detail

// Return true iff it's a valid action.
bool performAction(State &state, const Action action) {
  switch (action) {
  case Action::UPGRADE_AMOUNT:
    engine_detail::performUpgradeAmount(state);
    return true;

  case Action::UPGRADE_TIME:
    return engine_detail::performUpgradeTime(state);

  case Action::END:
    engine_detail::performEnd(state);
    return true;

  default:
    std::terminate();
  }
}
