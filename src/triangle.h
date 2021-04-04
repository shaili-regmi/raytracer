#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "hittable.h"
#include "AGLM.h"
#include <math.h>

class triangle : public hittable {
public:
   triangle() : a(0), b(0), c(0), mat_ptr(0) {}
   triangle(const glm::point3& v0, const glm::point3& v1, const glm::point3& v2, 
      std::shared_ptr<material> m) : a(v0), b(v1), c(v2), mat_ptr(m) {};

   virtual bool hit(const ray& r, hit_record& rec) const override
   {
      // todo
       float eps = 0.0001f;
       glm::point3 e1 = b - a;
       glm::point3 e2 = c - a;
       glm::vec3 p = cross(r.direction(), e2);
       float a1 = dot(e1, p);

       if (fabs(a1) < eps) return false;
       
       float f = 1.0f / a1;
       glm::vec3 s = r.origin() - a;
       float u = f * (dot(s, p));
       if (u < 0 || u > 1.0f) return false;

       glm::vec3 q = cross(s, e1);
       float v = f * (dot(r.direction(), q));
       if (v < 0 || (u + v) > 1.0f) return false;

       float t = f * (dot(e2, q));

      // save relevant data in hit record
      rec.t = t; // save the time when we hit the object
      rec.p = r.at(t); // ray.origin + t * ray.direction
      rec.mat_ptr = mat_ptr;

      // save normal
      glm::vec3 outward_normal = normalize(cross(e1, e2)); // compute unit length normal
      rec.set_face_normal(r, outward_normal);

      return true;
   }

public:
   glm::point3 a;
   glm::point3 b;
   glm::point3 c;
   std::shared_ptr<material> mat_ptr;
};

#endif
