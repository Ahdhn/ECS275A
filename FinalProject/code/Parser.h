
#ifndef Parser_h
#define Parser_h

#include "Vector.h"
#include "Point.h"
#include "Color.h"
#include <iostream>
#include <string>
#include <map>

class Camera;
class Background;
class Light;
class Material;
class Object;
class Scene;

class Parser {

	struct Token {
		enum type {
			end_of_file,
			integer, real, string,
			left_brace, right_brace,
			left_bracket, right_bracket,
			comma,
		};
		type token_type;
		std::string string_value;
		int integer_value;
		double real_value;
		int line_number;
		int column_number;
	};
	std::istream &input;
	int line_number;
	int column_number;
	Token next_token;
	Material *default_material;
	std::map< std::string, Material * > defined_materials;
	std::map< std::string, Object * > defined_objects;

	void throwParseException(std::string const &message) const;

	void readNextToken();
	bool peek(
		Token::type const type);
	bool peek(
		std::string const &keyword);
	Token match(
		Token::type const type,
		std::string const &failure_message);
	Token match(
		std::string const &keyword,
		std::string const &failure_message);

	std::string parseString();
	bool parseBoolean();
	int parseInteger();
	double parseReal();
	Vector const parseVector();
	Point const parsePoint();
	Color const parseColor();

	Camera *parsePinholeCamera();
	Camera *parseThinLenCamera();
	Camera *parseCamera();

	Background *parseConstantBackground();
	Background *parseBackground();

	Light *parsePointLight();
	Light *parseLight();

	Material *parseLambertianMaterial(); 
	Material *parsePhongMaterial();
	Material *parseMetalMaterial();
	Material *parseMaterial();

	Object *parseGroupObject();
	Object *parsePlaneObject();
	Object *parseSphereObject();
	Object *parseTriangleObject();
	Object *parsePolygonObject();
	Object *parseBoxObject();
	Object *parseObject();

	Object *parseBVHObject();
	Object *parseInstanceObject();
	Object *parsePLYObject();	

public:

	Parser(
		std::istream &input);

	Scene *parseScene(
		std::string &filename);

};

#endif
