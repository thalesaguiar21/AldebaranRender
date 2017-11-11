#include "CoolToWarm.h"

CoolToWarm::CoolToWarm(void) {
	set_shader_value(0.f);
}

CoolToWarm::CoolToWarm(float value) {
	set_shader_value(value);
}

RGB CoolToWarm::GetColor(Ray r_, World world) {
  HitRecord rec;
	RGB ambient (1.0, 1.0, 1.0);
	if(world.HitAnything(r_, rec)) {
		RGB color (0,0,0);
		vector<Light*> lights = world.lights;
		for(int i = 0; i < lights.size(); i++) {
			if(lights[i]->IsIlluminating(rec.hit)){
				HitRecord tmp;
				// L
				auto lightRay = UnitVector(lights[i]->origin() - rec.hit);
				// V
				auto viewDir = UnitVector(r_.origin - rec.hit);
				// H
				auto halfWay = UnitVector(viewDir + lightRay);

				float difCoef = std::max(0.f, dot(rec.normal, lightRay));
				auto diffuse = rec.mat->prop().X() * difCoef * rec.mat->diffuse();
        float specCoef = std::max(0.f, dot(halfWay, rec.normal));
        auto specular = rec.mat->prop().Y() *specCoef * rec.mat->specular();
        float angle = fabs(dot(UnitVector(rec.normal), UnitVector(r_.Direction())));
        if(angle < 0.30){
          return RGB(0,0,0);
        }
        auto kw = (1.0 + dot(UnitVector(rec.normal), lightRay)) / 2.0;
        color += kw*diffuse + (1-kw)*specular;

				auto shadowRay =  lights[i]->GetShadowRay(rec.hit);
				if(!world.HitAnything(shadowRay, tmp)) {
          color += kw*diffuse + (1-kw)*specular;
				}
			}
		}
		return color;
	} else {
		Vector3 unitDirection = UnitVector(r_.Direction());
    float t = 0.5 * (unitDirection.Y() + 1.0);
    return (1.0 - t)*RGB(1.0, 1.0, 1.0) + t*RGB(0.5, 0.7, 1.0);
	}
}
