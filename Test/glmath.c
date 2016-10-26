#include "glmath.h"
#include <math.h>

//vec2
vec2 vec2_init(Real x, Real y, )
{
	return (vec2){{ x, y } };
}

vec2 vec2_zero()
{
	return vec2_init(0.0, 0.0);
}
/* ------> I've never seen a cross product in dimension inferior to 3
   vec2 vec2_cross(vec2 a, vec2 b)
   {
        return vec3_init(
                       a.y * b.z - a.z * b.y,
                       a.z * b.x - a.x * b.z,
                       a.x * b.y - a.y * b.x
                       );
   }
 */
Real vec2_dot(vec2 a, vec2 b)
{
	return a.x * b.x + a.y * b.y;
}

Real vec2_length(vec2 a)
{
	return sqrt(vec2_dot(a, a));
}

vec2 vec2_plus(vec2 a, vec2 b)
{
	return vec2_init(a.x + b.x, a.y + b.y);
}

vec2 vec2_minus(vec2 a, vec2 b)
{
	return vec2_init(a.x - b.x, a.y - b.y);
}

vec2 vec2_mul(vec2 a, Real b)
{
	return vec2_init(a.x * b, a.y * b);
}

vec2 vec2_normalize(vec2 a)
{
	return vec2_mul(a, 1.0 / vec2_length(a));
}


// vec3

vec3 vec3_init(Real x, Real y, Real z)
{
	return (vec3){{ x, y, z } };
}

vec3 vec3_zero()
{
	return vec3_init(0.0, 0.0, 0.0);
}

vec3 vec3_cross(vec3 a, vec3 b)
{
	return vec3_init(
		       a.y * b.z - a.z * b.y,
		       a.z * b.x - a.x * b.z,
		       a.x * b.y - a.y * b.x
		       );
}

Real vec3_dot(vec3 a, vec3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Real vec3_length(vec3 a)
{
	return sqrt(vec3_dot(a, a));
}

vec3 vec3_plus(vec3 a, vec3 b)
{
	return vec3_init(a.x + b.x, a.y + b.y, a.z + b.z);
}

vec3 vec3_minus(vec3 a, vec3 b)
{
	return vec3_init(a.x - b.x, a.y - b.y, a.z - b.z);
}

vec3 vec3_mul(vec3 a, Real b)
{
	return vec3_init(a.x * b, a.y * b, a.z * b);
}

vec3 vec3_normalize(vec3 a)
{
	return vec3_mul(a, 1.0 / vec3_length(a));
}

// vec4

vec4 vec4_init(Real x, Real y, Real z, Real w)
{
	return (vec4)
	       {
		       {
			       x, y, z, w
		       }
	       };
}

vec4 vec4_zero()
{
	return vec4_init(0.0, 0.0, 0.0, 0.0);
}

vec4 vec4_plus(vec4 a, vec4 b)
{
	return vec4_init(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

vec4 vec4_minus(vec4 a, vec4 b)
{
	return vec4_init(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

Real vec4_inner(vec4 a, vec4 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

vec4 vec4_product(vec4 a, vec4 b)
{
	return vec4_init(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

mat4 vec4_outer(vec4 a, vec4 b)
{
	return mat4_init(
		       a.x * b.x, a.y * b.x, a.z * b.x, a.w * b.x,
		       a.x * b.y, a.y * b.y, a.z * b.y, a.w * b.y,
		       a.x * b.z, a.y * b.z, a.z * b.z, a.w * b.z,
		       a.x * b.w, a.y * b.w, a.z * b.w, a.w * b.w
		       );
}

vec4 vec4_mul(vec4 a, Real s)
{
	return vec4_init(a.x * s, a.y * s, a.z * s, a.w * s);
}

// mat4

mat4 mat4_init(Real m00, Real m01, Real m02, Real m03, Real m10, Real m11, Real m12, Real m13, Real m20, Real m21, Real m22, Real m23, Real m30, Real m31, Real m32, Real m33)
{
	return (mat4)
	       {{
			{ { m00, m01, m02, m03 } },
			{ { m10, m11, m12, m13 } },
			{ { m20, m21, m22, m23 } },
			{ { m30, m31, m32, m33 } }
		} };
}

mat4 mat4_initvec4(vec4 a, vec4 b, vec4 c, vec4 d)
{
	return (mat4)
	       {{
			a, b, c, d
		} };
}

mat4 mat4_initdiag(Real x)
{
	return mat4_init(
		       x, 0.0, 0.0, 0.0,
		       0.0, x, 0.0, 0.0,
		       0.0, 0.0, x, 0.0,
		       0.0, 0.0, 0.0, x
		       );
}

mat4 mat4_zero()
{
	return mat4_initdiag(0.0);
}

mat4 mat4_identity()
{
	return mat4_initdiag(1.0);
}

vec4 mat4_column(mat4 m, int n)
{
	return m.column[n];
}

vec4 mat4_row(mat4 m, int n)
{
	return vec4_init(
		       m.matrix[0][n],
		       m.matrix[1][n],
		       m.matrix[2][n],
		       m.matrix[3][n]
		       );
}

mat4 mat4_transpose(mat4 m)
{
	return mat4_initvec4(
		       mat4_row(m, 0),
		       mat4_row(m, 1),
		       mat4_row(m, 2),
		       mat4_row(m, 3)
		       );
}

mat4 mat4_mul(mat4 m, Real s)
{
	return mat4_initvec4(
		       vec4_mul(m.column[0], s),
		       vec4_mul(m.column[1], s),
		       vec4_mul(m.column[2], s),
		       vec4_mul(m.column[3], s)
		       );
}

mat4 mat4_product(mat4 a, mat4 b)
{
	vec4 R0 = mat4_row(a, 0);
	vec4 R1 = mat4_row(a, 1);
	vec4 R2 = mat4_row(a, 2);
	vec4 R3 = mat4_row(a, 3);
	vec4 C0 = mat4_column(b, 0);
	vec4 C1 = mat4_column(b, 1);
	vec4 C2 = mat4_column(b, 2);
	vec4 C3 = mat4_column(b, 3);

	return mat4_init(
		       vec4_inner(R0, C0),
		       vec4_inner(R1, C0),
		       vec4_inner(R2, C0),
		       vec4_inner(R3, C0),
		       vec4_inner(R0, C1),
		       vec4_inner(R1, C1),
		       vec4_inner(R2, C1),
		       vec4_inner(R3, C1),
		       vec4_inner(R0, C2),
		       vec4_inner(R1, C2),
		       vec4_inner(R2, C2),
		       vec4_inner(R3, C2),
		       vec4_inner(R0, C3),
		       vec4_inner(R1, C3),
		       vec4_inner(R2, C3),
		       vec4_inner(R3, C3)
		       );
}

mat4 mat4_perspectiveRH(Real fovy, Real aspect, Real zNear, Real zFar)
{
	Real tanHalfFovy = tan(fovy / 2.);
	Real depth = zFar - zNear;

	return mat4_init(
		       1. / (aspect * tanHalfFovy), 0.0, 0.0, 0.0,
		       0.0, 1. / tanHalfFovy, 0.0, 0.0,
		       0.0, 0.0, -(zFar + zNear) / depth, -1.0,
		       0.0, 0.0, -(2.0 * zFar * zNear) / depth, 1.0
		       );
}
mat4 mat4_perspectiveLH(Real fovy, Real aspect, Real zNear, Real zFar)
{
	Real tanHalfFovy = tan(fovy / 2.);
	Real depth = zFar - zNear;

	return mat4_init(
		       1. / (aspect * tanHalfFovy), 0.0, 0.0, 0.0,
		       0.0, 1. / tanHalfFovy, 0.0, 0.0,
		       0.0, 0.0, -(zFar + zNear) / depth, 1.0,
		       0.0, 0.0, -(2.0 * zFar * zNear) / depth, 1.0
		       );
}

mat4 mat4_perspective(Real fovy, Real aspect, Real zNear, Real zFar)
{
	return mat4_perspectiveRH(fovy, aspect, zNear, zFar);
}

mat4 mat4_lookAtRH(vec3 eye, vec3 center, vec3 up)
{
	vec3 f = vec3_normalize(vec3_minus(center, eye));
	vec3 s = vec3_normalize(vec3_cross(f, up));
	vec3 u = vec3_cross(s, f);

	return mat4_init(
		       s.x, u.x, -f.x, 0.0,
		       s.y, u.y, -f.y, 0.0,
		       s.z, u.z, -f.z, 0.0,
		       -vec3_dot(s, eye), -vec3_dot(u, eye), vec3_dot(f, eye), 1.0
		       );
}

mat4 mat4_lookAtLH(vec3 eye, vec3 center, vec3 up)
{
	vec3 f = vec3_normalize(vec3_minus(center, eye));
	vec3 s = vec3_normalize(vec3_cross(up, f));
	vec3 u = vec3_cross(f, s);

	return mat4_init(
		       s.x, u.x, f.x, 0.0,
		       s.y, u.y, f.y, 0.0,
		       s.z, u.z, f.z, 0.0,
		       -vec3_dot(s, eye), -vec3_dot(u, eye), -vec3_dot(f, eye), 1.0
		       );
}

mat4 mat4_lookAt(vec3 eye, vec3 center, vec3 up)
{
	return mat4_lookAtRH(eye, center, up);
}

mat4 compute_inverse(mat4 m)
{
	Real C00 = m.matrix[2][2] * m.matrix[3][3] - m.matrix[3][2] * m.matrix[2][3];
	Real C02 = m.matrix[1][2] * m.matrix[3][3] - m.matrix[3][2] * m.matrix[1][3];
	Real C03 = m.matrix[1][2] * m.matrix[2][3] - m.matrix[2][2] * m.matrix[1][3];

	Real C04 = m.matrix[2][1] * m.matrix[3][3] - m.matrix[3][1] * m.matrix[2][3];
	Real C06 = m.matrix[1][1] * m.matrix[3][3] - m.matrix[3][1] * m.matrix[1][3];
	Real C07 = m.matrix[1][1] * m.matrix[2][3] - m.matrix[2][1] * m.matrix[1][3];

	Real C08 = m.matrix[2][1] * m.matrix[3][2] - m.matrix[3][1] * m.matrix[2][2];
	Real C10 = m.matrix[1][1] * m.matrix[3][2] - m.matrix[3][1] * m.matrix[1][2];
	Real C11 = m.matrix[1][1] * m.matrix[2][2] - m.matrix[2][1] * m.matrix[1][2];

	Real C12 = m.matrix[2][0] * m.matrix[3][3] - m.matrix[3][0] * m.matrix[2][3];
	Real C14 = m.matrix[1][0] * m.matrix[3][3] - m.matrix[3][0] * m.matrix[1][3];
	Real C15 = m.matrix[1][0] * m.matrix[2][3] - m.matrix[2][0] * m.matrix[1][3];

	Real C16 = m.matrix[2][0] * m.matrix[3][2] - m.matrix[3][0] * m.matrix[2][2];
	Real C18 = m.matrix[1][0] * m.matrix[3][2] - m.matrix[3][0] * m.matrix[1][2];
	Real C19 = m.matrix[1][0] * m.matrix[2][2] - m.matrix[2][0] * m.matrix[1][2];

	Real C20 = m.matrix[2][0] * m.matrix[3][1] - m.matrix[3][0] * m.matrix[2][1];
	Real C22 = m.matrix[1][0] * m.matrix[3][1] - m.matrix[3][0] * m.matrix[1][1];
	Real C23 = m.matrix[1][0] * m.matrix[2][1] - m.matrix[2][0] * m.matrix[1][1];

	vec4 F0 = vec4_init(C00, C00, C02, C03);
	vec4 F1 = vec4_init(C04, C04, C06, C07);
	vec4 F2 = vec4_init(C08, C08, C10, C11);
	vec4 F3 = vec4_init(C12, C12, C14, C15);
	vec4 F4 = vec4_init(C16, C16, C18, C19);
	vec4 F5 = vec4_init(C20, C20, C22, C23);

	vec4 V0 = vec4_init(m.matrix[1][0],
			    m.matrix[0][0],
			    m.matrix[0][0],
			    m.matrix[0][0]);
	vec4 V1 = vec4_init(m.matrix[1][1],
			    m.matrix[0][1],
			    m.matrix[0][1],
			    m.matrix[0][1]);
	vec4 V2 = vec4_init(m.matrix[1][2],
			    m.matrix[0][2],
			    m.matrix[0][2],
			    m.matrix[0][2]);
	vec4 V3 = vec4_init(m.matrix[1][3],
			    m.matrix[0][3],
			    m.matrix[0][3],
			    m.matrix[0][3]);

	vec4 I0 = vec4_plus(vec4_minus(vec4_product(V1, F0), vec4_product(V2, F1)),
			    vec4_product(V3, F2));
	vec4 I1 = vec4_plus(vec4_minus(vec4_product(V0, F0), vec4_product(V2, F3)),
			    vec4_product(V3, F4));
	vec4 I2 = vec4_plus(vec4_minus(vec4_product(V0, F1), vec4_product(V1, F3)),
			    vec4_product(V3, F5));
	vec4 I3 = vec4_plus(vec4_minus(vec4_product(V0, F2), vec4_product(V1, F4)),
			    vec4_product(V2, F5));

	vec4 SD = vec4_init(+1.0, -1.0, +1.0, -1.0);
	vec4 SI = vec4_init(-1.0, +1.0, -1.0, +1.0);
	mat4 Inv = mat4_initvec4(
		vec4_product(I0, SD),
		vec4_product(I1, SI),
		vec4_product(I2, SD),
		vec4_product(I3, SI)
		);


	vec4 Row0 = vec4_init(Inv.matrix[0][0],
			      Inv.matrix[1][0],
			      Inv.matrix[2][0],
			      Inv.matrix[3][0]);

	vec4 Dot0 = vec4_product(m.column[0], Row0);
	Real Dot1 = (Dot0.x + Dot0.y) + (Dot0.z + Dot0.w);

	Real OneOverDeterminant = 1.0 / Dot1;

	return mat4_mul(Inv, OneOverDeterminant);
}

mat4 mat4_rotate( Real angle, vec3 v )
{
	Real c = cos(angle);
	Real s = sin(angle);

	vec3 axis = vec3_normalize( v );
	vec3 temp = vec3_mul( axis, (1.0 - c) );

	mat4 mr = mat4_identity();

	mr.matrix[0][0] = c + temp.row[0] * axis.row[0];
	mr.matrix[0][1] = 0 + temp.row[0] * axis.row[1] + s * axis.row[2];
	mr.matrix[0][2] = 0 + temp.row[0] * axis.row[2] - s * axis.row[1];

	mr.matrix[1][0] = 0 + temp.row[1] * axis.row[0] - s * axis.row[2];
	mr.matrix[1][1] = c + temp.row[1] * axis.row[1];
	mr.matrix[1][2] = 0 + temp.row[1] * axis.row[2] + s * axis.row[0];

	mr.matrix[2][0] = 0 + temp.row[2] * axis.row[0] + s * axis.row[1];
	mr.matrix[2][1] = 0 + temp.row[2] * axis.row[1] - s * axis.row[0];
	mr.matrix[2][2] = c + temp.row[2] * axis.row[2];

	return mr;
}

vec3 vec3_transform( mat4 m, vec3 v )
{
	vec4 R0 = mat4_row(m, 0);
	vec4 R1 = mat4_row(m, 1);
	vec4 R2 = mat4_row(m, 2);
	vec4 R3 = mat4_row(m, 3);
	vec4 s = vec4_init(v.x, v.y, v.z, 1.0);
	vec4 d = vec4_init(vec4_inner(R0, s),
			   vec4_inner(R1, s),
			   vec4_inner(R2, s),
			   vec4_inner(R3, s)
			   );

	return vec3_init( d.x / d.w, d.y / d.w, d.z / d.w );
}

vec4 vec4_transform( mat4 m, vec4 v )
{
	vec4 R0 = mat4_row(m, 0);
	vec4 R1 = mat4_row(m, 1);
	vec4 R2 = mat4_row(m, 2);
	vec4 R3 = mat4_row(m, 3);

	return vec4_init(vec4_inner(R0, v),
			 vec4_inner(R1, v),
			 vec4_inner(R2, v),
			 vec4_inner(R3, v)
			 );
}
