#pragma once
class Vector3D {
private:
	float x;
	float y;
	float z;
public:
	Vector3D();
	Vector3D() {};
	float getX() { return x; }
	float getY() { return y; }
	float getZ() { return z; }
	void setX(float x) { this->x = x; }
	void setY(float y) { this->y = y; }
	void setZ(float z) { this->z = z; }

	void Normalize();

	Vector3D operator=(const Vector3D* auxVec) const {
		setX(auxVec->x);
		this->
	}
	Vector3D operator+(const Vector3D* auxVec) const {}
	Vector3D operator-(const Vector3D* auxVec) const {}
	Vector3D operator*(const Vector3D* auxVec) const {}
									   
	Vector3D operator+(const Vector3D* auxVec) const {}
	Vector3D operator-(const Vector3D* auxVec) const {}
	Vector3D operator*(const Vector3D* auxVec) const {}
};