#pragma once
#include <cstdint>
#include <iostream>
#include <string>

class decimal {
  private:
	int64_t significand = 0;
	int8_t power = 1;

  public:
	decimal();
	decimal(const decimal &);
	decimal(decimal&&);
	decimal(int64_t);
	decimal(const char *);
	decimal(int64_t significand, int8_t power);

	friend std::istream &operator>>(const std::istream &, decimal &);
	friend std::ostream &operator<<(const std::ostream &, const decimal &);

	friend decimal &operator+(const decimal &, const decimal &);
	friend decimal &operator+(int64_t, const decimal &);
	friend decimal &operator+(const decimal &, int64_t);

	friend decimal &operator-(const decimal &, const decimal &);
	friend decimal &operator-(int64_t, const decimal &);
	friend decimal &operator-(const decimal &, int64_t);

	friend decimal &operator*(const decimal &, const decimal &);
	friend decimal &operator*(int64_t, const decimal &);
	friend decimal &operator*(const decimal &, int64_t);

	decimal &operator+=(const decimal &);
	decimal &operator+=(int64_t);

	decimal &operator-=(const decimal &);
	decimal &operator-=(int64_t);

	decimal &operator*=(const decimal &);
	decimal &operator*=(int64_t);

	friend bool operator==(const decimal &, const decimal &);
	friend bool operator==(const decimal &, int64_t);
	friend bool operator==(int64_t, const decimal &);

	// for making a literal of type "2.07"d
	friend decimal operator"" _d(const char *,size_t);
};