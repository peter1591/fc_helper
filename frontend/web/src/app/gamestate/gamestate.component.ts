import {Component} from '@angular/core';
import {FormControl} from '@angular/forms';

import {State, Building} from '../../../build/service.pb'
import {StateService} from '../state.service';

export const UNIT_K = 1e3;
export const UNIT_M = 1e6;
export const UNIT_B = 1e9;
export const UNIT_T = 1e12;
export const UNIT_aa = 1e15;
export const UNIT_bb = 1e18;
export const UNIT_cc = 1e21;

export function parseNumber(s: string|null): number {
  if (s == null) {
    throw new Error("failed to parse: s is null");
  }

  const c = s[s.length - 1];
  var l = s.length;
  var unit = 1;

  switch (c) {
  case 'K': {
    unit = UNIT_K;
    --l;
    break;
  }
  case 'M': {
    unit = UNIT_M;
    --l;
    break;
  }
  case 'B': {
    unit = UNIT_B;
    --l;
    break;
  }
  case 'T': {
    unit = UNIT_T;
    --l;
    break;
  }
  case 'a': {
    unit = UNIT_aa;
    --l;
    break;
  }
  case 'b': {
    unit = UNIT_bb;
    --l;
    break;
  }
  case 'c': {
    unit = UNIT_cc;
    --l;
    break;
  }
  default: {
    unit = 1;
  }
  }
  const v = Number(s.substr(0, l));
  if (isNaN(v)) {
    throw new Error("failed to parse: " + s);
  }

  return v * unit;
}

export function fromNumber(v: number) {
  let formatNumber = (v: number) => v.toFixed(2);

  if (v >= UNIT_cc) {
    return formatNumber(v / UNIT_cc) + "c";
  } else if (v >= UNIT_bb) {
    return formatNumber(v / UNIT_bb) + "b";
  } else if (v >= UNIT_aa) {
    return formatNumber(v / UNIT_aa) + "a";
  } else if (v >= UNIT_T) {
    return formatNumber(v / UNIT_T) + "T";
  } else if (v >= UNIT_B) {
    return formatNumber(v / UNIT_B) + "B";
  } else if (v >= UNIT_M) {
    return formatNumber(v / UNIT_M) + "M";
  } else if (v >= UNIT_K) {
    return formatNumber(v / UNIT_K) + "K";
  }
  return formatNumber(v);
}

@Component({
  selector : 'app-gamestate',
  templateUrl : './gamestate.component.html',
  styleUrls : [ './gamestate.component.css' ]
})
export class GamestateComponent {
  unitPrice = new FormControl<string>("");
  incomeUnits = new FormControl<string>("");
  incomeInterval = new FormControl<string>("");
  upgradeAmount_cost = new FormControl<string>("");
  upgradeAmount_oneTimeCost = new FormControl<string>("");
  upgradeAmountMultiplyUpgrades = new FormControl<string>("");
  upgradeAmountMultiplyMultiply = new FormControl<string>("");
  upgradeTimeCost = new FormControl<string>("");
  upgradeTimeIncomeShorten = new FormControl<string>("");

  constructor(
      private stateService: StateService,
  ) {
    this.stateService.setStateGetter(this.buildState.bind(this));
    this.stateService.stateSet$.subscribe(state => this.setState(state))
  }

  setState(state: State) {
		const building = state.buildings![0];
    this.unitPrice.setValue(fromNumber(this.stateService.UNIT_PRICE));
    this.incomeUnits.setValue(
        fromNumber(building.income!.amount / this.stateService.UNIT_PRICE));
    this.incomeInterval.setValue(fromNumber(building.income!.interval));
    this.upgradeAmount_cost.setValue(fromNumber(building.upgradeAmount!.cost));
    this.upgradeAmount_oneTimeCost.setValue(
        fromNumber(building.upgradeAmount!.onetimeCost));
    this.upgradeAmountMultiplyUpgrades.setValue(
        fromNumber(building.upgradeAmount!.multiply!.upgrades));
    this.upgradeAmountMultiplyMultiply.setValue(
        fromNumber(building.upgradeAmount!.multiply!.multiply));
    this.upgradeTimeCost.setValue(fromNumber(building.upgradeTime!.cost));
    this.upgradeTimeIncomeShorten.setValue(
        fromNumber(building.upgradeTime!.incomeShorten));
  }

  buildState() {
		var building = new Building();
    building.currentAmount = 0;

    building.targetAmount = 0;
    building.elapsedTime = 0;

    const UNIT_PRICE = parseNumber(this.unitPrice.value);

    building.income = new Building.Income();
    building.income.amount = parseNumber(this.incomeUnits.value) * UNIT_PRICE;
    building.income.interval = parseNumber(this.incomeInterval.value);
    building.income.otherIncomePerSec = 0;

    building.upgradeAmount = new Building.UpgradeAmount();
    building.upgradeAmount.cost = parseNumber(this.upgradeAmount_cost.value);
    building.upgradeAmount.nextCostMultipler = 1.08;
    building.upgradeAmount.onetimeCost =
        parseNumber(this.upgradeAmount_oneTimeCost.value);
    building.upgradeAmount.multiply = new Building.UpgradeAmount.MultiplyAmount();
    building.upgradeAmount.multiply.upgrades =
        parseNumber(this.upgradeAmountMultiplyUpgrades.value);
    building.upgradeAmount.multiply.multiply =
        parseNumber(this.upgradeAmountMultiplyMultiply.value);
    building.upgradeAmount.availables = building.upgradeAmount.multiply.upgrades + 1;

    building.upgradeTime = new Building.UpgradeTime();
    building.upgradeTime.cost = parseNumber(this.upgradeTimeCost.value);
    building.upgradeTime.incomeShorten =
        parseNumber(this.upgradeTimeIncomeShorten.value);
    building.upgradeTime.nextCostMultipler = 1.08;
    building.upgradeTime.availables = 10;

    var state = new State();
		state.buildings = [building];
    return state;
  }
}
