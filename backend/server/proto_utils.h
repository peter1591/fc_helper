#include "ai/ai.h"
#include "ai/state.h"
#include "proto/service.pb.h"

void toProtoStateIncome(const State::Income &in, wiring::Building::Income &out) {
  out.set_amount(in.amount);
  out.set_interval(in.interval);
  out.set_other_income_per_sec(in.other_income_per_sec);
}

void fromProtoStateIncome(const wiring::Building::Income &in, State::Income &out) {
  out.amount = in.amount() * in.unit();
  out.interval = in.interval();
  out.other_income_per_sec = in.other_income_per_sec();
}

void toProtoStateUpgradeAmount(const State::UpgradeAmount &in,
                               wiring::Building::UpgradeAmount &out) {
  out.set_cost(in.cost);
  out.set_income_increase(in.income_increase);
  out.set_next_cost_multipler(in.next_cost_multipler);
  out.set_onetime_cost(in.onetime_cost);

  [](const State::UpgradeAmount::MultiplyAmount &in,
     wiring::Building::UpgradeAmount::MultiplyAmount &out) {
    out.set_upgrades(in.upgrades);
    out.set_multiply(in.multiply);
  }(in.multiply, *out.mutable_multiply());

  out.set_availables(in.availables);
}

void fromProtoStateUpgradeAmount(const wiring::Building::UpgradeAmount &in,
                                 State::UpgradeAmount &out) {
  out.cost = in.cost();
  out.income_increase = in.income_increase();
  out.next_cost_multipler = in.next_cost_multipler();
  out.onetime_cost = in.onetime_cost();

  [](const wiring::Building_UpgradeAmount::MultiplyAmount &in,
     State::UpgradeAmount::MultiplyAmount &out) {
    out.upgrades = in.upgrades();
    out.multiply = in.multiply();
  }(in.multiply(), out.multiply);

  out.availables = in.availables();
}

void toProtoStateUpgradeTime(const State::UpgradeTime &in,
                             wiring::Building::UpgradeTime &out) {
  out.set_cost(in.cost);
  out.set_income_shorten(in.income_shorten);
  out.set_next_cost_multipler(in.next_cost_multipler);
  out.set_availables(in.availables);
}

void fromProtoStateUpgradeTime(const wiring::Building::UpgradeTime &in,
                               State::UpgradeTime &out) {
  out.cost = in.cost();
  out.income_shorten = in.income_shorten();
  out.next_cost_multipler = in.next_cost_multipler();
  out.availables = in.availables();
}

void toProtoBuilding(const State &in, wiring::Building &out) {
  out.set_current_amount(in.current_amount);
  out.set_target_amount(in.target_amount);
  out.set_elapsed_time(in.elapsed_time);

  toProtoStateIncome(in.income, *out.mutable_income());
  toProtoStateUpgradeAmount(in.upgrade_amount, *out.mutable_upgrade_amount());
  toProtoStateUpgradeTime(in.upgrade_time, *out.mutable_upgrade_time());
}

void fromProtoBuilding(const wiring::Building &in, State &out) {
  out.current_amount = in.current_amount();
  out.target_amount = in.target_amount();
  out.elapsed_time = in.elapsed_time();

  fromProtoStateIncome(in.income(), out.income);
  fromProtoStateUpgradeAmount(in.upgrade_amount(), out.upgrade_amount);
  fromProtoStateUpgradeTime(in.upgrade_time(), out.upgrade_time);
}

const wiring::Building* getProtoBuilding(const wiring::State& s, const std::string_view buildingName) {
	for (const auto& b : s.buildings()) {
		if (b.name() == buildingName) {
			return &b;
		}
	}
	return nullptr;
}

AIRequest toAiRequest(const wiring::RunRequest &in) {
  AIRequest out;

	bool found = false;
	for (const auto& building : in.state().buildings()) {
		if (building.name() != in.objective().building_name()) {
			continue;
		}
		if (found) {
			throw std::runtime_error("more than two buildings with the same name");
		}

		const auto* targetBuilding = getProtoBuilding(in.state(), in.objective().building_name());
		if (targetBuilding == nullptr) {
			throw std::runtime_error("building in the objective is not found");
		}
		fromProtoBuilding(*targetBuilding, out.state);
		found = true;
	}
	if (!found) {
		throw std::runtime_error("couldn't find building with objective name");
	}

  out.rand_seed = in.rand_seed();
  out.iteration_report_interval = in.iteration_report_interval();
  return out;
}

wiring::RunResponse::BestStrategy::Action toProtoAction(Action in) {
  switch (in) {
  case Action::UPGRADE_AMOUNT:
    return wiring::RunResponse::BestStrategy::UPGRADE_AMOUNT;
  case Action::UPGRADE_TIME:
    return wiring::RunResponse::BestStrategy::UPGRADE_TIME;
  case Action::END:
    return wiring::RunResponse::BestStrategy::END;
  default:
    return wiring::RunResponse::BestStrategy::ACTION_UNSET;
  }
}

wiring::RunResponse fromAiResult(const AIResult &in) {
  wiring::RunResponse out;
  if (std::holds_alternative<BestStrategy>(in)) {
    [](const BestStrategy &in, wiring::RunResponse::BestStrategy &out) {
      for (const Action action : in.actions) {
        out.mutable_actions()->Add(toProtoAction(action));
      }
			toProtoBuilding(in.state, *out.mutable_building());
    }(std::get<BestStrategy>(in), *out.mutable_best_strategy());
  } else if (std::holds_alternative<AIProgressInfo>(in)) {
    [](const AIProgressInfo &in, wiring::RunResponse::ProgressInfo &out) {
      out.set_iteration(in.iteration);
      out.set_new_node_count(in.new_node_count);
      out.set_new_node_count_max(in.new_node_count_max);
    }(std::get<AIProgressInfo>(in), *out.mutable_progress_info());
  }
  return out;
}
