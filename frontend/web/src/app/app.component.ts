import {Component, ViewChild} from '@angular/core';
import {FormControl} from '@angular/forms';
import {GrpcStatusEvent} from '@ngx-grpc/common';

import * as pb from '../../build/service.pb'
import {AIServiceClient} from '../../build/service.pbsc';

import {fromNumber, parseNumber} from './gamestate/gamestate.component';
import {StorageService} from './storage.service';

@Component({
  selector : 'app-root',
  templateUrl : './app.component.html',
  styleUrls : [ './app.component.css' ]
})
export class AppComponent {
  msgs: string[] = [];
  bestActions: pb.RunResponse.BestStrategy.Action[] = [];

  targetAmount = new FormControl<string>("");
	resultProgress = 0;

  constructor(
      private client: AIServiceClient,
      private storageService: StorageService,
  ) {
    this.storageService.registerFiller((v: pb.LoadResponse) => {
      v.request!.objective ??= new pb.Objective();
      var o = v.request!.objective;
      o.targetAmount = parseNumber(this.targetAmount.value);
    });
    this.storageService.setSubject$.subscribe(
        o => this.targetAmount.setValue(
            fromNumber(o.request?.objective?.targetAmount)));
  }

  sendRequest() {
    const request = this.storageService.get().request!;
    request.randSeed = 42;
		this.resultProgress = 0;

    this.postMessage('sending request: ' + JSON.stringify(request.toJSON()));

    this.bestActions = [];

    this.client.run(request).subscribe(
        response => this.processResponse(response),
        error => this.processError(error),
        () => { this.postMessage('rpc completed'); })
  }

  private processResponse(response: pb.RunResponse) {
    if (response.bestStrategy) {
			this.postMessage('got response: ' + JSON.stringify(response.toJSON()));
      this.bestActions = response.bestStrategy.actions ??
                         [ pb.RunResponse.BestStrategy.Action.ACTION_UNSET ];
    }
		if (response.progressInfo) {
			var rest = response.progressInfo.newNodeCount / response.progressInfo.newNodeCountMax;
			this.resultProgress = 100 * (1-rest);
		}
  }

  private processError(error: GrpcStatusEvent) {
    this.postMessage('got error: ' + error.statusMessage);
  }

  private postMessage(msg: string) { this.msgs.push(msg); }
}
