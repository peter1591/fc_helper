import {Injectable} from '@angular/core';
import {Subject} from 'rxjs';

import {State} from '../../build/service.pb';

@Injectable({providedIn : 'root'})
export class StateService {
  // stateGetter should be set only by the gamestate component, so we can always
  // get the gamestate reflected on the ui.
  private stateGetter!: () => State;
  setStateGetter(fn: () => State) { this.stateGetter = fn; }

  getState() { return this.stateGetter(); }

  private stateSet = new Subject<State>();
  stateSet$ = this.stateSet.asObservable();

  setState(state: State) { this.stateSet.next(state); }

  UNIT_PRICE = 0;
}
