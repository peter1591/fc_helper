#pragma once

#include <string>

enum class Action {
	UPGRADE_AMOUNT = 0,
	UPGRADE_TIME = 1,
	MAX_NON_END_ACTION = 2,
	
	END = 2,
	MAX_ACTION = 3,
};

std::string actionStr(const Action action) {
	switch (action) {
		case Action::UPGRADE_AMOUNT:
			return "UPGRADE_AMOUNT";
		case Action::UPGRADE_TIME:
			return "UPGRADE_TIME";
		case Action::END:
			return "END";
		default:
			return "<unknown>";
	}
}
