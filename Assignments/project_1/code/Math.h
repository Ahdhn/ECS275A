
#ifndef Math_h
#define Math_h

#ifndef M_PI
#define M_PI 3.141592653589793
#endif

template<typename T>
static inline T Abs(T x) {
  return x<0?-x:x;
}

template<typename T>
static inline T Min(T m1, T m2) {
  return m1<m2?m1:m2;
}

template<typename T>
static inline T Max(T m1, T m2) {
  return m1>=m2?m1:m2;
}

template<typename T>
static inline T Min(T m1, T m2, T m3) {
  return m1<m2?m1<m3?m1:m3:m2<m3?m2:m3;
}

template<typename T>
static inline T Max(T m1, T m2, T m3) {
  return m1>=m2?m1>=m3?m1:m3:m2>=m3?m2:m3;
}

#if 0
// These break the IBM xlC compiler
template<typename T>
static inline T Min(T m1, T m2, T m3, T m4) {
  return Min(Min(m1, m2), Min(m3, m4));
}

template<typename T>
static inline T Max(T m1, T m2, T m3, T m4) {
  return Max(Max(m1, m2), Max(m3, m4));
}
#else
template<typename T>
static inline T Min(T m1, T m2, T m3, T m4) {
  T ma = m1<m2?m1:m2;
  T mb = m3<m4?m3:m4;
  return ma<mb?ma:mb;
}

template<typename T>
static inline T Max(T m1, T m2, T m3, T m4) {
  T ma = m1>=m2?m1:m2;
  T mb = m3>=m4?m3:m4;
  return ma>=mb?ma:mb;
}
#endif

template<typename T>
static inline T Clamp(T value, T min, T max) {
  return value < min? min: value>max? max: value;
}

inline int Floor(double d)
{
  if(d<0){
    int i = -static_cast<int>(-d);
    if(i == d)
      return i;
    else
      return i-1;
  } else {
    return static_cast<int>(d);
  }
}

inline int Ceil(double d)
{
  if(d<0){
    int i = -static_cast<int>(-d);
    return i;
  } else {
    int i = static_cast<int>(d);
    if(i == d)
      return i;
    else
      return i+1;
  }
}

inline double ipow(double x, int p)
{
  double result=1;
  while(p){
    if(p&1)
      result*=x;
    x*=x;
    p>>=1;
  }
  return result;
}

inline float ipow(float x, int p)
{
  float result=1;
  while(p){
    if(p&1)
      result*=x;
    x*=x;
    p>>=1;
  }
  return result;
}

#endif
