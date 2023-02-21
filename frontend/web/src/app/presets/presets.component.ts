import {Component} from '@angular/core';

import {State, Building} from '../../../build/service.pb';
import {
  UNIT_aa,
  UNIT_B,
  UNIT_bb,
  UNIT_cc,
  UNIT_K,
  UNIT_M,
  UNIT_T,
} from '../gamestate/gamestate.component';
import {StateService} from '../state.service';

@Component({
  selector : 'app-presets',
  templateUrl : './presets.component.html',
  styleUrls : [ './presets.component.css' ]
})
export class PresetsComponent {
  constructor(
      private stateService: StateService,
  ) {}

  ngAfterViewInit() {
    const UNIT_PRICE = 511.12 * UNIT_bb / 7250;
    this.stateService.UNIT_PRICE = UNIT_PRICE;

		var building = new Building();
    building.income = new Building.Income();
    building.income.amount = 6194 * UNIT_PRICE;
    building.income.interval = 27.3;

    building.upgradeAmount = new Building.UpgradeAmount();
    building.upgradeAmount.cost = 7.66 * UNIT_cc;
    building.upgradeAmount.onetimeCost = 0;
    building.upgradeAmount.multiply = new Building.UpgradeAmount.MultiplyAmount();
    building.upgradeAmount.multiply.upgrades = 9;
    building.upgradeAmount.multiply.multiply = 1.2;

    building.upgradeTime = new Building.UpgradeTime();
    building.upgradeTime.cost = 8.3 * UNIT_cc;
    building.upgradeTime.incomeShorten = 0.6;

    var state = new State();
		state.buildings = [building];

    // This assumes the gamestate component is already initialized, so this
    // initial state will be reflected on UI. Thus this is only called in
    // `ngAfterViewInit`.
    this.stateService.setState(state);
  }
}
