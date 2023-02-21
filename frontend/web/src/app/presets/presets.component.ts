import {Component} from '@angular/core';
import {FormControl} from '@angular/forms';

import {
  Building,
  ListRequest,
  ListResponse,
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
  ) {
    this.loadSaveNames();
  }

  saveNames: string[] = [];
  saveName = new FormControl<string>("default");
  msg = "";

  private loadSaveNames() {
    var request = new ListRequest();
    this.client.list(request).subscribe(
        response => this.saveNames = response.name,
        error => this.msg = "failed to list: " + error,
        () => {},
    );
  }

  ngAfterViewInit() {
    // onLoad() internally calls `this.stateService.setState`, which assumes the
    // gamestate component is already initialized, so this initial state will be
    // reflected on UI. Thus this is only called in `ngAfterViewInit`.
    this.onLoad();
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
