import {Component, ViewChild} from '@angular/core';
import {FormControl} from '@angular/forms';
import {MatTabGroup} from '@angular/material/tabs';

import * as pb from '../../../build/service.pb'
import {StorageService} from '../storage.service';

export const UNIT_K = 1e3;
export const UNIT_M = 1e6;
export const UNIT_B = 1e9;
export const UNIT_T = 1e12;
export const UNIT_aa = 1e15;
export const UNIT_bb = 1e18;
export const UNIT_cc = 1e21;

export function parseNumber(s: string|null): number {
  if (s == null) {
    throw new Error("failed to parse: s is null");
  }

  const c = s[s.length - 1];
  var l = s.length;
  var unit = 1;

  switch (c) {
  case 'K': {
    unit = UNIT_K;
    --l;
    break;
  }
  case 'M': {
    unit = UNIT_M;
    --l;
    break;
  }
  case 'B': {
    unit = UNIT_B;
    --l;
    break;
  }
  case 'T': {
    unit = UNIT_T;
    --l;
    break;
  }
  case 'a': {
    unit = UNIT_aa;
    --l;
    break;
  }
  case 'b': {
    unit = UNIT_bb;
    --l;
    break;
  }
  case 'c': {
    unit = UNIT_cc;
    --l;
    break;
  }
  default: {
    unit = 1;
  }
  }
  const v = Number(s.substr(0, l));
  if (isNaN(v)) {
    throw new Error("failed to parse: " + s);
  }

  return v * unit;
}

export function fromNumber(v: number|undefined) {
  if (v == undefined) {
    return "";
  }

  let formatNumber = (v: number) => v.toFixed(2);

  if (v >= UNIT_cc) {
    return formatNumber(v / UNIT_cc) + "c";
  } else if (v >= UNIT_bb) {
    return formatNumber(v / UNIT_bb) + "b";
  } else if (v >= UNIT_aa) {
    return formatNumber(v / UNIT_aa) + "a";
  } else if (v >= UNIT_T) {
    return formatNumber(v / UNIT_T) + "T";
  } else if (v >= UNIT_B) {
    return formatNumber(v / UNIT_B) + "B";
  } else if (v >= UNIT_M) {
    return formatNumber(v / UNIT_M) + "M";
  } else if (v >= UNIT_K) {
    return formatNumber(v / UNIT_K) + "K";
  }
  return formatNumber(v);
}

class Building {
  id = "";
	buildingName = "";

  incomeAmount = new FormControl<string>("");
  incomeUnit = new FormControl<string>("");
  incomeInterval = new FormControl<string>("");
  upgradeAmountCost = new FormControl<string>("");
  upgradeAmountIncomeIncrease = new FormControl<string>("");
  upgradeAmountOneTimeCost = new FormControl<string>("");
  upgradeAmountMultiplyUpgrades = new FormControl<string>("");
  upgradeAmountMultiplyMultiply = new FormControl<string>("");
  upgradeTimeCost = new FormControl<string>("");
  upgradeTimeIncomeShorten = new FormControl<string>("");
}
function makeBuilding(cityName: string, buildingName: string): Building {
  var o = new Building();
  o.id = cityName + "." + buildingName;
	o.buildingName = buildingName;
  return o;
}

class City {
  name = "";
  buildings: Building[] = [];
  currentBuildingIndex = 0;

  constructor(name: string, buildings: Building[]) {
    this.name = name;
    this.buildings = buildings;
  }
}

@Component({
  selector : 'app-gamestate',
  templateUrl : './gamestate.component.html',
  styleUrls : [ './gamestate.component.css' ]
})
export class GamestateComponent {
  cities: City[] = [
    new City("main",
             [
               makeBuilding("main", "factory1"),
               makeBuilding("main", "factory2"),
               makeBuilding("main", "factory3"),
             ]),
    new City("automn",
             [
               makeBuilding("automn", "unit1"),
               makeBuilding("automn", "unit2"),
               makeBuilding("automn", "unit3"),
               makeBuilding("automn", "unit4"),
               makeBuilding("automn", "unit5"),
               makeBuilding("automn", "unit6"),
               makeBuilding("automn", "unit7"),
               makeBuilding("automn", "unit8"),
               makeBuilding("automn", "unit9"),
             ])
  ];

  public currentCityIndex = 0;
  public currentBuildingIndex = 0;

  constructor(
      private storageService: StorageService,
  ) {
    this.storageService.registerFiller((v: pb.LoadResponse) => {
      v.request!.state = this.buildState();

      v.request!.objective ??= new pb.Objective();
      v.request!.objective.buildingId = this.getSelectedBuildingId();
    });
    this.storageService.setSubject$.subscribe(o => {
      this.setState(o.request!.state!);
      this.selectBuilding(o.request!.objective!.buildingId);
    });
  }

  private loadBuilding(building: Building, source: pb.Building) {
    building.incomeAmount.setValue(fromNumber(source.income?.amount));
    building.incomeUnit.setValue(fromNumber(source.income?.unit));
    building.incomeInterval.setValue(fromNumber(source.income?.interval));
    building.upgradeAmountCost.setValue(fromNumber(source.upgradeAmount?.cost));
    building.upgradeAmountIncomeIncrease.setValue(
        fromNumber(source.upgradeAmount?.incomeIncrease));
    building.upgradeAmountOneTimeCost.setValue(
        fromNumber(source.upgradeAmount?.onetimeCost));
    building.upgradeAmountMultiplyUpgrades.setValue(
        fromNumber(source.upgradeAmount?.multiply?.upgrades));
    building.upgradeAmountMultiplyMultiply.setValue(
        fromNumber(source.upgradeAmount?.multiply?.multiply));
    building.upgradeTimeCost.setValue(fromNumber(source.upgradeTime?.cost));
    building.upgradeTimeIncomeShorten.setValue(
        fromNumber(source.upgradeTime?.incomeShorten));
  }
  private buildBuilding(building: Building): pb.Building {
    var ret = new pb.Building();

    ret.id = building.id;
    ret.currentAmount = 0;
    ret.elapsedTime = 0;

    ret.income = new pb.Building.Income();
    ret.income.amount = parseNumber(building.incomeAmount.value);
    ret.income.unit = parseNumber(building.incomeUnit.value);
    ret.income.interval = parseNumber(building.incomeInterval.value);
    ret.income.otherIncomePerSec = 0;

    ret.upgradeAmount = new pb.Building.UpgradeAmount();
    ret.upgradeAmount.cost = parseNumber(building.upgradeAmountCost.value);
    ret.upgradeAmount.incomeIncrease =
        parseNumber(building.upgradeAmountIncomeIncrease.value);
    ret.upgradeAmount.nextCostMultipler = 1.08;
    ret.upgradeAmount.onetimeCost =
        parseNumber(building.upgradeAmountOneTimeCost.value);
    ret.upgradeAmount.multiply = new pb.Building.UpgradeAmount.MultiplyAmount();
    ret.upgradeAmount.multiply.upgrades =
        parseNumber(building.upgradeAmountMultiplyUpgrades.value);
    ret.upgradeAmount.multiply.multiply =
        parseNumber(building.upgradeAmountMultiplyMultiply.value);
    ret.upgradeAmount.availables = 10;

    ret.upgradeTime = new pb.Building.UpgradeTime();
    ret.upgradeTime.cost = parseNumber(building.upgradeTimeCost.value);
    ret.upgradeTime.incomeShorten =
        parseNumber(building.upgradeTimeIncomeShorten.value);
    ret.upgradeTime.nextCostMultipler = 1.08;
    ret.upgradeTime.availables = 10;

    return ret;
  }

  private getPbBuilding(state: pb.State, buildingId: string): pb.Building {
    var found: pb.Building|undefined;
    if (state.buildings != undefined) {
      state.buildings.forEach(pbBuilding => {
        if (pbBuilding.id == buildingId) {
          found = pbBuilding;
        }
      })
    }
    return found || new pb.Building();
  }

  setState(state: pb.State) {
    this.cities.forEach(
        city => {city.buildings.forEach(building => {
          this.loadBuilding(building, this.getPbBuilding(state, building.id));
        })});
  }

  buildState() {
    var state = new pb.State();
    state.buildings = [];
    this.cities.forEach(city => {city.buildings.forEach(building => {
                          state.buildings!.push(this.buildBuilding(building));
                        })});
    return state;
  }

  getSelectedBuildingId() {
    const city = this.cities[this.currentCityIndex];
    return city.buildings[city.currentBuildingIndex].id;
  }

  selectBuilding(buildingId: string) {
    console.log("trying to select to building: " + buildingId);
    this.cities.forEach((city, cityIndex) => city.buildings.forEach(
                            (building, buildingIndex) => {
                              if (building.id== buildingId) {
                                this.currentCityIndex = cityIndex;
                                city.currentBuildingIndex = buildingIndex;
                              }
                            }));
  }
}
