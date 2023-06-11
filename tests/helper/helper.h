#ifndef MCMV_CORE_TESTS_HELPER_HELPER_H_
#define MCMV_CORE_TESTS_HELPER_HELPER_H_

#include <gtest/gtest.h>
#include "math/vector3.h"
#include "gmock/gmock.h"

using ::testing::FloatEq;
using testing::AllOf;
using testing::Field;

std::ostream& operator<<(std::ostream& out, const Vector3& data)
{
  return out << "Vector3("
             << data.x << ','
             << data.y << ','
             << data.z << ')';
}

MATCHER_P(Vector3Eq, vec, "")
{
  return ExplainMatchResult(
      AllOf(Field("x", &Vector3::x, FloatEq(vec.x)),
            Field("y", &Vector3::y, FloatEq(vec.y)),
            Field("z", &Vector3::z, FloatEq(vec.z))),
      arg, result_listener);
}

std::ostream& operator<<(std::ostream& out, const Quaternion& data)
{
  return out << "Vector3("
             << data.x << ','
             << data.y << ','
             << data.z << ')';
}

MATCHER_P(QuaternionEq, q, "")
{
  return ExplainMatchResult(
      AllOf(Field("x", &Quaternion::x, FloatEq(q.x)),
            Field("y", &Quaternion::y, FloatEq(q.y)),
            Field("z", &Quaternion::z, FloatEq(q.z)),
            Field("w", &Quaternion::w, FloatEq(q.w))),
      arg, result_listener);
}

#endif //MCMV_CORE_TESTS_HELPER_HELPER_H_
