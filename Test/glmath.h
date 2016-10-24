#ifndef _GLMATH_H_
#define _GLMATH_H_

#include <GL/gl.h>

typedef GLfloat Real;

typedef union { struct { Real x, y, z; }; struct { Real r, g, b; }; Real row[3]; } vec3;
typedef union { struct { Real x, y, z, w; }; struct { Real r, g, b, a; }; Real row[4]; } vec4;
typedef union { vec4 column[4]; Real data[16]; Real matrix[4][4]; } mat4;

vec3 vec3_zero();
vec3 vec3_init(Real x, Real y, Real z);
vec3 vec3_cross(vec3 a, vec3 b);
Real vec3_dot(vec3 a, vec3 b);
Real vec3_length(vec3 a);
vec3 vec3_plus(vec3 a, vec3 b);
vec3 vec3_minus(vec3 a, vec3 b);
vec3 vec3_mul(vec3 a, Real b);
vec3 vec3_normalize(vec3 a);

// zero vector
vec4 vec4_zero();
// vector from 4 scalars
vec4 vec4_init(Real x, Real y, Real z, Real w);
// sum
vec4 vec4_plus(vec4 a, vec4 b);
// difference
vec4 vec4_minus(vec4 a, vec4 b);
// component product (not inner, nor outer)
vec4 vec4_product(vec4 a, vec4 b);
// product with scalar
vec4 vec4_mul(vec4 a, Real s);

// matrix from scalars
mat4 mat4_init(Real m00, Real m01, Real m02, Real m03, Real m10, Real m11, Real m12, Real m13, Real m20, Real m21, Real
	       m22, Real m23, Real m30, Real m31, Real m32, Real m33);
// matrix from 4 columns
mat4 mat4_initvec4(vec4 a, vec4 b, vec4 c, vec4 d);
// matrix with x on diagonal and zero elsewhere
mat4 mat4_initdiag(Real x);
// zero matrix
mat4 mat4_zero();
// identity matrix
mat4 mat4_identity();
// nth column
vec4 mat4_column(mat4 m, int n);
// nth row
vec4 mat4_row(mat4 m, int n);
// transposed matrix
mat4 mat4_transpose(mat4 m);
// product with scalar
mat4 mat4_mul(mat4 m, Real s);
// product with another matrix
mat4 mat4_product(mat4 a, mat4 b);
// perspective projection matrix (right-handed)
mat4 mat4_perspectiveRH(Real fovy, Real aspect, Real zNear, Real zFar);
// perspective projection matrix (left-handed)
mat4 mat4_perspectiveLH(Real fovy, Real aspect, Real zNear, Real zFar);
// perspective projection matrix (default=right-handed)
mat4 mat4_perspective(Real fovy, Real aspect, Real zNear, Real zFar);
// view matrix (right handed)
mat4 mat4_lookAtRH(vec3 eye, vec3 center, vec3 up);
// view matrix (left-handed)
mat4 mat4_lookAtLH(vec3 eye, vec3 center, vec3 up);
// view matrix (default=right-handed)
mat4 mat4_lookAt(vec3 eye, vec3 center, vec3 up);
// inverse matrix
mat4 compute_inverse(mat4 m);
// rotation matrix
mat4 mat4_rotate( Real angle, vec3 v );
// transform vector
vec3 vec3_transform( mat4 m, vec3 v );
// transform vector
vec4 vec4_transform( mat4 m, vec4 v );

#endif // _GLMATH_H_
