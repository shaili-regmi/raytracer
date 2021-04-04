// Raytracer framework from https://raytracing.github.io by Peter Shirley, 2018-2020
// alinen 2021, modified to use glm and ppm_image class

#include "ppm_image.h"
#include "AGLM.h"
#include "ray.h"
#include "sphere.h"
#include "plane.h"
#include "triangle.h"
#include "camera.h"
#include "material.h"
#include "hittable_list.h"

using namespace glm;
using namespace agl;
using namespace std;


color ray_color(const ray& r, const hittable_list& world, int depth)
{
   hit_record rec;
   if (depth <= 0)
   {
      return color(0);
   }

   if (world.hit(r, 0.001f, infinity, rec))
   {
      ray scattered;
      color attenuation;
      if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
      {
         color recurseColor = ray_color(scattered, world, depth - 1);
         return attenuation * recurseColor;
      }
      return attenuation;
   }
   vec3 unit_direction = normalize(r.direction());
   auto t = 0.5f * (unit_direction.y + 1.0f);
   return (1.0f - t) * color(1, 1, 1) + t * color(0.5f, 0.7f, 1.0f);
}

color normalize_color(const color& c, int samples_per_pixel)
{
   // todo: implement me!
   float scale = 1.0f / samples_per_pixel;
   float r = std::min(0.999f, std::max(0.0f, c.r * scale));
   float g = std::min(0.999f, std::max(0.0f, c.g * scale));
   float b = std::min(0.999f, std::max(0.0f, c.b * scale));

   // apply gamma correction 
   r = sqrt(r);
   g = sqrt(g);
   b = sqrt(b);

   return color(r, g, b);
}

void ray_trace(ppm_image& image)
{
   // Image
   int height = image.height();
   int width = image.width();
   float aspect = width / float(height);
   int samples_per_pixel = 10; // higher => more anti-aliasing
   int max_depth = 10; // higher => less shadow acne

   // Camera
   vec3 camera_pos(0, 0, 6);
   float viewport_height = 2.0f;
   float focal_length = 4.0; 
   //camera cam(point3(3, 0, 0), point3(-3, 0, 0), vec3(0, 1, 0), 50, aspect); // Changing the camera position using look at and up vectors
   camera cam(camera_pos, viewport_height, aspect, focal_length);

   // World
   shared_ptr<material> gray = make_shared<lambertian>(color(0.5f));
   shared_ptr<material> matteGreen = make_shared<lambertian>(color(0, 0.5f, 0));
   shared_ptr<material> metalRed = make_shared<metal>(color(1, 0, 0), 0.3f);
   shared_ptr<material> glass = make_shared<dielectric>(1.5f);
   shared_ptr<material> phongDefault = make_shared<phong>(camera_pos);
   
   hittable_list world;
   world.add(make_shared<sphere>(point3(-2.25, 0, -1), 0.5f, phongDefault));
   world.add(make_shared<sphere>(point3(-0.75, 0, -1), 0.5f, glass));
   world.add(make_shared<sphere>(point3(2.25, 0, -1), 0.5f, metalRed));
   world.add(make_shared<sphere>(point3(0.75, 0, -1), 0.5f, matteGreen));
   world.add(make_shared<sphere>(point3(0, -100.5, -1), 100, gray));
   
   // Ray trace
   for (int j = 0; j < height; j++)
   {
      for (int i = 0; i < width; i++)
      {
         color c(0, 0, 0);
         for (int s = 0; s < samples_per_pixel; s++) // antialias
         {
            float u = float(i + random_float()) / (width - 1);
            float v = float(height - j - 1 - random_float()) / (height - 1);

            ray r = cam.get_ray(u, v);
            c += ray_color(r, world, max_depth);
         }
         c = normalize_color(c, samples_per_pixel);
         image.set_vec3(j, i, c);
      }
   }

   image.save("../materials.png");
   //image.save("../camera-changed-materials.png");
}
/*
// Tests and unique image
color ray_color(const ray& r, const hittable_list& world, int depth)
{
    hit_record rec;
    if (depth <= 0)
    {
        return color(0);
    }

    if (world.hit(r, 0.001f, infinity, rec))
    {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        {
            color recurseColor = ray_color(scattered, world, depth - 1);
            return attenuation * recurseColor;
        }
        return attenuation;
    }
    
    vec3 unit_direction = normalize(r.direction());
    auto t = 0.5f * (unit_direction.y + 1.0f);
    return (1.0f - t) * color(1, 1, 1) + t * color(0.5f, 0.7f, 1.0f);

    /* 
    // Changing background (gradient in z-direction)
    vec3 unit_direction = normalize(r.direction());
    auto t = 0.5f * (unit_direction.z + 1.0f);
    return (1.0f - t) * color(1, 1, 1) + t * color(0.5f, 1.0f, 0.4f);
    */
/*
}

color normalize_color(const color& c, int samples_per_pixel)
{
    // todo: implement me!
    float scale = 1.0f / samples_per_pixel;
    float r = std::min(0.999f, std::max(0.0f, c.r * scale));
    float g = std::min(0.999f, std::max(0.0f, c.g * scale));
    float b = std::min(0.999f, std::max(0.0f, c.b * scale));

    // apply gamma correction 
    r = sqrt(r);
    g = sqrt(g);
    b = sqrt(b);

    return color(r, g, b);
}

void ray_trace(ppm_image& image)
{
    // Image
    int height = image.height();
    int width = image.width();
    float aspect = width / float(height);
    int samples_per_pixel = 10; // higher => more anti-aliasing
    int max_depth = 10; // higher => less shadow acne

    // Camera
    vec3 camera_pos(0, 0, 6);
    float viewport_height = 2.0f;
    float focal_length = 4.0;
    //camera cam(point3(3, 0, 0), point3(-3, 0, 0), vec3(0, 1, 0), 50, aspect); // Changing the camera position using look at and up vectors
    //camera cam(camera_pos, viewport_height, aspect, focal_length);

    // World
    shared_ptr<material> gray = make_shared<lambertian>(color(0.5f));
    shared_ptr<material> matteGreen = make_shared<lambertian>(color(0, 0.5f, 0));
    shared_ptr<material> metalRed = make_shared<metal>(color(1, 0, 0), 0.3f);
    shared_ptr<material> glass = make_shared<dielectric>(1.5f);
    shared_ptr<material> phongDefault = make_shared<phong>(camera_pos);
    shared_ptr<material> base = make_shared<lambertian>(color(0.6f, 0.1f, 0.2f));

    hittable_list world;
    //world.add(make_shared<triangle>(point3(-2.25, 0, -1), point3(-0.75, -0.5, -1), point3(2.25, 1, -1), matteGreen)); //saved as triangle
    //camera cam(point3(1, 0, 1), point3(-1, 0, -1), vec3(0, 1, 0), 100, aspect);
    //world.add(make_shared<plane>(vec3(1, 1, 0), vec3(0, 0, 1), gray)); //saved as plane
 
    /*
    // Unique image 1 - Helical spheres from 2 viewpoints
    //camera cam(camera_pos, viewport_height, aspect, focal_length);
    camera cam(point3(3, 2, 0), point3(-3, -2, 0), vec3(0, 0, 1), 100, aspect);
    world.add(make_shared<sphere>(point3(0,0,0), 0.1f, metalRed));
    for (int i = -10; i <= 10; i += 1)
    {
        //world.add(make_shared<sphere>(point3(cos(i), sin(i), i), 0.25f, phongDefault));
        world.add(make_shared<sphere>(point3(2*cos(i), 2*sin(i), 0.5*i), 0.5f, phongDefault));
    }
    */
/*
    // Unique Image 2 - Platonic solid with triangles on a plane base
    camera cam(point3(1, 1, 0), point3(-1, -1, 0), vec3(0, 0, 1), 75, aspect);
   
    world.add(make_shared<plane>(vec3(-5, -5, -5), vec3(0.75, 0.5, -5), base)); //plane used as base for platonic solid
    world.add(make_shared<triangle>(point3(0, 0, 0), point3(-0.5, -0.5, 0.5), point3(-0.5, 0.5, -0.5), matteGreen));
    world.add(make_shared<triangle>(point3(0, 0, 0), point3(-0.5, -0.5, 0.5), point3(0.5, -0.5, -0.5), metalRed));
    world.add(make_shared<triangle>(point3(0, 0, 0), point3(0.5, -0.5, -0.5), point3(0.5, 0.5, -0.5), matteGreen));
    world.add(make_shared<triangle>(point3(0, 0, 0), point3(0.5, 0.5, -0.5), point3(-0.5, 0.5, -0.5), metalRed));
    world.add(make_shared<triangle>(point3(0, 0, 0), point3(-0.5, -0.5, -0.5), point3(-0.5, 0.5, 0.5), matteGreen));
    world.add(make_shared<triangle>(point3(0, 0, 0), point3(-0.5, -0.5, -0.5), point3(0.5, -0.5, 0.5), metalRed));
    world.add(make_shared<triangle>(point3(0, 0, 0), point3(0.5, -0.5, 0.5), point3(0.5, 0.5, 0.5), matteGreen));
    world.add(make_shared<triangle>(point3(0, 0, 0), point3(0.5, 0.5, 0.5), point3(-0.5, 0.5, 0.5), metalRed));
    
    // Ray trace
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            color c(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; s++) // antialias
            {
                float u = float(i + random_float()) / (width - 1);
                float v = float(height - j - 1 - random_float()) / (height - 1);

                ray r = cam.get_ray(u, v);
                c += ray_color(r, world, max_depth);
            }
            c = normalize_color(c, samples_per_pixel);
            image.set_vec3(j, i, c);
        }
    }

    image.save("../unique-image2.png");
}
*/