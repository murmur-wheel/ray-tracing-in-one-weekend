#ifndef UTIL_H
#define UTIL_H

#include <cmath>
#include <cstdint>
#include <vector>

using Float = float;

struct Vec3 {
  Float x, y, z;
  Vec3() : x(0), y(0), z(0) {}
  explicit Vec3(Float a) : x(a), y(a), z(a) {}
  explicit Vec3(Float x_, Float y_, Float z_) : x(x_), y(y_), z(z_) {}

  Vec3 operator-() const { return Vec3(-x, -y, -z); }
  Float& operator[](int i) { return (&x)[i]; }
  const Float& operator[](int i) const { return (&x)[i]; }

  Vec3 operator+(const Vec3& rhs) const {
    return Vec3(x + rhs.x, y + rhs.y, z + rhs.z);
  }
  Vec3 operator+(Float a) const { return Vec3(x + a, y + a, z + a); }
  Vec3 operator-(const Vec3& rhs) const {
    return Vec3(x - rhs.x, y - rhs.y, z - rhs.z);
  }
  Vec3 operator*(const Vec3& rhs) const {
    return Vec3(x * rhs.x, y * rhs.y, z * rhs.z);
  }
  Vec3 operator*(Float a) const { return Vec3(x * a, y * a, z * a); }
  Vec3 operator/(const Vec3& rhs) const {
    return Vec3(x / rhs.x, y / rhs.y, z / rhs.z);
  }

  Vec3& operator+=(const Vec3& rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
  }
  Vec3& operator-=(const Vec3& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
  }
  Vec3& operator*=(const Vec3& rhs) {
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
    return *this;
  }
  Vec3& operator/=(const Vec3& rhs) {
    x /= rhs.x;
    y /= rhs.y;
    z /= rhs.z;
    return *this;
  }
  Vec3& operator/=(Float v) {
    Float inv = 1 / v;
    x *= inv;
    y *= inv;
    z *= inv;
    return *this;
  }
};

inline Vec3 operator*(Float t, const Vec3& v) { return v * t; }

inline Float dot(const Vec3& v1, const Vec3& v2) {
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline Float squared_length(const Vec3& v) { return dot(v, v); }

inline Float length(const Vec3& v) { return std::sqrt(squared_length(v)); }

inline Vec3 normalize(const Vec3& v) {
  Float len = length(v);
  return v * (1 / len);
}

inline Vec3 cross(const Vec3& v1, const Vec3& v2) {
  Float x = v1.y * v2.z - v1.z * v2.y;
  Float y = v1.z * v2.x - v1.x * v2.z;
  Float z = v1.x * v2.y - v1.y * v2.x;
  return Vec3(x, y, z);
}

inline Vec3 random_in_unit_sphere() {
  Vec3 p;
  do {
    p = 2.0 * Vec3(drand48(), drand48(), drand48()) - Vec3(1);
  } while (squared_length(p) >= 1.0);
  return p;
}

inline Vec3 reflect(const Vec3& v, const Vec3& n) {
  return v - 2 * dot(v, n) * n;
}

inline bool refract(const Vec3& v, const Vec3& n, Float ni_over_nt,
                    Vec3& refracted) {
  Vec3 uv = normalize(v);
  Float dt = dot(uv, n);
  Float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
  if (discriminant > 0) {
    refracted = ni_over_nt * (uv - n * dt) - n * std::sqrt(discriminant);
    return true;
  } else {
    return false;
  }
}

inline Float schlick(Float cosine, Float ref_idx) {
  Float r0 = (1 - ref_idx) / (1 + ref_idx);
  r0 = r0 * r0;
  return r0 + (1 - r0) * std::pow(1 - cosine, 5);
}

struct Bitmap {
  explicit Bitmap(int cols, int rows) : cols(cols), rows(rows) {
    const int total = cols * rows;
    data.resize(total);
  }

  Vec3* operator[](int row) {
    const int pos = row * cols;
    return &data[pos];
  }

  const Vec3* operator[](int row) const {
    const int pos = row * cols;
    return &data[pos];
  }

  const int cols, rows;
  std::vector<Vec3> data;
};

struct Ray {
  Vec3 origin;
  Vec3 direction;

  Ray() = default;
  explicit Ray(const Vec3& origin, const Vec3& direction)
      : origin(origin), direction(direction) {}
};

// example: save_as_png(bitmap, "output.png");
void save_as_png(const Bitmap& bitmap, const char* filename);

#endif