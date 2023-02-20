import {Component, ViewChild} from '@angular/core';
import {FormControl} from '@angular/forms';
import {GrpcStatusEvent} from '@ngx-grpc/common';

import {RunRequest, RunResponse, State} from '../../build/service.pb'
import {AIServiceClient} from '../../build/service.pbsc';

import {
  GamestateComponent,
  UNIT_aa,
  UNIT_B,
  UNIT_bb,
  UNIT_cc,
  UNIT_K,
  UNIT_M,
  UNIT_T,
} from './gamestate/gamestate.component';

@Component({
  selector : 'app-root',
  templateUrl : './app.component.html',
  styleUrls : [ './app.component.css' ]
})
export class AppComponent {
  title = 'web';

  @ViewChild(GamestateComponent) private gamestate!: GamestateComponent;

  msgs: string[] = [];
  bestActions: RunResponse.BestStrategy.Action[] = [];

  constructor(
      private client: AIServiceClient,
  ) {}

  ngAfterViewInit() {
  const UNIT_PRICE = 511.12 * UNIT_bb / 7250;

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

    this.gamestate.setState(state, UNIT_PRICE);
  }

  sendRequest() {
    const request = new RunRequest();
    request.state = this.gamestate.buildState();
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
