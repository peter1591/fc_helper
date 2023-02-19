import {NgModule} from '@angular/core';
import {FormsModule, ReactiveFormsModule} from '@angular/forms';
import {BrowserModule} from '@angular/platform-browser';
import {GrpcCoreModule} from '@ngx-grpc/core';
import {GrpcWebClientModule} from '@ngx-grpc/grpc-web-client';

import {AppComponent} from './app.component';

@NgModule({
  declarations : [ AppComponent ],
  imports : [
    BrowserModule,
    FormsModule,
    ReactiveFormsModule,
    GrpcCoreModule.forRoot(),
    GrpcWebClientModule.forRoot({
      settings : {host : 'http://192.168.1.10:5782'},
    }),
  ],
  providers : [],
  bootstrap : [ AppComponent ]
})
export class AppModule {
}
