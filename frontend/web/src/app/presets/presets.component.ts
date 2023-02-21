import {Component} from '@angular/core';
import {FormControl} from '@angular/forms';

import {
  Building,
  LoadRequest,
  RunRequest,
  SaveRequest,
  State
} from '../../../build/service.pb';
import {AIServiceClient} from '../../../build/service.pbsc';
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
      private client: AIServiceClient,
      private stateService: StateService,
  ) {}

  saveName = new FormControl<string>("default");
  msg = "";

	options = ['asd', 'sdf', 'dfg'];

  ngAfterViewInit() {
    var building = new Building();
    building.income = new Building.Income();
    building.income.amount = 6194;
		building.income.unit = 511.12 * UNIT_bb / 7250;
    building.income.interval = 27.3;

    building.upgradeAmount = new Building.UpgradeAmount();
    building.upgradeAmount.cost = 7.66 * UNIT_cc;
    building.upgradeAmount.onetimeCost = 0;
    building.upgradeAmount.multiply =
        new Building.UpgradeAmount.MultiplyAmount();
    building.upgradeAmount.multiply.upgrades = 9;
    building.upgradeAmount.multiply.multiply = 1.2;

    building.upgradeTime = new Building.UpgradeTime();
    building.upgradeTime.cost = 8.3 * UNIT_cc;
    building.upgradeTime.incomeShorten = 0.6;

    var state = new State();
    state.buildings = [ building ];

    // This assumes the gamestate component is already initialized, so this
    // initial state will be reflected on UI. Thus this is only called in
    // `ngAfterViewInit`.
    this.stateService.setState(state);
  }

  onSave() {
    var request = new SaveRequest();
    request.name = this.saveName.value ?? "";
    request.request = new RunRequest();
    request.request.state = this.stateService.getState();

    this.msg = "saving";
    this.client.save(request).subscribe(
        response => this.msg = "saved",
        error => this.msg = "failed to save: " + error,
        () => {},
    );
  }

  onLoad() {
    var request = new LoadRequest();
    request.name = this.saveName.value ?? "";

    this.msg = "loading";
    this.client.load(request).subscribe(
        response => {
          if (response.request?.state === undefined) {
            this.msg = "failed to load: got empty response"
          } else {
            this.stateService.setState(response.request!.state!);
            this.msg = "loaded";
          }
        },
        error => this.msg = "failed to load: " + error,
        () => {},
    );
  }
}
