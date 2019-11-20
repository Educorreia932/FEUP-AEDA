/*
 * Car.cpp
 *
 *  Created on: 24/10/2017
 *      Author: hlc
 */

#include <iostream>
#include "Car.h"

using namespace std;

Car::Car(string b, string m) : brand(b), model(m) {
	price = 0;
}

std::string Car::getBrand() const {
	return brand;
}

std::string Car::getModel() const {
	return model;
}

float Car::getPrice() const {
	return price;
}

void Car::setPrice(float price) {
	this->price = price;
}

bool Car::operator==(const Car &car) const {
	return brand == car.getBrand() && model == car.getModel();
}

bool Car::operator<(const Car &car) const {
	if(getBrand() < car.getBrand())
		return true;
	else if(getBrand() == car.getBrand()){
		return (getPrice() > car.getPrice());
	}
	return false;
}

FuelCar::FuelCar(string b, string m) : Car(b, m) {
	tank = 0;
	l_100km = 0;
}

FuelCar::FuelCar(string brand, string model, float price, float tank, float l_100km) : Car(brand, model) {
	this->setPrice(price);
	this->tank = tank;
	this->l_100km = l_100km;
}

float FuelCar::getTank() const {
	return tank;
}

void FuelCar::setTank(float t) {
	this->tank = t;
}

float FuelCar::getL_100km() const {
	return l_100km;
}

void FuelCar::setL_100km(float l_100km) {
	this->l_100km = l_100km;
}

float FuelCar::range() const {
	return (tank / l_100km) * 100;
}

ElectricCar::ElectricCar(string b, string m) : Car(b, m) {
	battery = 0;
	kWh_100km = 0;
}

ElectricCar::ElectricCar(string brand, string model, float price, float battery, float kWh_100km) : Car(brand, model) {
	setPrice(price);
	this->battery = battery;
	this->kWh_100km = kWh_100km;
}

float ElectricCar::getBattery() const {
	return battery;
}

void ElectricCar::setBattery(float b) {
	this->battery = b;
}

float ElectricCar::getKWh_100km() const {
	return kWh_100km;
}

void ElectricCar::setKWh_100km(float kWh_100km) {
	this->kWh_100km = kWh_100km;
}

float ElectricCar::range() const {
	return (battery / kWh_100km) * 100;
}

HybridCar::HybridCar(string brand, string model) : FuelCar(brand, model), ElectricCar(brand, model) {
}

HybridCar::HybridCar(string brand, string model, float price, float tank, float l_100km, float battery, float kWh_100km) : FuelCar(brand, model), ElectricCar(brand, model) {
	this->FuelCar::setPrice(price);
	this->setTank(tank);
	this->setL_100km(l_100km);
	this->ElectricCar::setPrice(price);
	this->setBattery(battery);
	this->setKWh_100km(kWh_100km);
}

float HybridCar::range() const {
	return ((getTank() / getL_100km()) + (getBattery() / getKWh_100km())) * 100;
}

