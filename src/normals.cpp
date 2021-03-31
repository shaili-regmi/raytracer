#include "ray.h"
#include <iostream>
#include "ppm_image.h"
#include "AGLM.h"

using namespace glm;
using namespace agl;
using namespace std;


float hit_sphere(const ray& ray, const vec3& center, float radius)
{
	vec3 oc = ray.origin() - center;
	float a = dot(ray.direction(), ray.direction());
	float b = 2.0f * dot(oc, ray.direction());
	float c = dot(oc, oc) - radius * radius;

	float discriminant = b * b - 4 * a * c;
	if (discriminant < 0) return -1;
	else return (-b - sqrt(discriminant)) / 2 * a;
}


color ray_color(const ray& ray)
{
	point3 sphere_center = point3(0, 0, -1);
	float t1 = hit_sphere(ray, sphere_center, 0.5f);
	//if we hit sphere
	if (t1 > 0) {
		vec3 hit_point = ray.origin() + t1 * ray.direction();
		vec3 normal = normalize(hit_point - sphere_center);
		vec3 color = 0.5f * (normal + vec3(1));
		return color;
	}
	vec3 dir = normalize(ray.direction());
	float t2 = 0.5f * (dir.y + 1.0f);
	color c1(0.5f, 0.7f, 1.0f);
	color c2(1.0f);
	return c1 * (1 - t2) + c2 * t2;
}


void ray_trace(ppm_image& image)
{
	int height = image.height();
	int width = image.width();

	float aspect = width / (float)height;
	float world_height = 2.0f;
	float world_width = world_height * aspect;
	float focal_length = 1.0f;

	vec3 camera_pos = vec3(0);
	vec3 lower_left = camera_pos
		- 0.5f * vec3(world_width, world_height, 0)
		- vec3(0, 0, focal_length);
	for (int i = 0; i < height; i++) // rows
	{
		for (int j = 0; j < width; j++) // cols
		{
			float u = j / ((float)width - 1);
			float v = (height - i - 1) / ((float)height - 1);
			vec3 world_pos = lower_left +
				vec3(u * world_width, v * world_height, 0);
			color c = ray_color(ray(camera_pos, world_pos - camera_pos));
			image.set_vec3(i, j, c);
		}
	}
	image.save("normals.png");
}