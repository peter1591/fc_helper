import {NgModule, isDevMode} from '@angular/core';
import {FormsModule, ReactiveFormsModule} from '@angular/forms';
import {BrowserModule} from '@angular/platform-browser';
import {GrpcCoreModule} from '@ngx-grpc/core';
import {GrpcWebClientModule} from '@ngx-grpc/grpc-web-client';

import {AppComponent} from './app.component';
import { ServiceWorkerModule } from '@angular/service-worker';

@NgModule({
  declarations : [ AppComponent ],
  imports : [
    BrowserModule,
    FormsModule,
    ReactiveFormsModule,
    GrpcCoreModule.forRoot(),
    GrpcWebClientModule.forRoot({
      settings : {host : 'http://lilyss.ddns.net:5782'},
    }),
    ServiceWorkerModule.register('ngsw-worker.js', {
      enabled: !isDevMode(),
      // Register the ServiceWorker as soon as the application is stable
      // or after 30 seconds (whichever comes first).
      registrationStrategy: 'registerWhenStable:30000'
    }),
  ],
  providers : [],
  bootstrap : [ AppComponent ]
})
export class AppModule {
}
