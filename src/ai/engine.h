#pragma once

#include <cassert>
#include <exception>
#include <iostream>

#include "action.h"
#include "state.h"

namespace engine_detail {
void waitUntil(State &state, const double until) {
  double rest = until - state.current_amount;
  if (rest <= 0) {
    return;
  }

  assert(state.income.amount > 0);
  assert(state.income.interval > 0);

	double income = state.income.amount / state.income.interval;
	income += state.income.other_income_per_sec;
	const double secs = rest / income;

	state.current_amount += income * secs;
	state.elapsed_time += secs;

	if (state.current_amount < until) {
		state.current_amount = until; // round-up in case
	}
  assert(state.current_amount >= until);
}

void performEnd(State &state) {
  engine_detail::waitUntil(state, state.target_amount);
}

bool performUpgradeAmount(State &state) {
	if (state.upgrade_amount.availables <= 0) {
		return false;
	}
	state.upgrade_amount.availables--;

	const double cost = state.upgrade_amount.cost + state.upgrade_amount.onetime_cost;
  assert(state.upgrade_amount.onetime_cost >= 0);
  assert(state.upgrade_amount.cost > 0);
  waitUntil(state, cost);
  state.current_amount -= cost;
	state.upgrade_amount.onetime_cost = 0;
  assert(state.current_amount >= 0);

  state.upgrade_amount.cost =
      state.upgrade_amount.next_cost_multipler * state.upgrade_amount.cost;
  state.income.amount += state.upgrade_amount.income_increase;

  if (state.upgrade_amount.multiply.upgrades <= 0) {
    state.income.amount =
        state.upgrade_amount.multiply.multiply * state.income.amount;
    state.upgrade_amount.multiply.upgrades = 9;
  }
	return true;
};

bool performUpgradeTime(State &state) {
	if (state.upgrade_time.availables <= 0) {
		return false;
	}
	state.upgrade_time.availables--;

  assert(state.upgrade_time.cost > 0);
  waitUntil(state, state.upgrade_time.cost);

  state.current_amount -= state.upgrade_time.cost;
  assert(state.current_amount >= 0);

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
    return engine_detail::performUpgradeAmount(state);

  case Action::UPGRADE_TIME:
    return engine_detail::performUpgradeTime(state);

  case Action::END:
    engine_detail::performEnd(state);
    return true;

  default:
    std::terminate();
  }
}
