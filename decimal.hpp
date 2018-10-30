#pragma once
#include <cstdint>
#include <cstdlib> //for exit()
#include <iostream>
#include <string>

class decimal {
  private:
	// a large integer holding the significant values of the decimal
	int64_t significand = 0;
	// a small integer holding the scientific notation exponent of the decimal
	int8_t power = 0;
	// construct with the internal desired values, not for public interface
	// for use by friend functions
	decimal(int64_t significand, int8_t power);
	// this could possibly be more efficient as a proper member class
	static int64_t tensPow(int64_t significand, int8_t power);
	// reduces away any possible trailing zeros
	decimal &reduce();

  public:
	// default constructor giving value of 0
	decimal();
	// copy constructor
	decimal(const decimal &);
	// no actual move semantics but might as well implement for the future
	decimal(decimal &&);
	// construct with an integer
	decimal(int64_t);
	// construct with a c string
	decimal(const char *);

	// insertion and extraction operators
	friend std::istream &operator>>(std::istream &, decimal &);
	friend std::ostream &operator<<(std::ostream &, const decimal &);

	// basic arithmatic operators
	friend decimal operator+(const decimal &, const decimal &);
	friend decimal operator+(int64_t, const decimal &);
	friend decimal operator+(const decimal &, int64_t);
	friend decimal operator-(const decimal &, const decimal &);
	friend decimal operator-(int64_t, const decimal &);
	friend decimal operator-(const decimal &, int64_t);
	friend decimal operator*(const decimal &, const decimal &);
	friend decimal operator*(int64_t, const decimal &);
	friend decimal operator*(const decimal &, int64_t);
	// unary operator
	friend decimal operator-(const decimal &);

	// copy assignment
	decimal &operator=(const decimal &);
	// no actual move semantics but might as well implement for the future
	decimal &operator=(decimal &&);

	// basic arithmatic operators
	decimal &operator+=(const decimal &);
	decimal &operator+=(int64_t);
	decimal &operator-=(const decimal &);
	decimal &operator-=(int64_t);
	decimal &operator*=(const decimal &);
	decimal &operator*=(int64_t);

	// basic equality checks
	friend bool operator==(const decimal &, const decimal &);
	friend bool operator==(const decimal &, int64_t);
	friend bool operator==(int64_t, const decimal &);

	// for making a literal of type "2.07"d
	friend decimal operator"" _d(const char *, size_t);
};