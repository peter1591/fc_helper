import {Component} from '@angular/core';
import {FormControl} from '@angular/forms';
import {GrpcStatusEvent} from '@ngx-grpc/common';

import {RunRequest, RunResponse, State} from '../../build/service.pb'
import {AIServiceClient} from '../../build/service.pbsc'

const UNIT_K = 1e3;
const UNIT_M = 1e6;
const UNIT_B = 1e9;
const UNIT_T = 1e12;
const UNIT_aa = 1e15;
const UNIT_bb = 1e18;
const UNIT_cc = 1e21;

function parseNumber(s: string|null): number {
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

@Component({
  selector : 'app-root',
  templateUrl : './app.component.html',
  styleUrls : [ './app.component.css' ]
})
export class AppComponent {
  title = 'web';

  unitPrice = new FormControl<string>("");
  incomeUnits = new FormControl<string>("");
  incomeInterval = new FormControl<string>("");
  upgradeAmount_cost = new FormControl<string>("");
  upgradeAmount_oneTimeCost = new FormControl<string>("");
  upgradeAmountMultiplyUpgrades = new FormControl<string>("");
  upgradeAmountMultiplyMultiply = new FormControl<string>("");
  upgradeTimeCost = new FormControl<string>("");
  upgradeTimeIncomeShorten = new FormControl<string>("");

  msgs: string[] = [];
  bestActions: RunResponse.BestStrategy.Action[] = [];

  constructor(
      private client: AIServiceClient,
  ) {
    const UNIT_PRICE = 511.12 * UNIT_bb / 7250;
    this.unitPrice.setValue(String(UNIT_PRICE));
    this.incomeUnits.setValue("6194");
    this.incomeInterval.setValue("27.3");
    this.upgradeAmount_cost.setValue("7.66c");
    this.upgradeAmount_oneTimeCost.setValue("0");
    this.upgradeAmountMultiplyUpgrades.setValue("9");
    this.upgradeAmountMultiplyMultiply.setValue("9");
    this.upgradeTimeCost.setValue("8.3c");
    this.upgradeTimeIncomeShorten.setValue("0.6");
  }

  private buildState() {
    var state = new State();
    state.currentAmount = 0;

    state.targetAmount = 0;
    state.elapsedTime = 0;

    const UNIT_PRICE = parseNumber(this.unitPrice.value);

    state.income = new State.Income();
    state.income.amount = parseNumber(this.incomeUnits.value) * UNIT_PRICE;
    state.income.interval = parseNumber(this.incomeInterval.value);
    state.income.otherIncomePerSec = 0;

    state.upgradeAmount = new State.UpgradeAmount();
    state.upgradeAmount.cost = parseNumber(this.upgradeAmount_cost.value);
    state.upgradeAmount.nextCostMultipler = 1.08;
    state.upgradeAmount.onetimeCost =
        parseNumber(this.upgradeAmount_oneTimeCost.value);
    state.upgradeAmount.multiply = new State.UpgradeAmount.MultiplyAmount();
    state.upgradeAmount.multiply.upgrades =
        parseNumber(this.upgradeAmountMultiplyUpgrades.value);
    state.upgradeAmount.multiply.multiply =
        parseNumber(this.upgradeAmountMultiplyMultiply.value);
    state.upgradeAmount.availables = state.upgradeAmount.multiply.upgrades + 1;

    state.upgradeTime = new State.UpgradeTime();
    state.upgradeTime.cost = parseNumber(this.upgradeTimeCost.value);
    state.upgradeTime.incomeShorten =
        parseNumber(this.upgradeTimeIncomeShorten.value);
    state.upgradeTime.nextCostMultipler = 1.08;
    state.upgradeTime.availables = 10;

    return state;
  }

  sendRequest() {
    const request = new RunRequest();
    request.state = this.buildState();
    request.randSeed = 42;
    request.iterationReportInterval = 1000000;

    this.postMessage('sending request: ' + JSON.stringify(request.toJSON()));

    this.client.run(request).subscribe(
        response => this.processResponse(response),
        error => this.processError(error),
        () => { this.postMessage('rpc completed'); })
  }

  private processResponse(response: RunResponse) {
    this.postMessage('got response: ' + JSON.stringify(response.toJSON()));

    if (response.bestStrategy) {
      this.bestActions = response.bestStrategy.actions ??
                         [ RunResponse.BestStrategy.Action.ACTION_UNSET ];
    }
  }

  private processError(error: GrpcStatusEvent) {
    this.postMessage('got error: ' + error.statusMessage);
  }

  private postMessage(msg: string) { this.msgs.push(msg); }
}
