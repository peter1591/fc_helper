import {Component, ViewChild} from '@angular/core';
import {FormControl} from '@angular/forms';
import {GrpcStatusEvent} from '@ngx-grpc/common';

import * as pb from '../../build/service.pb'
import {AIServiceClient} from '../../build/service.pbsc';

import {GamestateComponent} from './gamestate/gamestate.component';
import {StateService} from './state.service';

@Component({
  selector : 'app-root',
  templateUrl : './app.component.html',
  styleUrls : [ './app.component.css' ]
})
export class AppComponent {
  msgs: string[] = [];
  bestActions: pb.RunResponse.BestStrategy.Action[] = [];

	@ViewChild(GamestateComponent) gamestate! : GamestateComponent;

  constructor(
      private client: AIServiceClient,
      private stateService: StateService,
  ) {}

  sendRequest() {
    const request = new pb.RunRequest();
    request.state = this.stateService.getState();
		request.objective = new pb.Objective(); 
		request.objective.buildingName = this.gamestate.getBuildingName();
    request.randSeed = 42;
    request.iterationReportInterval = 1000000;

    this.postMessage('sending request: ' + JSON.stringify(request.toJSON()));

    this.client.run(request).subscribe(
        response => this.processResponse(response),
        error => this.processError(error),
        () => { this.postMessage('rpc completed'); })
  }

  private processResponse(response: pb.RunResponse) {
    this.postMessage('got response: ' + JSON.stringify(response.toJSON()));

    if (response.bestStrategy) {
      this.bestActions = response.bestStrategy.actions ??
                         [ pb.RunResponse.BestStrategy.Action.ACTION_UNSET ];
    }
  }

  private processError(error: GrpcStatusEvent) {
    this.postMessage('got error: ' + error.statusMessage);
  }

  private postMessage(msg: string) { this.msgs.push(msg); }
}
