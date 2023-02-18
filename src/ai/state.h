#pragma once

#include <iostream>

struct State {
  double current_amount = 0;

  double target_amount = 0;
  double elapsed_time = 0;

  struct Income {
    double amount = 0;
    double interval = 0;
  } income;

  struct UpgradeAmount {
    double cost = 0;
    double income_increase = 0;

    double next_cost_multipler = 0.0;

		// E.g., upgrade building so we can upgrade
    double onetime_cost = 0;

    struct MultiplyAmount {
      // How many upgrades til amount is multipled
      // 0 means next upgrade will multiply
      // valid: 0~9
      int upgrades = 0;

      double multiply = 0.0;
    } multiply;

		// How many upgrades are still available.
		int availables = 0;
  } upgrade_amount;

  struct UpgradeTime {
    double cost = 0;
    double income_shorten = 0.0;

    double next_cost_multipler = 0.0;

		// How many upgrades are still available.
		int availables = 0;
  } upgrade_time;
};

void printState(const State &state) {
  std::cout << "===== state =====" << std::endl;
  std::cout << "  current amount: " << state.current_amount << std::endl;
	std::cout << "  target amount: " << state.target_amount << std::endl;
  std::cout << "  elapsed time: " << state.elapsed_time << std::endl;
  std::cout << "  income amount: " << state.income.amount << std::endl;
  std::cout << "  income interval: " << state.income.interval << std::endl;
  std::cout << std::endl;
}
