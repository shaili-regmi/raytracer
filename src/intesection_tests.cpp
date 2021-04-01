#include <cassert>
#include "AGLM.h"
#include "material.h"
#include "ray.h"
#include "sphere.h"
#include "box.h"
#include "plane.h"
#include "triangle.h"
#include "hittable.h"

using namespace glm;
using namespace std;

float eps = 0.0001f; // epsilon for comparing floats

template <class T>
bool vecEquals(T a, T b)
{
   return all(epsilonEqual(a, b, eps));
}

template <class T>
bool equals(T a, T b)
{
   return abs<T>(a - b) < eps;
}

void check(bool val, const std::string& message, const hit_record& hit, const ray& ray)
{
   if (!val)
   {
      cout << message << endl;
      cout << hit.str() << endl;
      cout << ray.str() << endl;
   }
   assert(val);
}

void check_plane_or_triangle(bool val, const std::string& message, float hit_t, float t)
{
    if (!val)
    {
        cout << message << endl;
        cout << hit_t << " != " << t << endl;
    }
    assert(val);
}

void test_sphere(const sphere& s, const ray& r, bool hits, const hit_record& desired) {
   hit_record hit;
   bool result = s.hit(r, hit);

   check(result == hits, "error: ray should hit", hit, r);
   if (hits) {
      check(vecEquals(hit.p, desired.p), "error: position incorrect:", hit, r);
      check(vecEquals(hit.normal, desired.normal), "error: normal incorrect:", hit, r);
      check(equals(hit.t, desired.t), "error: hit time incorrect", hit, r);
      check(hit.front_face == desired.front_face, "error: front facing incorrect", hit, r);
   }
}

void test_plane(const plane& p, const ray& r, bool hits, float t) {
    hit_record hit;
    bool result = p.hit(r, hit);

    check_plane_or_triangle(result == hits, "error: ray should hit", hit.t, t);
    if (hits) {
        check_plane_or_triangle(equals(hit.t, t), "error: hit time incorrect", hit.t, t);
    }
}

void test_triangle(const triangle& tri, const ray& r, bool hits, float t) {
    hit_record hit;
    bool result = tri.hit(r, hit);

    check_plane_or_triangle(result == hits, "error: ray should hit", hit.t, t);
    if (hits) {
        check_plane_or_triangle(equals(hit.t, t), "error: hit time incorrect", hit.t, t);
    }
}

int main(int argc, char** argv)
{
    
   shared_ptr<material> empty = 0; 
 
   hit_record none = hit_record{ point3(0), point3(0), -1.0f, false, empty};

   sphere s(point3(0), 2.0f, empty);
   test_sphere(s, 
               ray(point3(0, 0, 3), vec3(0, 0, -1)), // ray outside/towards sphere
               true, 
               hit_record{vec3(0,0,2), vec3(0,0,1), 1, true, empty}); 

   test_sphere(s, 
               ray(point3(0, 0, 0), vec3(0, 0, -1)), // ray inside sphere
               true, 
               hit_record{ vec3(0,0,-2), vec3(0,0,1), 2, false, empty}); 

   test_sphere(s, 
               ray(point3(0, 0, 3), vec3(0, 0, 1)), // ray outside/away sphere
               false, 
               none); 

   test_sphere(s, 
               ray(point3(0, 0, 3), vec3(0, 5,-1)), // ray outside/towards sphere (miss)
               false, 
               none); 

   test_sphere(s, 
               ray(point3(0, 0, 3), vec3(0, 1,-3)), // ray outside/towards sphere (hit)
               true, 
               hit_record{vec3(0,0.3432f, 1.9703f), vec3(0,0.1716f, 0.9851f), 0.3432f, true, empty}); 

   // TODO: Your tests here

   // Test plane:

   plane p(vec3(1, 1, 0), vec3(0, 0, 1), empty); // XY plane

   test_plane(p,
       ray(point3(0, 0, 5), vec3(0, 0, -1)), // Ray outside primitive which hits the plane
       true, 5);

   test_plane(p,
       ray(point3(5, 9, 0), vec3(1, 0, -1)), // Ray inside primitive which hits the plane
       true, 0);

   test_plane(p,
       ray(point3(0, 0, 5), vec3(1, 1, 0)), // A ray parallel to XY plane (does not hit)
       false, NULL);

   test_plane(p,
       ray(point3(0, 0, 5), vec3(0, 0, 6)), // A ray outside, pointing away from the primitive (misses)
       false, NULL);

   plane p1(vec3(5, 4, -3), cross(vec3(5, 4, -3), vec3(3, 6, -5)), empty); // Random plane

   test_plane(p1,
       ray(point3(0, 0, 5), vec3(1, 1, -1)), // Ray outside primitive which hits the plane
       true, 22.5f);

   test_plane(p1,
       ray(point3(4, 5, -4), vec3(1, 1, 0)), // Ray inside primitive which hits the plane
       true, 0);

   test_plane(p1,
       ray(point3(0, 0, 5), vec3(1, 1, 10)), // A ray outside, pointing towards the primitive that misses
       false, NULL);

   test_plane(p1,
       ray(point3(0, 0, 5), vec3(-1, -1, 1)), // A ray outside, pointing away from the primitive (misses)
       false, NULL);
  
   // Test Triangle:

   triangle tri(point3(0, 0, 0), point3(5, 5, 1), point3(6, 2, 1), empty);
  
   test_triangle(tri,
       ray(point3(1, 1, 5), vec3(0, 0, -6)), // Ray outside primitive which hits the triangle
       true, 0.8f);

   test_triangle(tri,
       ray(point3(5, 5, 1), vec3(-1, -1, -1)), // Ray inside primitive which hits the triangle
       true, 0);  

   test_triangle(tri,
       ray(point3(1, 1, 5), vec3(10, 10, -6)), // A ray outside, pointing towards the primitive that misses
       false, NULL);

   test_triangle(tri,
       ray(point3(1, 1, 5), vec3(-10, -10, 6)), // A ray outside, pointing away from the primitive (misses)
       false, NULL);
}
