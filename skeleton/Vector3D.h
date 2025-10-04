#pragma once
#include <cmath>
class Vector3D {
private:
	float _x;
	float _y;
	float _z;
public:
	Vector3D() : _x(0), _y(0), _z(0) {};
	Vector3D(float x, float y, float z) : _x(x), _y(y), _z(z) {};
	float getX() { return _x; }
	float getY() { return _y; }
	float getZ() { return _z; }
	void setX(float x) { this->_x = x; }
	void setY(float y) { this->_y = y; }
	void setZ(float z) { this->_z = z; }

	void Normalize() {
		double mag = sqrt(pow(_x, 2) + pow(_y, 2) + pow(_z, 2));
		_x = _x / mag;
		_y = _y / mag;
		_z = _z / mag;
	}

	float Module() const {
		double mag = sqrt(pow(_x, 2) + pow(_y, 2) + pow(_z, 2));
		return mag;
	}

	//OPERADORES CON VECTORES AUXILIARES
	Vector3D& operator=(const Vector3D* auxVec) const {
		return Vector3D(auxVec->_x, auxVec->_y, auxVec->_z);
	}

	Vector3D& operator+(const Vector3D* auxVec) const {
		return Vector3D(_x + auxVec->_x, _y + auxVec->_y, _z + auxVec->_z);
	}

	Vector3D& operator-(const Vector3D* auxVec) const {
		return Vector3D(_x - auxVec->_x, _y - auxVec->_y, _z - auxVec->_z);
	}
	
	//OPERADORES CON NUMERO
	Vector3D& operator+(int num) const {
		return Vector3D(_x + num, _y + num, _z + num);
	}
	Vector3D& operator-(int num) const {
		return Vector3D(_x + num, _y + num, _z + num);
	}
	
	//PRODUCTO ESCALAR
	Vector3D& operator*(int num) const {
		return Vector3D(_x * num, _y * num, _z * num);
	}
	//PRODUCTO VECTORIAL
	Vector3D& operator*(const Vector3D* auxVec) const {
		return Vector3D(_x * auxVec->_x, _y * auxVec->_y, _z * auxVec->_z);
	}
};