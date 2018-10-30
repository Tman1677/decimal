#pragma once
#include <cstdint>
#include <iostream>
#include <cstdlib> //for exit()
#include <string>

class decimal {
  private:
	int64_t significand = 0;
	int8_t power = 0;
	//construct with the internal desired values, not for public interface
	//for use by friend classes
	decimal(int64_t significand, int8_t power);
	//this could possibly be more efficient as a proper member class
	static int64_t tensPow(int64_t significand, int8_t power);
	//reduces away any possible trailing zeros
	decimal& reduce();

  public:
	//default constructor giving value of 0
	decimal();
	//copy constructor
	decimal(const decimal &);
	// no actual move semantics but might as well implement for the future
	decimal(decimal&&);
	//construct with an integer
	decimal(int64_t);
	//construct with a c string
	decimal(const char *);
	
	//insertion and extraction operators
	friend std::istream &operator>>(std::istream &, decimal &);
	friend std::ostream &operator<<(std::ostream &, const decimal &);

	//basic arithmatic operators
	friend decimal operator+(const decimal &, const decimal &);
	friend decimal operator+(int64_t, const decimal &);
	friend decimal operator+(const decimal &, int64_t);
	friend decimal operator-(const decimal &, const decimal &);
	friend decimal operator-(int64_t, const decimal &);
	friend decimal operator-(const decimal &, int64_t);
	friend decimal operator*(const decimal &, const decimal &);
	friend decimal operator*(int64_t, const decimal &);
	friend decimal operator*(const decimal &, int64_t);
	//unary operator
	friend decimal operator-(const decimal &);

	//copy assignment
	decimal &operator=(const decimal &);
	// no actual move semantics but might as well implement for the future
	decimal &operator=(decimal &&);

	//basic arithmatic operators
	decimal &operator+=(const decimal &);
	decimal &operator+=(int64_t);
	decimal &operator-=(const decimal &);
	decimal &operator-=(int64_t);
	decimal &operator*=(const decimal &);
	decimal &operator*=(int64_t);

	//basic equality checks
	friend bool operator==(const decimal &, const decimal &);
	friend bool operator==(const decimal &, int64_t);
	friend bool operator==(int64_t, const decimal &);

	// for making a literal of type "2.07"d
	friend decimal operator"" _d(const char *,size_t);
};

decimal::decimal() {}

//private constructor only to be used by friend classes
decimal::decimal(int64_t significand, int8_t power) {
	this->significand = significand;
	this->power = power;
}

decimal::decimal(const decimal& copier) {
	significand = copier.significand;
	power = copier.power;
	//I think a reduce is unecessary here
}
decimal::decimal(decimal&& mover) { 
	significand = mover.significand;
	power = mover.power;
	//I think a reduce is unecessary here
}
decimal::decimal(int64_t value) {
	significand = value;
	reduce();
}
decimal::decimal(const char * cstring) {
	//this section needs optimizing terribly bad
	std::string string (cstring);
	size_t period = string.find('.');
	size_t length = string.length();

	//if there isn't a period
	if (period==std::string::npos) {
		power = length - period - 1;
		significand = std::stoll(string);
	} else { //if there is
		power = period + 1 - length;
		string.erase(period,1);
		significand = std::stoll(string);
	}
	reduce();
}

decimal operator"" _d(const char * cstring, size_t length) {
	//this section needs optimizing terribly bad
	std::string string (cstring);
	size_t period = string.find('.');
	decimal result;

	//if there isn't a period
	if (period==std::string::npos) {
		result.power = length - period - 1;
		result.significand = std::stoll(string);
	} else { //if there is
		result.power = period + 1 - length;
		string.erase(period,1);
		result.significand = std::stoll(string);
	}
	//reduce any possible trailing zeros
	while(result.significand % 10 == 0) {
		result.power++;
		result.significand /= 10;
	}
	return result;
}

std::istream &operator>>(std::istream & instream, decimal & result) {
	//this section needs optimizing terribly bad
	std::string string;
	instream >> string;
	size_t period = string.find('.');
	size_t length = string.length();

	//if there isn't a period
	if (period==std::string::npos) {
		result.power = length - period - 1;
		result.significand = std::stoll(string);
	} else { //if there is
		result.power = period + 1 - length;
		string.erase(period,1);
		result.significand = std::stoll(string);
	}
	//reduce any possible trailing zeros
	while(result.significand % 10 == 0) {
		result.power++;
		result.significand /= 10;
	}
	return instream;
}
std::ostream &operator<<(std::ostream & outstream, const decimal & object) {
	//needs optimization badly
	std::string outstring = std::to_string(object.significand);
	if(object.power > 0) {
		for(int8_t i = 0; i < object.power; i++) {
			outstring.push_back('0');
		}
	} else if(object.power < 0) {
		if(object.power + outstring.length() == 0) {
			outstring.insert(0,"0.");
		} else {
			outstring.insert(outstring.begin() + outstring.length() + object.power,'.');
		}
	}
	outstream << outstring;
	return outstream;
}
int64_t decimal::tensPow(int64_t significand, int8_t power) {
		if (power < 0) {
			std::cerr << "This method should only be used with positive tens powers to prevent a loss of data\n";
		}
		while (power > 0) {
			significand *= 10;
			power--;
		}
		return significand;
}
decimal& decimal::reduce() {
	//reduce any possible trailing zeros
	while(significand % 10 == 0) {
		power++;
		significand /= 10;
	}
	return *this;
}
decimal operator+(const decimal & object1, const decimal & object2) {
	if(object1.power == object2.power) {
		return decimal(object1.significand + object2.significand,object1.power);
	} else if (object1.power < object2.power) {
		//shifts the number with a bigger power up an appropriate amount of digits
		return decimal(object1.significand + decimal::tensPow(object2.significand,object2.power - object1.power),object1.power);
	} else {
		//shifts the number with a bigger power up an appropriate amount of digits
		return decimal(object2.significand + decimal::tensPow(object1.significand,object1.power - object2.power),object2.power);
	}
}

decimal operator+(int64_t primitive1, const decimal &object1) {
	if(object1.power == 0) {
		return decimal(object1.significand + primitive1,object1.power);
	} else if (object1.power < 0) {
		//shifts the number with a bigger power up an appropriate amount of digits
		return decimal(object1.significand + decimal::tensPow(primitive1,-object1.power),object1.power);
	} else {
		//shifts the number with a bigger power up an appropriate amount of digits
		return decimal(primitive1 + decimal::tensPow(object1.significand,object1.power),0);
	}
}
decimal operator+(const decimal &object1, int64_t primitive1) {
	if(object1.power == 0) {
		return decimal(object1.significand + primitive1,object1.power);
	} else if (object1.power < 0) {
		//shifts the number with a bigger power up an appropriate amount of digits
		return decimal(object1.significand + decimal::tensPow(primitive1,-object1.power),object1.power);
	} else {
		//shifts the number with a bigger power up an appropriate amount of digits
		return decimal(primitive1 + decimal::tensPow(object1.significand,object1.power),0);
	}
}
decimal operator-(const decimal &object1, const decimal &object2) {
	if(object1.power == object2.power) {
		return decimal(object1.significand - object2.significand,object1.power);
	} else if (object1.power < object2.power) {
		//shifts the number with a bigger power up an appropriate amount of digits
		return decimal(object1.significand - decimal::tensPow(object2.significand,object2.power - object1.power),object1.power);
	} else {
		//shifts the number with a bigger power up an appropriate amount of digits
		return decimal(-object2.significand + decimal::tensPow(object1.significand,object1.power - object2.power),object2.power);
	}
}
decimal operator-(int64_t primitive1, const decimal &object1) {
	if(object1.power == 0) {
		return decimal(-object1.significand + primitive1,object1.power);
	} else if (object1.power < 0) {
		//shifts the number with a bigger power up an appropriate amount of digits
		return decimal(-object1.significand + decimal::tensPow(primitive1,-object1.power),object1.power);
	} else {
		//shifts the number with a bigger power up an appropriate amount of digits
		return decimal(primitive1 - decimal::tensPow(object1.significand,object1.power),0);
	}
}
decimal operator-(const decimal &object1, int64_t primitive1) {
	if(object1.power == 0) {
		return decimal(object1.significand - primitive1,object1.power);
	} else if (object1.power < 0) {
		//shifts the number with a bigger power up an appropriate amount of digits
		return decimal(object1.significand - decimal::tensPow(primitive1,-object1.power),object1.power);
	} else {
		//shifts the number with a bigger power up an appropriate amount of digits
		return decimal(-primitive1 + decimal::tensPow(object1.significand,object1.power),0);
	}
}
decimal operator*(const decimal & object1, const decimal & object2) {
	return decimal(object1.significand * object2.significand,object1.power + object2.power);
}
decimal operator*(int64_t primitive1, const decimal &object1) {
	return decimal(object1.significand * primitive1,object1.power);
}
decimal operator*(const decimal &object1, int64_t primitive1) {
	return decimal(object1.significand * primitive1,object1.power);
}
decimal operator-(const decimal & object1) {
	return decimal(-object1.significand,object1.power);
}
decimal& decimal::operator=(const decimal & object1) {
	significand = object1.significand;
	power = object1.power;
	return *this;
}
decimal& decimal::operator=(decimal && object1) {
	significand = object1.significand;
	power = object1.power;
	return *this;
}
decimal& decimal::operator+=(const decimal & object1) {
	if(object1.power == power) {
		significand += object1.significand;
	} else if (object1.power < power) {
		//shifts the number with a bigger power up an appropriate amount of digits
		significand = object1.significand + decimal::tensPow(significand,power - object1.power);
		power = object1.power;
	} else {
		//shifts the number with a bigger power up an appropriate amount of digits
		significand = significand + decimal::tensPow(object1.significand,object1.power - power);
	}
	reduce();
	return *this;
}
decimal& decimal::operator+=(int64_t primitive) {
	if(0 == power) {
		significand += primitive;
	} else if (0 < power) {
		//shifts the number with a bigger power up an appropriate amount of digits
		significand = primitive + decimal::tensPow(significand,power);
		power = 0;
	} else {
		//shifts the number with a bigger power up an appropriate amount of digits
		significand = significand + decimal::tensPow(primitive,-power);
	}
	reduce();
	return *this;
}
decimal& decimal::operator-=(const decimal & object1) {
	if(object1.power == power) {
		significand -= object1.significand;
	} else if (object1.power < power) {
		//shifts the number with a bigger power up an appropriate amount of digits
		significand = -object1.significand + decimal::tensPow(significand,power - object1.power);
		power = object1.power;
	} else {
		//shifts the number with a bigger power up an appropriate amount of digits
		significand = significand - decimal::tensPow(object1.significand,object1.power - power);
	}
	reduce();
	return *this;
}
decimal& decimal::operator-=(int64_t primitive) {
	if(0 == power) {
		significand -= primitive;
	} else if (0 < power) {
		//shifts the number with a bigger power up an appropriate amount of digits
		significand = -primitive + decimal::tensPow(significand,power);
		power = 0;
	} else {
		//shifts the number with a bigger power up an appropriate amount of digits
		significand = significand - decimal::tensPow(primitive,-power);
	}
	reduce();
	return *this;
}
decimal& decimal::operator*=(const decimal & object1) {
	significand *= object1.significand;
	power += object1.power;
	reduce();
	return *this;
}
decimal& decimal::operator*=(int64_t primitive) {
	significand *= primitive;
	reduce();
	return *this;
}
bool operator==(const decimal & object1, const decimal & object2) {
	return object1.significand == object2.significand && object1.power == object2.power;
}
bool operator==(const decimal & object1, int64_t primitive) {
	return object1.significand == primitive && object1.power == 0;
}
bool operator==(int64_t primitive, const decimal & object1) {
	return object1.significand == primitive && object1.power == 0;
}
