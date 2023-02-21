import {isDevMode, NgModule} from '@angular/core';
import {FormsModule, ReactiveFormsModule} from '@angular/forms';
import {MatAutocompleteModule} from '@angular/material/autocomplete';
import {MatButtonModule} from '@angular/material/button';
import {MatDividerModule} from '@angular/material/divider';
import {MatInputModule} from '@angular/material/input';
import {BrowserModule} from '@angular/platform-browser';
import {BrowserAnimationsModule} from '@angular/platform-browser/animations';
import {ServiceWorkerModule} from '@angular/service-worker';
import {GrpcCoreModule} from '@ngx-grpc/core';
import {GrpcWebClientModule} from '@ngx-grpc/grpc-web-client';

import {AppComponent} from './app.component';
import {GamestateComponent} from './gamestate/gamestate.component';
import {PresetsComponent} from './presets/presets.component';

@NgModule({
  declarations : [ AppComponent, GamestateComponent, PresetsComponent ],
  imports : [
    BrowserModule,
    FormsModule,
    ReactiveFormsModule,
    GrpcCoreModule.forRoot(),
    GrpcWebClientModule.forRoot({
      settings : {host : 'http://lilyss.ddns.net:5782'},
    }),
    ServiceWorkerModule.register('ngsw-worker.js', {
      enabled : !isDevMode(),
      // Register the ServiceWorker as soon as the application is stable
      // or after 30 seconds (whichever comes first).
      registrationStrategy : 'registerWhenStable:30000'
    }),
    BrowserAnimationsModule,
    MatAutocompleteModule,
    MatDividerModule,
    MatButtonModule,
    MatInputModule,
  ],
  providers : [],
  bootstrap : [ AppComponent ]
})
export class AppModule {
}
