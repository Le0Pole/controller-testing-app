/*

Created by LE0POLE.
Email: Le0Pole@pm.me

This library provides classes and functions useful for doing Linear Algebra.

You are free to modify, use and distribute these files, as long as you don't claim their unmodified version as your own.

*/

#pragma once
#include <cmath>
#include <ostream>

struct Vec2 {	
	double x;
	double y;
	
	Vec2() : x(0), y(0) {};
	Vec2(const double& x, const double& y) : x(x), y(y) {};
	Vec2 operator+ (const Vec2& vec) const { return Vec2(x + vec.x,y + vec.y); }
	Vec2 operator- (const Vec2& vec) const { return Vec2(x - vec.x, y - vec.y); }
	template<typename T>
	Vec2 operator* (const T& Scalar) const { return Vec2(x * Scalar, y * Scalar); }
	template<typename T>
	Vec2 operator/ (const T& Scalar) const { return Vec2(x / Scalar, y / Scalar); }
	template<typename T>
	friend Vec2 operator* (const T& Scalar, const Vec2& vec) { return Vec2(Scalar * vec.x, Scalar * vec.y); }
	template<typename T>
	friend Vec2 operator/ (const T& Scalar, const Vec2& vec) { return Vec2(Scalar / vec.x, Scalar / vec.y); }


	double magnitude() const { return std::hypot(x,y); }
	double dotProduct(const Vec2& vec) const { return x * vec.x + y * vec.y;}

	Vec2 normalized() const {
		double mag = magnitude();
		return Vec2(x/mag, y/mag);
	}

	friend std::ostream& operator<<(std::ostream& os, const Vec2& vec) {
		os << "[" << vec.x << ", " << vec.y << "]";
		return os;
	}
};


struct Vec3 {	
	double x;
	double y;
	double z;

	Vec3() : x(0), y(0), z(0) {};
	Vec3(const double& x,const double& y,const double& z) : x(x), y(y), z(z) {};
	Vec3 operator+ (const Vec3& vec) const { return Vec3(x + vec.x,y + vec.y,z + vec.z); }
	Vec3 operator- (const Vec3& vec) const { return Vec3(x - vec.x, y - vec.y, z - vec.z); }
	template<typename T>
	Vec3 operator* (const T& Scalar) const { return Vec3(x * Scalar, y * Scalar, z * Scalar); }
	template<typename T>
	Vec3 operator/ (const T& Scalar) const { return Vec3(x / Scalar, y / Scalar, z / Scalar); }	
	template<typename T>
	friend Vec3 operator* (const T& Scalar, const Vec3& vec) { return Vec3(Scalar * vec.x, Scalar * vec.y, Scalar * vec.z); }
	template<typename T>
	friend Vec3 operator/ (const T& Scalar, const Vec3& vec) { return Vec3(Scalar / vec.x, Scalar / vec.y, Scalar / vec.z); }

	double magnitude() const { return std::sqrt(x*x + y*y + z*z); }
	double dotProduct(const Vec3& vec) const { return x * vec.x + y * vec.y + z * vec.z;}
	
	Vec3 crossProduct(const Vec3& vec) const { return Vec3(y*vec.z - z*vec.y, z*vec.x - x*vec.z, x*vec.y - y*vec.x); }
	Vec3 normalized() const {
		double mag = magnitude();
		return Vec3(x/mag, y/mag, z/mag);
	}
	
	friend std::ostream& operator<<(std::ostream& os, const Vec3& vec) {
		os << "[" << vec.x << ", " << vec.y <<  ", " << vec.z << "]";
		return os;
	}
};


struct Vec4 {	
	double x;
	double y;
	double z;
	double w;

	Vec4() : x(0), y(0), z(0), w(0) {};
	Vec4(const double& x, const double& y, const double& z, const double& w) : x(x), y(y), z(z), w(w) {};
	Vec4 operator+ (const Vec4& vec) const { return Vec4(x + vec.x,y + vec.y,z + vec.z,w + vec.w); }
	Vec4 operator- (const Vec4& vec) const { return Vec4(x - vec.x, y - vec.y, z - vec.z, w - vec.w); }
	template<typename T>
	Vec4 operator* (const T& Scalar) const { return Vec4(x * Scalar, y * Scalar, z * Scalar, w * Scalar); }
	template<typename T>
	Vec4 operator/ (const T& Scalar) const { return Vec4(x / Scalar, y / Scalar, z / Scalar, w * Scalar); }
	template<typename T>
	friend Vec4 operator* (const T& Scalar, const Vec4& vec) { return Vec4(Scalar * vec.x, Scalar * vec.y, Scalar * vec.z, Scalar * vec.w); }
	template<typename T>
	friend Vec4 operator/ (const T& Scalar, const Vec4& vec) { return Vec4(Scalar / vec.x, Scalar / vec.y, Scalar / vec.z, Scalar / vec.w); }

	double magnitude() const { return std::sqrt(x*x + y*y + z*z + w*w); }
	double dotProduct(const Vec4& vec) const { return x * vec.x + y * vec.y + z * vec.z + w * vec.w;}
	
	Vec4 normalized() const {
		double mag = magnitude();
		return Vec4(x/mag, y/mag, z/mag, w/mag);
	}

	friend std::ostream& operator<<(std::ostream& os, const Vec4& vec) {
		os << "[" << vec.x << ", " << vec.y <<  ", " << vec.z << ", " << vec.w << "]";
		return os;
	}
};
