import {Injectable} from '@angular/core';
import {Subject} from 'rxjs';

import * as pb from '../../build/service.pb';

@Injectable({providedIn : 'root'})
export class StorageService {
  private setSubject = new Subject<pb.LoadResponse>();
  setSubject$ = this.setSubject.asObservable();
  set(o: pb.LoadResponse) { this.setSubject.next(o); }

  // Other component(s) can register a filler to fill up a certain piece of
  // information. The goal is to collect pieces from UI to reflect the current
  // view of the state.
  //
  // Relies on gentleman's aggrement to not collide.
  private fillers: ((v: pb.LoadResponse) => void)[] = [];
  registerFiller(fn: (v: pb.LoadResponse) => void) { this.fillers.push(fn); }

  get(): pb.LoadResponse {
    var o = new pb.LoadResponse();
    o.request = new pb.RunRequest();
    this.fillers.forEach(fn => fn(o));
    return o;
  }
}
