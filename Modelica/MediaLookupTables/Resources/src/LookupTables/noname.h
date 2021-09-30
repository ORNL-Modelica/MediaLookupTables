#ifndef NONAME_H
#define NONAME_H

// Detect the platform
#if defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) ||                \
    defined(__WIN64__)
#define __ISWINDOWS__
#elif __APPLE__
#define __ISAPPLE__
#elif __linux
#define __ISLINUX__
#endif

// Define the EXPORT macro on windows
#ifndef EXPORT
#if defined(__ISLINUX__)
#define EXPORT
#elif defined(__ISAPPLE__)
#define EXPORT
#else
#define EXPORT __declspec(dllexport)
#endif
#endif

#ifdef __cplusplus

extern "C" {
#endif

EXPORT double noname_spline2d_eval(char const *o, double x, double y);
EXPORT double noname_spline2d_eval_deriv_x(char const *o, double x, double y);
EXPORT double noname_spline2d_eval_deriv_y(char const *o, double x, double y);
EXPORT double noname_spline2d_eval_deriv_xx(char const *o, double x, double y);
EXPORT double noname_spline2d_eval_deriv_yy(char const *o, double x, double y);
EXPORT double noname_spline2d_eval_deriv_xy(char const *o, double x, double y);

#ifdef __cplusplus
}
#endif

#endif
