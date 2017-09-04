#idndef _DEPTHMAP_H_
#define _DEPTHMAP_H_

#include "Shader.h"

class DepthMap : public Shader {
	RGB getColor(const Ray & r_, Image img, Scene scene) override 
	{
		return RGB(0, 0, 0);
	}
};

#endif