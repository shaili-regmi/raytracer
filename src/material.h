#ifndef MATERIAL_H
#define MATERIAL_H

#include <cmath>
#include "AGLM.h"
#include "ray.h"
#include "hittable.h"

class material {
public:
  virtual bool scatter(const ray& r_in, const hit_record& rec, 
     glm::color& attenuation, ray& scattered) const = 0;
  virtual ~material() {}
};

class lambertian : public material {
public:
  lambertian(const glm::color& a) : albedo(a) {}

  virtual bool scatter(const ray& r_in, const hit_record& rec, 
     glm::color& attenuation, ray& scattered) const override 
  {
     // todo

      using namespace glm;
      
      // lambertian version 2
      vec3 scatter_direction = rec.p + rec.normal + random_unit_vector();
      if (near_zero(scatter_direction)) scatter_direction = rec.normal;
      scattered = ray(rec.p, scatter_direction - rec.p);
      attenuation = albedo;
      return true; // bounce

      /* // lambertian version 1
      vec3 unitn = normalize(rec.normal);
      vec3 lightDir = normalize(vec3(5, 5, 0) - rec.p);
      color diffuse = max(vec3(0), dot(unitn, lightDir)) * albedo;
      
      attenuation = diffuse;
      return false;
      */
  }

public:
  glm::color albedo;
};

class phong : public material {
public:
  phong(const glm::vec3& view) :
     diffuseColor(0,0,1), 
     specColor(1,1,1),
     ambientColor(.01f, .01f, .01f),
     lightPos(5,5,0),
     viewPos(view), 
     kd(0.45), ks(0.45), ka(0.1), shininess(10.0)
  {}

  phong(const glm::color& idiffuseColor, 
        const glm::color& ispecColor,
        const glm::color& iambientColor,
        const glm::point3& ilightPos, 
        const glm::point3& iviewPos, 
        float ikd, float iks, float ika, float ishininess) :
     diffuseColor(idiffuseColor), 
     specColor(ispecColor),
     ambientColor(iambientColor),
     lightPos(ilightPos),
     viewPos(iviewPos), kd(ikd), ks(iks), ka(ika), shininess(ishininess)
  {}

  virtual bool scatter(const ray& r_in, const hit_record& hit, 
     glm::color& attenuation, ray& scattered) const override 
  {
     // todo
      using namespace glm;
      color final;
      color ambience;
      color diffuse;
      color specular;
      vec3 unitn = normalize(hit.normal);
      vec3 lightDir = normalize(lightPos - hit.p);
      float l_dot_n = dot(lightDir, unitn);
      
      if (l_dot_n >= 0)
      {
          ambience = ka * ambientColor;
          diffuse = kd * l_dot_n * diffuseColor;
          vec3 r = 2 * l_dot_n * unitn - lightDir;
          float v_dot_r = dot(normalize(viewPos), normalize(r));
          if (v_dot_r >= 0)
          {
              specular = ks * specColor * pow(v_dot_r, shininess);
              final = ambience + diffuse + specular;
              attenuation = final;
          }
      }
      return false; // single ray; no bounce
  }

public:
  glm::color diffuseColor;
  glm::color specColor;
  glm::color ambientColor;
  glm::point3 lightPos;
  glm::point3 viewPos; 
  float kd; 
  float ks;
  float ka; 
  float shininess;
};

class metal : public material {
public:
   metal(const glm::color& a, float f) : albedo(a), fuzz(glm::clamp(f,0.0f,1.0f)) {}

   virtual bool scatter(const ray& r_in, const hit_record& rec, 
      glm::color& attenuation, ray& scattered) const override 
   {
     // todo
       glm::vec3 reflected = reflect(normalize(r_in.direction()), rec.normal);
       scattered = ray(rec.p, reflected + fuzz * random_unit_sphere());
       attenuation = albedo;
       return (dot(scattered.direction(), rec.normal) > 0);
   }

public:
   glm::color albedo;
   float fuzz;
};

class dielectric : public material {
public:
  dielectric(float index_of_refraction) : ir(index_of_refraction) {}

  virtual bool scatter(const ray& r_in, const hit_record& rec, 
     glm::color& attenuation, ray& scattered) const override 
   {
     // todo

      attenuation = glm::color(1.0, 1.0, 1.0);
      float refraction_ratio = rec.front_face ? (1.0f / ir) : ir;

      glm::vec3 unit_direction = normalize(r_in.direction());
      glm::vec3 refracted = refract(unit_direction, rec.normal, refraction_ratio);

      scattered = ray(rec.p, refracted);
      return true;
   }

public:
  float ir; // Index of Refraction
};


#endif

