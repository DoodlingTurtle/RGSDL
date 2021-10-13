#pragma once

namespace RGSDL {
    template <typename T> struct Vec2 {
        T x, y;

        Vec2<T> operator+(const Vec2<T> &o) const {
            return Vec2<T>{this->x + o.x, this->y + o.y};
        }
        Vec2<T> operator-(const Vec2<T> &o) const {
            return Vec2<T>{this->x - o.x, this->y - o.y};
        }
        Vec2<T> operator*(const Vec2<T> &o) const {
            return Vec2<T>{this->x * o.x, this->y * o.y};
        }
        Vec2<T> operator/(const Vec2<T> &o) const {
            return Vec2<T>{this->x / o.x, this->y / o.y};
        }

        Vec2<T> &operator+=(const Vec2<T> &o) {
            this->x += o.x;
            this->y += o.y;
            return *this;
        }
        Vec2<T> &operator-=(const Vec2<T> &o) {
            this->x -= o.x;
            this->y -= o.y;
            return *this;
        }
        Vec2<T> &operator*=(const Vec2<T> &o) {
            this->x *= o.x;
            this->y *= o.y;
            return *this;
        }
        Vec2<T> &operator/=(const Vec2<T> &o) {
            this->x /= o.x;
            this->y /= o.y;
            return *this;
        }

        operator Vec2<float>() {
            return Vec2<float>{static_cast<float>(x), static_cast<float>(y)};
        }
        operator Vec2<double>() {
            return Vec2<double>{static_cast<double>(x), static_cast<double>(y)};
        }
        operator Vec2<int>() {
            return Vec2<int>{static_cast<int>(x), static_cast<int>(y)};
        }

        Vec2<T>(T x, T y) {
            this->x = x;
            this->y = y;
        };
        Vec2<T>(T v = 0) {
            this->x = v;
            this->y = v;
        };

        template<typename C>
        operator Vec2<C> () { return Vec2<C>(static_cast<C>(x), static_cast<C>(y)); }

        void fillArray(T arr[2]) {
            arr[0] = this->x;
            arr[1] = this->y;
        }
    };
}
