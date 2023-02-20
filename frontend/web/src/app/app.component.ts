import {Component, ViewChild} from '@angular/core';
import {FormControl} from '@angular/forms';
import {GrpcStatusEvent} from '@ngx-grpc/common';

import {RunRequest, RunResponse, State} from '../../build/service.pb'
import {AIServiceClient} from '../../build/service.pbsc';

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

import {GamestateComponent} from './gamestate/gamestate.component';
