#ifndef PLY_h
#define PLY_h

#include "BVH.h"
#include "Triangle.h"
#include "Material.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

class PLY : public BVH
{
public:
	PLY(Material *mat, std::string filename) :myMat(mat), myFilename(filename){}

		
	virtual  void preprocess()
	{
		std::ifstream ifile(myFilename.c_str(), std::ifstream::in);
		if (ifile.is_open()){
			int vertexCount = 0, faceCount = 0, propCount = 0, xidx = -1, yidx = -1, zidx = -1, nxidx = -1, nyidx = -1, nzidx = -1;
			std::string keyword;
			while (keyword != "end_header"){
				ifile >> keyword;
				if (keyword == "vertex"){
					ifile >> vertexCount;
					while (keyword != "element"){
						if (keyword == "property"){
							ifile >> keyword; // property type
							ifile >> keyword; // property name
							if (keyword == "x"){ xidx = propCount; }
							else if (keyword == "y"){ yidx = propCount; }
							else if (keyword == "z"){ zidx = propCount; }
							else if (keyword == "nx"){ nxidx = propCount; }
							else if (keyword == "ny"){ nyidx = propCount; }
							else if (keyword == "nz"){ nzidx = propCount; }
							propCount++;
						}
						ifile >> keyword;
					}
				}
				else if (keyword == "face"){
					ifile >> faceCount;
				}
			}
			bool vertNormals = false;
			if ((nxidx >= 0) && (nyidx >= 0) && (nzidx >= 0)){
				vertNormals = true;
			}
			std::vector< std::vector<float> > verts;
			verts.resize(vertexCount);
			for (int i = 0; i < vertexCount; i++){
				verts[i].resize(propCount);
				for (int j = 0; j < propCount; j++)
					ifile >> verts[i][j];
			}
			for (int i = 0; i < faceCount; i++){
				int faceSize;
				ifile >> faceSize;

				// Create triangles
				if (faceSize == 3){
					Point pList[6];
					for (int j = 0; j < 3; j++){
						int v;
						ifile >> v;

						pList[j] = Point(verts[v][xidx], verts[v][yidx], verts[v][zidx]);

						if (vertNormals)
							pList[j + 3] = Point(verts[v][nxidx], verts[v][nyidx], verts[v][nzidx]);
					}

					if (vertNormals){
						objects.push_back(new Triangle(myMat, pList[0], pList[1], pList[2], pList[3], pList[4], pList[5]));
					}
					else{
						objects.push_back(new Triangle(myMat, pList[0], pList[1], pList[2]));
					}
				}
			}

			ifile.close();
		}
		else{
			std::cout << "Unable to open " << myFilename << std::endl;
		}

		BVH::preprocess();
	}


private:
	Material *myMat;
	std::string myFilename;

};

#endif // !PLY_h
