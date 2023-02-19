import { Component } from '@angular/core';
import { GrpcStatusEvent } from '@ngx-grpc/common';

import { AIServiceClient } from '../../build/service.pbsc'
import { RunRequest, RunResponse } from 'build/service.pb'

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent {
  title = 'web';
	msgs : string[]  = [];

	constructor(
		private client: AIServiceClient,
	) {
		this.sendRequest();
 	}

	private sendRequest() {
		const request = new RunRequest();
		this.postMessage('sending request: ' + JSON.stringify(request.toJSON()));

		this.client.run(request).subscribe(
			response => this.processResponse(response),
			error => this.processError(error),
			() => {
				this.postMessage('rpc completed');
			}
		)
	}

	private processResponse(response : RunResponse) {
		this.postMessage('got response: ' + JSON.stringify(response.toJSON()));
	}

	private processError(error : GrpcStatusEvent) {
		this.postMessage('got error: ' + error.statusMessage);
	}

	private postMessage(msg : string) {
		this.msgs.push(msg);
	}
}
