/*
 * Car.h
 *
 *  Created on: 24/10/2017
 *      Author: hlc
 */

#ifndef SRC_CAR_H_
#define SRC_CAR_H_

#include <string>

using namespace std;

class Car {
		const string brand;
		const string model;
		float price;
	public:
		Car(string b, string m);
		virtual ~Car() {};
		string getBrand() const;
		string getModel() const;
		float getPrice() const;
		void setPrice(float price);
	 	virtual float range() const = 0;
		bool operator == (const Car &car) const;
		friend ostream & operator<<(ostream & os, const Car &car);
		bool operator<(const Car &car) const;
};

class FuelCar : public Car {
		float tank;   // in liters
		float l_100km;
	public:
		FuelCar(string b, string m);
		FuelCar(string brand, string model, float price, float tank, float l_100km);
		float getTank() const;
		void setTank(float t);
		float getL_100km() const;
		void setL_100km(float l_100km);
		virtual float range() const;
};

class ElectricCar : public Car {
		float battery; // in kWh
		float kWh_100km;
	public:
		ElectricCar(string b, string m);
		ElectricCar(string brand, string model, float price, float battery, float kWh_100km);
		float getBattery() const;
		void setBattery(float b);
		float getKWh_100km() const;
		void setKWh_100km(float kWh_100km);
		virtual float range() const;
};

class HybridCar : public FuelCar, public ElectricCar {
	public:
		HybridCar(std::string brand, std::string model);
		HybridCar(string brand, string model, float price, float tank, float l_100km, float battery, float kWh_100km);
		virtual float range() const;
};

#endif /* SRC_CAR_H_ */
