#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>
#include <thread>
#include <time.h>

#include "utility/Vector3.h"
#include "utility/Ray.h"
#include "utility/Gamma.h"

#include "scene/lights/Light.h"
#include "scene/lights/SpotLight.h"
#include "scene/cameras/Camera.h"
#include "scene/cameras/ParallelCamera.h"
#include "scene/cameras/PerspectiveCamera.h"

#include "hitables/HitRecord.h"
#include "hitables/Hitable.h"
#include "hitables/Transformer.h"
#include "hitables/basic_shapes/Sphere.h"
#include "hitables/basic_shapes/Triangle.h"
#include "hitables/World.h"

#include "file_reader/FileUtils.h"
#include "file_reader/Image.h"
#include "file_reader/Reader.h"

#include "shaders/factory/ShaderFactory.h"
#include "shaders/factory/ShaderType.h"

#include "materials/Material.h"
#include "materials/Lambertian.h"
#include "materials/Metalic.h"
#include "materials/BlinnPhong.h"

#include "Renderer.h"
// vec3, vec4, ivec4, mat4
#include "external/glm/glm.hpp"
// translate, rotate, scale, perspective
#include "external/glm/gtc/matrix_transform.hpp"

using std::cout;
using std::endl;
using std::string;
using std::numeric_limits;

using namespace utils;
using namespace filerd;

int main( int argc, char *argv[] ) {

  if(argc < 2) {
    cout << "[ERROR] No input file was given!" << endl;
    return 1;
  } else {
    vector<string> input;

    //==== Checks if the file reading was sccessfull
    if(!ReadFile(argv[1], input)){
      //==== Checks if the content in the input file is correct
      Image img;
      if(img.FromContent(input)) {
        //==== Create the Materials
        Material *mat1 = new BlinnPhong(RGB(0.0, 0.0, 1.0), RGB(1.0, 1.0, 1.0),
                                        Vector3(0.5, 0.4, 0.1));

        Material *mat2 = new BlinnPhong(RGB(0.0, 1.0, 0.0), RGB(1.0, 1.0, 1.0), 
                                        Vector3(0.5, 1.0, 0.1));

        Material *mat3 = new BlinnPhong(RGB(1.0, 0.0, 1.0), RGB(1.0, 1.0, 1.0),
                                        Vector3(1.8, 1.0, 0.1));

        Material *mat4 = new BlinnPhong(RGB(0.5, 0.5, 0.5), RGB(1.0, 0.0, 1.0),
                                        Vector3(0.5, 1.0, 0.1));

        Material *mat5 = new BlinnPhong(RGB(1.0, 0.0, 0.0), RGB(1.0, 1.0, 1.0),
                                        Vector3(1.8, 1.0, 0.1));

        //==== Create the Camera
        // Perspective Camera
        float dist = (Point3(0,3,2) - Point3(0,0,-2)).Length();
        Camera *perspecCam = new PerspectiveCamera( Point3(0,3,2), Point3(0,0,0), 80, 
                              float(img.width)/float(img.height), 0, 5);

        // Parallel Camera
        Camera *orthoCam = new ParallelCamera( Point3(0,1,3), Point3(0,0,0),
                                           -4, 4, -2, 2);
        
        //==== Create the hitable objects
        Point3 center (0, 0, -1);
        glm::vec4 p1(0, 2, 0, 1);

        Point3 v1 (-2, 0, -2);
        Point3 v2 (2, 0, -2);
        Point3 v3 (0, 3, -2);

        Sphere *original = new Sphere(center, 0.5, mat1);
        Triangle *orig_triang = new Triangle(v1, v2, v3, mat2);

        std::vector<Hitable*> myHitables = {
          // orig_triang->Rotate(Vector3(0, 90, 0)),
          // orig_triang->Scale(Vector3(0.5, 0.5, 0.5)),
          // orig_triang
          original,
          original->Translate(Vector3(1,0,1)),
          original->Translate(Vector3(-1,0,1)),
          // new Sphere(Point3(centerT[0], centerT[1], centerT[2]), 0.5, mat2),
          // new Sphere(Point3(-2, 0, -2), 0.5, mat3),
          // new Sphere(Point3(2, 0, -2), 0.5, mat3),
          // new Sphere(Point3(0, 1, -2), 0.5, mat5),
          new Sphere(Point3(0, -100.5, -3), 100, mat4)
          };
        
        //==== Create the world lights
        std::vector<Light*> lights = {
          new Light(Point3(0, 0, 0), 10.0)
          // new SpotLight(Point3(2,2,-3), Vector3(-1,-1,0), 10, 1, 45)
          // new SpotLight(Point3(0,2,-2), Vector3(0,-1,0), 10, 1, 60)
        };

        //==== Create the Shader
        Shader *shader = ShaderFactory::Create(ShaderType::blinnPhong, 100.0);
        World world (myHitables,lights, 0.0001f, numeric_limits<float>::max());
        Renderer renderer = Renderer(img, perspecCam, world, shader);
        renderer.Start();

        //==== Write the result into a file
        WriteOnFile(img);
        
        // Unlocking memory
        delete shader;
        delete orthoCam;
        delete perspecCam;
        delete mat1;
        delete mat2;
        delete mat3;
        delete mat4;
        delete mat5;
        delete original;
        delete orig_triang;
      }
    }    
    return 0;
  }
}
