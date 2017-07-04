/*
* Some geometry stuff. Useless and shitty.
* AKorpusenko
*/
//#define _CRT_SECURE_NO_WARNINGS
#include <cmath>
#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

const double PI = acos(-1.0);
const double EPS = 1e-9;

bool doubleEqual(double a, double b) {
	return fabs(a - b) < EPS;
}
bool doubleLessOrEqual(double a, double b) {
	return a < b || doubleEqual(a, b);
}
bool doubleLess(double a, double b) {
	return a < b && !doubleEqual(a, b);
}
bool doubleGreaterOrEqual(double a, double b) {
	return a > b || doubleEqual(a, b);
}
bool doubleGreater(double a, double b) {
	return a > b && !doubleEqual(a, b);
}

class tPoint {
public:
	double x, y;
	tPoint() : x(0.0), y(0.0) {}
	tPoint(double _x, double _y) : x(_x), y(_y) {}

	// DELETE THIS 
	void scan() {
		cin >> x >> y;
	}

	void print(bool k = 1) {
		printf("%.9lf %.9lf", x, y);
		if (k) {
			printf("\n");
		}
	}
	bool operator==(const tPoint & p) const {
		return x == p.x && y == p.y;
	}
	tPoint operator+ (const tPoint & p) const {
		return tPoint(x + p.x, y + p.y);
	}
	tPoint operator- (const tPoint & p) const {
		return tPoint(x - p.x, y - p.y);
	}
	tPoint operator/(double & k) const {
		if (doubleEqual(k, 0.0)) {
			throw "k = 0";
		}
		return tPoint(x / k, y / k);
	}
	tPoint operator*(const double & k) const {
		return tPoint(x * k, y * k);
	}
	double operator% (const tPoint & p) const {
		return x * p.x + y * p.y;
	}
	double operator* (const tPoint & p) const { // pseudo-scalar mull
		return (x * p.y - y * p.x);
	}
	double length() const {
		return sqrt(*this % *this);
	}
	double dist(const tPoint & p) const {
		return (p - *this).length();
	}
	bool isOnline(const tPoint & C, const tPoint & D) const {
		return doubleEqual(0.0, fabs((C - *this) * (D - *this)));
	}
	tPoint normalize(double k = 1.0) {
		double len = length();
		if (doubleEqual(len, 0))
		{
			if (doubleEqual(k, 0))
			{
				return tPoint();
			}
			throw "zero-size vector";
		}
		return *this * (k / len);
	}
	tPoint getH(const tPoint & A, const tPoint & B) const // return point H --  {*this, H} is height vector 
	{
		tPoint C = *this;
		tPoint v = B - A;
		tPoint u = C - A;
		double k = v % u / v.length();
		v = v.normalize(k);
		tPoint H = A + v;
		return H;
	}
	bool isOnsegment(const tPoint & A, const tPoint & B) const {
		return doubleEqual((B - A).length(), (B - *this).length() + (A - *this).length());
	}
	double polar_angle() const {
		double xi = atan2(y, x);

		if (doubleLess(xi, 0.0)) {
			xi += 2 * PI;
		}
		if (doubleEqual(xi, 2 * PI)) {
			xi = 0.0;
		}
		return xi;
	}
	tPoint rotate() const {
		return tPoint(-y, x);
	}
};
class tLine {
public:
	double A, B, C;
	tPoint S, v;

	tLine() : A(0.0), B(0.0), C(0.0) {
		S = tPoint();
		v = tPoint();
	}

	tLine(double A, double B, double C) : A(A), B(B), C(C) {
		v = tPoint(-B, A);

		if (doubleEqual(A, 0.0) || !doubleEqual(A, 0.0) && !doubleEqual(B, 0.0)) {
			// y == const. 
			S = tPoint(0.0, -C / B);
		}
		else {
			// x == const
			S = tPoint(-C / B, 0.0);
		}

	}

	tLine(tPoint _S, tPoint u) {
		A = u.y;
		B = -u.x;
		C = u.x * _S.y - u.y * _S.x;

		S = _S;
		v = u;
	}

	tLine(tPoint _S, tPoint normal, int k) {
		A = normal.x;
		B = normal.y;
		C = -normal.x * _S.x - normal.y * _S.y;

		S = _S;
		v = tPoint(-normal.y, normal.x);
	}
	void scan() {
		cin >> A >> B >> C;
		*this = tLine(A, B, C);
	}
	tLine normalize() {
		double k = 1.0 / (v).length();
		if (doubleGreater(C, 0.0)) {
			k *= -1.0;
		}
		return tLine(A * k, B * k, C * k);
	}

};

int get_intersection // return number of intesections.
(
	const tPoint & A,
	const tPoint & B,
	const tPoint & C,
	const tPoint & D,
	tPoint & O // intersection point
	)
{
	tPoint v = B - A;

	double s1 = (C - A) * (D - A);
	double s2 = (D - B) * (C - B);
	double s = s1 + s2;

	if (doubleEqual(s, 0.0)) {
		if (A.isOnline(C, D)) {
			return 2; // lines are equal
		}
		return 0; // lines are parallel
	}

	v = v / s;
	v = v * s1;

	O = A + v;

	return 1;
}
//*
vector<tPoint> get_segment_intesection
(
	tPoint A,
	tPoint B,
	tPoint C,
	tPoint D
	)
{
	tPoint S1;
	int res;

	res = get_intersection(A, B, C, D, S1);

	switch (res)
	{
	case 1:
		if (S1.isOnsegment(A, B) && S1.isOnsegment(C, D)) {
			return vector<tPoint>({ S1 });
		}
		else {
			return vector<tPoint>();
		}
		break;
	case 2:
		if (doubleEqual((B - A).x, 0.0)) {
			if (doubleGreater(A.y, B.y)) {
				swap(A, B);
			}
			if (doubleGreater(C.y, D.y)) {
				swap(C, D);
			}
			if (doubleGreater(A.y, C.y)) {
				swap(A, C);
				swap(B, D);
			}
		}
		else {
			if (doubleGreater(A.x, B.x)) {
				swap(A, B);
			}
			if (doubleGreater(C.x, D.x)) {
				swap(C, D);
			}
			if (doubleGreater(A.x, C.x)) {
				swap(A, C);
				swap(B, D);
			}
		}
		if (doubleLess(A.dist(C), A.dist(B))) {
			if (doubleLess(A.dist(D), A.dist(B))) {
				return vector<tPoint>({ C, D });
			}
			else {
				return vector<tPoint>({ C, B });
			}
		}
		else if (C == B) {
			return vector<tPoint>({ C });
		}
		else {
			return vector<tPoint>();
		}
		break;
	default:
		return vector<tPoint>();
	}


}
//*/

tPoint get_triangle_bisectrix_intersection
(
	const tPoint & A,
	const tPoint & B,
	const tPoint & C
);
tPoint get_triangle_height_intersection
(
	const tPoint & A,
	const tPoint & B,
	const tPoint & C
);
tPoint get_triangle_segment_bisector_intersection
(
	const tPoint & A,
	const tPoint & B,
	const tPoint & C
);
tPoint get_triangle_median_intersection
(
	const tPoint & A,
	const tPoint & B,
	const tPoint & C
);
int main() {
// DELETE ALL of THIS TOO
	//*
	freopen("trianglepoints.in", "r", stdin);
	freopen("trianglepoints.out", "w", stdout);
	//*/
	/*
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);
	//*/
	//*
	tPoint A, B, C;

	A.scan();
	B.scan();
	C.scan();
	
	tPoint O1 = get_triangle_bisectrix_intersection(A, B, C);
	tPoint O2 = get_triangle_median_intersection(A, B, C);
	tPoint O3 = get_triangle_height_intersection(A, B, C);	
	tPoint O4 = get_triangle_segment_bisector_intersection(A, B, C);

	O1.print();
	O2.print();
	O3.print();	
	O4.print();

	/*
	printf("\n");
	system("pause");
	//*/
	return 0;
}
tPoint get_triangle_bisectrix_intersection
(
	const tPoint & A,
	const tPoint & B,
	const tPoint & C
	)
{
	tPoint ba = (A - B).normalize();
	tPoint bc = (C - B).normalize();

	tPoint ca = (A - C).normalize();
	tPoint cb = (B - C).normalize();

	tPoint abc = ba + bc;
	tPoint bca = cb + ca;

	tPoint O;

	get_intersection(B, B + abc, C, C + bca, O);

	return O;
}

tPoint get_triangle_height_intersection
(
	const tPoint & A,
	const tPoint & B,
	const tPoint & C
)
{
	tPoint O;

	tPoint Hab = C.getH(A, B);
	tPoint Hcb = A.getH(C, B);
	
	tPoint ch = (Hab - C);
	tPoint ah = (Hcb - A);

	get_intersection(Hab, Hab + ch, Hcb, Hcb + ah, O);
	
	return O;
}
tPoint get_triangle_segment_bisector_intersection
(
	const tPoint & A,
	const tPoint & B,
	const tPoint & C
)
{
	tPoint O;
	tPoint Hab, Hac;
	double len;
	
	tPoint ab = (B - A);
	tPoint ac = (C - A);

	len = ab.length();
	Hab = A + ab.normalize(len / 2.0);
	len = ac.length();
	Hac  = A + ac.normalize(len / 2.0);

	get_intersection(Hab, Hab + ab.rotate(), Hac, Hac + ac.rotate(), O);
	
	return O;
}
tPoint get_triangle_median_intersection
(
	const tPoint & A,
	const tPoint & B,
	const tPoint & C
)
{

	tPoint O;
	tPoint Mab, Mac;
	double len;

	tPoint ab = (B - A);
	tPoint ac = (C - A);

	len = ab.length();
	Mab = A + ab.normalize(len / 2.0);
	len = ac.length();
	Mac = A + ac.normalize(len / 2.0);

	get_intersection(Mab, C, Mac, B, O);

	return O;
}