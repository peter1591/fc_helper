#pragma once

#include <sstream>
#include <string>

inline long double operator""_K(long double v) { return v * 1e3; }
inline long double operator""_M(long double v) { return v * 1e6; }
inline long double operator""_B(long double v) { return v * 1e9; }
inline long double operator""_T(long double v) { return v * 1e12; }
inline long double operator""_aa(long double v) { return v * 1e15; }
inline long double operator""_bb(long double v) { return v * 1e18; }

std::string numberToStr(long double v) {
  std::stringstream ss;
  ss.precision(2);
	ss << std::fixed;

  if (v >= 1.0_bb) {
    ss << v / 1.0_bb << "bb";
  } else if (v >= 1.0_aa) {
    ss << v / 1.0_aa << "aa";
  } else if (v >= 1.0_T) {
    ss << v / 1.0_T << "T";
  } else if (v >= 1.0_B) {
    ss << v / 1.0_B << "B";
  } else if (v >= 1.0_M) {
    ss << v / 1.0_M << "M";
  } else if (v >= 1.0_K) {
    ss << v / 1.0_K << "K";
  } else {
    ss << v;
  }
  return ss.str();
}
