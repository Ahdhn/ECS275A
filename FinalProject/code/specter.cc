
#include "Background.h"
#include "Camera.h"
#include "ConstantBackground.h"
#include "Color.h"
#include "Group.h"
#include "Image.h"
#include "LambertianMaterial.h"
#include "Light.h"
#include "Material.h"
#include "Object.h"
#include "PinholeCamera.h"
#include "Plane.h"
#include "PointLight.h"
#include "Ray.h"
#include "Scene.h"
#include "Sphere.h"
#include "Time.h"
#include "Parser.h"

#include <iomanip>
#include <iostream>
#include <fstream>
#include <pthread.h>

using namespace std;

#include <fstream>
#include <sstream>

void prepInput()
{
	/*std::ifstream file;
	file.open("C:/Users/Ahmed/Documents/GitHub/ECS275A/FinalProject/code/scenes/bunny.node", std::ifstream::in);
	int num_tri = 499;
	std::string blah;

	std::fstream file_out("bunny.obj", std::ios::out);


	for (int i = 0; i < num_tri; i++){
		for (int j = 0; j < 3; j++){
			double v1, v2, v3;
			
			file >> v1;
			
			file >> v2;
			
			file >> v3;
			

			file_out << "v " << v1 << " " << v2 << " " << v3 << std::endl;
		}
	}

	for (int i = 1; i <=num_tri; i++){
		file_out << "f " << i * 3 << " " << i * 3 - 1 << " " << i * 3 - 2 << std::endl;
	}*/

	std::ifstream file_in;
	file_in.open("armadillo.obj", std::ifstream::in);
	char tuna[50];
	int num_vert, num_tri;
	file_in >> tuna >> num_vert;
	file_in >> tuna >> num_tri;

	double**vert = new double*[num_vert];
	for (int i = 0; i < num_vert; i++){
		vert[i] = new double[3];
		file_in >> tuna >> vert[i][0] >> vert[i][1] >> vert[i][2];
	}

	std::fstream file_out("armadillo.node", std::ios::out);
	for (int i = 0; i < num_tri; i++){
		int v1, v2, v3;
		file_in >> tuna >> v1 >> v2 >> v3;
		v1 -= 1; v2 -= 1; v3 -= 1;

		file_out << "triangle {material bunny corner1[" << vert[v1][0] << "," << vert[v1][1] << "," << vert[v1][2] << "]";
		file_out << "corner2[" << vert[v2][0] << "," << vert[v2][1] << "," << vert[v2][2] << "]"; 
		file_out << "corner3[" << vert[v3][0] << "," << vert[v3][1] << "," << vert[v3][2] << "]}" << std::endl;
			
	}

}

int main(int argc, char** argv)
{
	//prepInput();

	double t1 = Time::currentSeconds();
	argc = 2;
		
	
	argv[1] = "C:/Users/Ahmed/Documents/GitHub/ECS275A/FinalProject/code/scenes/bunny3.scn";
	//argv[1] = "C:/Users/Ahmed/Documents/GitHub/ECS275A/FinalProject/code/scenes/teapot.scn";
	//argv[1] = "C:/Users/Ahmed/Documents/GitHub/ECS275A/FinalProject/code/scenes/spiral.scn";
	//argv[1] = "C:/Users/Ahmed/Documents/GitHub/ECS275A/FinalProject/code/scenes/armadillo.scn";
	
	

	if (argc < 2) {
		cerr << "Usage: specter scene" << endl;
		return 1;
	}
	ifstream scene_file(argv[1]);
	if (!scene_file.good()) {
		cerr << "Unable to read scene file: " << argv[1] << endl;
		return 1;
	}
	Parser reader(scene_file);

	string filename = "image.ppm";

	Scene *scene = reader.parseScene(filename);

	scene->preprocess();
	scene->getObject()->preprocess();

	double t2 = Time::currentSeconds();


	scene->render();
	
	double t3 = Time::currentSeconds();

	scene->getImage()->write(filename); 

	double t4 = Time::currentSeconds();
	cerr << "Setup/load time:\t" << setprecision(3) << t2 - t1 << " seconds\n";
	cerr << "Render time:\t\t" << setprecision(3) << t3 - t2 << " seconds\n";
	cerr << "Post-process time:\t" << setprecision(3) << t4 - t3 << " seconds\n";
	return 0;
}
