import {Component} from '@angular/core';

import {State} from '../../../build/service.pb';
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

    var state = new State();
    state.income = new State.Income();
    state.income.amount = 6194 * UNIT_PRICE;
    state.income.interval = 27.3;

    state.upgradeAmount = new State.UpgradeAmount();
    state.upgradeAmount.cost = 7.66 * UNIT_cc;
    state.upgradeAmount.onetimeCost = 0;
    state.upgradeAmount.multiply = new State.UpgradeAmount.MultiplyAmount();
    state.upgradeAmount.multiply.upgrades = 9;
    state.upgradeAmount.multiply.multiply = 1.2;

    state.upgradeTime = new State.UpgradeTime();
    state.upgradeTime.cost = 8.3 * UNIT_cc;
    state.upgradeTime.incomeShorten = 0.6;

    // This assumes the gamestate component is already initialized, so this
    // initial state will be reflected on UI. Thus this is only called in
    // `ngAfterViewInit`.
    this.stateService.setState(state);
  }
}
