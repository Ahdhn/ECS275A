
#include "Parser.h"
#include "PinholeCamera.h"
#include "ThinLensCamera.h" 
#include "ConstantBackground.h"
#include "PointLight.h"
#include "LambertianMaterial.h"
#include "PhongMaterial.h"
#include "BVH.h"
#include "Group.h"
#include "Plane.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Polygon.h"
#include "Box.h"
#include "Scene.h"
#include "Image.h"
#include <cmath>
#include <iostream>
#include <istream>
#include <sstream>
#include <string>
#include <cstdlib>
using namespace std;

void Parser::throwParseException(string const &message) const
{
	cerr << next_token.line_number << ":" << next_token.column_number << ": " << message << endl;
	exit(1);
}

void Parser::readNextToken()
{
	int state = 0;
	long long mantissa = 0;
	int exponent = 0;
	int exponent_adjustment = 0;
	bool negative_mantissa = false;
	bool negative_exponent = false;
	for (;;) {
		int character = input.get();
		switch (state) {
		case 0:
			next_token.line_number = line_number;
			next_token.column_number = column_number;
			if (input.eof()) {
				next_token.token_type = Token::end_of_file;
				return;
			}
			else if (character == ' ' || character == '\t' ||
				character == '\r' || character == '\n')
				state = 0;
			else if (character == '/')
				state = 1;
			else if (character == '+' || character == '-') {
				negative_mantissa = character == '-';
				state = 3;
			}
			else if (character >= '0' && character <= '9') {
				mantissa = character - '0';
				state = 4;
			}
			else if (character == '.')
				state = 5;
			else if (character == '"') {
				next_token.string_value.clear();
				state = 10;
			}
			else if (character >= 'A' && character <= 'Z' ||
				character >= 'a' && character <= 'z' ||
				character == '_') {
				next_token.string_value = static_cast< char >(character);
				state = 12;
			}
			else if (character == ',') {
				++column_number;
				next_token.token_type = Token::comma;
				return;
			}
			else if (character == '{') {
				++column_number;
				next_token.token_type = Token::left_brace;
				return;
			}
			else if (character == '}') {
				++column_number;
				next_token.token_type = Token::right_brace;
				return;
			}
			else if (character == '[') {
				++column_number;
				next_token.token_type = Token::left_bracket;
				return;
			}
			else if (character == ']') {
				++column_number;
				next_token.token_type = Token::right_bracket;
				return;
			}
			else
				throwParseException("Unexpected character");
			break;
		case 1:
			if (character == '/')
				state = 2;
			else
				throwParseException("Malformed comment");
			break;
		case 2:
			if (character == '\n' || input.eof())
				state = 0;
			break;
		case 3:
			if (character >= '0' && character <= '9') {
				mantissa = character - '0';
				state = 4;
			}
			else if (character == '.')
				state = 5;
			else
				throwParseException("Invalid number");
			break;
		case 4:
			if (character >= '0' && character <= '9')
				mantissa = mantissa * 10 + character - '0';
			else if (character == '.')
				state = 6;
			else if (character == 'E' || character == 'e')
				state = 7;
			else {
				input.putback(static_cast< char >(character));
				next_token.integer_value = (static_cast< int >(mantissa)*
					(negative_mantissa ? -1 : 1));
				next_token.token_type = Token::integer;
				return;
			}
			break;
		case 5:
			if (character >= '0' && character <= '9') {
				mantissa = character - '0';
				exponent_adjustment = 1;
				state = 6;
			}
			else
				throwParseException("Invalid number");
			break;
		case 6:
			if (character >= '0' && character <= '9') {
				mantissa = mantissa * 10 + character - '0';
				++exponent_adjustment;
			}
			else if (character == 'E' || character == 'e')
				state = 7;
			else {
				input.putback(static_cast< char >(character));
				next_token.real_value = (mantissa * (negative_mantissa ? -1 : 1) *
					pow(10.0, -exponent_adjustment));
				next_token.token_type = Token::real;
				return;
			}
			break;
		case 7:
			if (character == '+' || character == '-') {
				negative_exponent = character == '-';
				state = 8;
			}
			else if (character >= '0' && character <= '9') {
				exponent = character - '0';
				state = 9;
			}
			else
				throwParseException("Invalid number");
			break;
		case 8:
			if (character >= '0' && character <= '9') {
				exponent = character - '0';
				state = 9;
			}
			else
				throwParseException("Invalid number");
			break;
		case 9:
			if (character >= '0' && character <= '9')
				exponent = exponent * 10 + character - '0';
			else {
				input.putback(static_cast< char >(character));
				next_token.real_value = (mantissa * (negative_mantissa ? -1 : 1) *
					pow(10.0, (exponent * (negative_exponent ? -1 : 1) -
					exponent_adjustment)));
				next_token.token_type = Token::real;
				return;
			}
			break;
		case 10:
			if (input.eof() || character == '\n')
				throwParseException("Unterminated string");
			else if (character == '\\')
				state = 11;
			else if (character == '"') {
				++column_number;
				next_token.token_type = Token::string;
				return;
			}
			else
				next_token.string_value.push_back(static_cast< char >(character));
			break;
		case 11:
			if (input.eof())
				throwParseException("Unterminated string");
			else if (character == '\n')
				next_token.string_value.push_back('\n');
			else if (character == '\\')
				next_token.string_value.push_back('\\');
			else if (character == '"')
				next_token.string_value.push_back('"');
			else
				next_token.string_value.push_back(static_cast< char >(character));
			state = 10;
			break;
		case 12:
			if (character >= '0' && character <= '9' ||
				character >= 'A' && character <= 'Z' ||
				character >= 'a' && character <= 'z' ||
				character == '_')
				next_token.string_value.push_back(static_cast< char >(character));
			else {
				input.putback(static_cast< char >(character));
				next_token.token_type = Token::string;
				return;
			}
			break;
		}
		if (character == '\n')
		{
			++line_number;
			column_number = 0;
		}
		else if (character == '\t')
			column_number = (column_number + 8) / 8 * 8;
		else
			++column_number;
	}
}

bool Parser::peek(Token::type const type)
{
	bool matched = next_token.token_type == type;
	if (matched)
		readNextToken();
	return matched;
}

bool Parser::peek(string const &keyword)
{
	bool matched = (next_token.token_type == Token::string && next_token.string_value == keyword);
	if (matched)
		readNextToken();
	return matched;
}

Parser::Token Parser::match(Token::type const type, string const &failure_message)
{
	if (next_token.token_type != type)
		throwParseException(failure_message);
	Token current_token(next_token);
	readNextToken();
	return current_token;
}

Parser::Token Parser::match(string const &keyword, string const &failure_message)
{
	if (next_token.token_type != Token::string ||
		next_token.string_value != keyword)
		throwParseException(failure_message);
	Token current_token(next_token);
	readNextToken();
	return current_token;
}

string Parser::parseString()
{
	Token next(match(Token::string, "Expected a string"));
	return next.string_value;
}

bool Parser::parseBoolean()
{
	if (peek("true"))
		return true;
	else if (peek("false"))
		return false;
	else
		throwParseException("Expected `true' or `false'.");
	return false;
}

int Parser::parseInteger()
{
	Token next(match(Token::integer, "Expected an integer"));
	return next.integer_value;
}

double Parser::parseReal()
{
	if (next_token.token_type == Token::integer) {
		Token next(match(Token::integer, "Expected an integer or real"));
		return static_cast< double >(next.integer_value);
	}
	Token next(match(Token::real, "Expected an integer or real"));
	return next.real_value;
}

Vector const Parser::parseVector()
{
	match(Token::left_bracket, "Expected a left bracket");
	double x = parseReal();
	match(Token::comma, "Expected a comma");
	double y = parseReal();
	match(Token::comma, "Expected a comma");
	double z = parseReal();
	match(Token::right_bracket, "Expected a right bracket");
	return Vector(x, y, z);
}

Point const Parser::parsePoint()
{
	match(Token::left_bracket, "Expected a left bracket");
	double x = parseReal();
	match(Token::comma, "Expected a comma");
	double y = parseReal();
	match(Token::comma, "Expected a comma");
	double z = parseReal();
	match(Token::right_bracket, "Expected a right bracket");
	return Point(x, y, z);
}

Color const Parser::parseColor()
{
	if (peek(Token::left_bracket)) {
		double r = parseReal();
		match(Token::comma, "Expected a comma");
		double g = parseReal();
		match(Token::comma, "Expected a comma");
		double b = parseReal();
		match(Token::right_bracket, "Expected a right bracket");
		return Color(float(r), float(g), float(b));
	}
	double v = parseReal();
	return Color(float(v), float(v), float(v));
}

Camera *Parser::parsePinholeCamera()
{
	Point eye(0.0, 0.0, 0.0);
	Point lookat(0.0, 1.0, 0.0);
	Vector up(0.0, 0.0, 1.0);
	double hfov = 90.0;
	if (peek(Token::left_brace))
		for (;;)
		{
			if (peek("eye"))
				eye = parsePoint();
			else if (peek("lookat"))
				lookat = parsePoint();
			else if (peek("up"))
				up = parseVector();
			else if (peek("hfov"))
				hfov = parseReal();
			else if (peek(Token::right_brace))
				break;
			else
				throwParseException("Expected `eye', `lookat', `up', `hfov' or }.");
		}
	return new PinholeCamera(eye, lookat, up, hfov);
}

Camera *Parser::parseThinLenCamera()
{
	Point eye(0.0, 0.0, 0.0);
	Point lookat(0.0, 1.0, 0.0);
	Vector up(0.0, 0.0, 1.0);
	double hfov(90.0);
	double len_radius = 0.3;
	double focus_dist = 1.0;
	if (peek(Token::left_brace)){
		for (;;){
			if (peek("eye")){
				eye = parsePoint();
			}
			else if (peek("lookat")){
				lookat = parsePoint();
			}
			else if (peek("up")){
				up = parseVector();
			}
			else if (peek("hfov")){
				hfov = parseReal();
			}
			else if (peek("lensradius")){
				len_radius = parseReal();
			}
			else if (peek("focus")){
				focus_dist = parseReal();
			}
			else if (peek(Token::right_brace)){
				break;
			}
			else {
				throwParseException("Expected `eye', `lookat', `up', `hfov', `len_radius', `focus_dist' or }.");
			}
		}
	}
	return new ThinLensCamera(eye, lookat, up, hfov, len_radius, focus_dist);
}

Camera *Parser::parseCamera()
{
	if (peek("pinhole"))
		return parsePinholeCamera();
	else if (peek("thinlens"))
		return parseThinLenCamera();
	else 
		throwParseException("Expected a camera type.");

	return 0;
}

Background *Parser::parseConstantBackground()
{
	Color color(0.0, 0.0, 0.0);
	if (peek(Token::left_brace))
		for (;;)
		{
			if (peek("color"))
				color = parseColor();
			else if (peek(Token::right_brace))
				break;
			else
				throwParseException("Expected `color' or }.");
		}
	return new ConstantBackground(color);
}

Background *Parser::parseBackground()
{
	if (peek("constant"))
		return parseConstantBackground();
	throwParseException("Expected a background type.");
	return 0;
}

Light *Parser::parsePointLight()
{
	Point position(0.0, 0.0, 10.0);
	Color color(1.0, 1.0, 1.0);
	if (peek(Token::left_brace))
		for (;;)
		{
			if (peek("position"))
				position = parsePoint();
			else if (peek("color"))
				color = parseColor();
			else if (peek(Token::right_brace))
				break;
			else
				throwParseException("Expected `position', `color' or }.");
		}
	return new PointLight(position, color);
}

Light *Parser::parseLight()
{
	if (peek("point"))
		return parsePointLight();
	throwParseException("Expected a light type.");
	return 0;
}

Material *Parser::parseLambertianMaterial()
{
	Color color(1.0, 1.0, 1.0);
	double Kd = 0.6;
	double Ka = 0.3;
	if (peek(Token::left_brace))
		for (;;)
		{
			if (peek("color"))
				color = parseColor();
			else if (peek("Kd"))
				Kd = parseReal();
			else if (peek("Ka"))
				Ka = parseReal();
			else if (peek(Token::right_brace))
				break;
			else
				throwParseException("Expected `color', `Kd', `Ka' or }.");
		}
	return new LambertianMaterial(color, float(Kd), float(Ka));
}

Material *Parser::parsePhongMaterial()
{
	Color color(1.0, 1.0, 1.0);
	double Kd = 0.6;
	double Ka = 0.3;
	double Kr = 0.5;
	Color highlight(1.0, 1.0, 1.0);
	int exponent = 100;
	if (peek(Token::left_brace))
		for (;;) {
			if (peek("color"))
				color = parseColor();
			else if (peek("Ka"))
				Ka = parseReal();
			else if (peek("Kd"))
				Kd = parseReal();			
			else if (peek("exponent"))
				exponent = parseInteger();
			else if (peek("Kr"))
				Kr = parseReal();
			else if (peek(Token::right_brace))
				break;
			else
				throwParseException("Expected `color', `Kd', `Ka', `highlight', `exponent' or }.");
		}	
	return new PhongMaterial(color, Ka, Kd, exponent, Kr);
}

Material *Parser::parseMaterial()
{
	if (peek("lambertian"))
		return parseLambertianMaterial();
	else if (peek("phong"))
		return parsePhongMaterial();	
	else if (next_token.token_type == Token::string)
	{
		map< string, Material * >::iterator found = defined_materials.find(parseString());
		if (found != defined_materials.end())
			return (*found).second;
	}
	throwParseException("Expected an material type.");
	return 0;
}

Object *Parser::parseGroupObject()
{
	Group *group = new Group();
	match(Token::left_brace, "Expected a left brace");
	while (!peek(Token::right_brace))
		group->addObject(parseObject());
	return group;
}
Object *Parser::parsePlaneObject()
{
	Material *material = default_material;
	Vector normal(0.0, 0.0, 1.0);
	Point point(0.0, 0.0, 0.0);
	if (peek(Token::left_brace))
		for (;;)
		{
			if (peek("material"))
				material = parseMaterial();
			else if (peek("normal"))
				normal = parseVector();
			else if (peek("point"))
				point = parsePoint();
			else if (peek(Token::right_brace))
				break;
			else
				throwParseException("Expected `material', `point', `normal' or }.");
		}
	return new Plane(material, normal, point);
}
Object *Parser::parseSphereObject()
{
	Material *material = default_material;
	Point center(0.0, 0.0, 0.0);
	Vector direction(0.0, 0.0, 0.0);
	double speed = 0.0;
	double radius = 0.5;
	if (peek(Token::left_brace))
		for (;;)
		{
			if (peek("material"))
				material = parseMaterial();
			else if (peek("center"))
				center = parsePoint();
			else if (peek("radius"))
				radius = parseReal();
			else if (peek("direction"))
				direction = parseVector();
			else if (peek("speed"))
				speed = parseReal();
			else if (peek(Token::right_brace))
				break;
			else
				throwParseException("Expected `material', `center', `radius' or }.");
		}
	//return new Sphere(material, center, radius);
	return new Sphere(material, center, radius, direction, speed);
}
Object *Parser::parseTriangleObject()
{
	Material *material = default_material;
	Vector direction(0.0, 0.0, 0.0);
	double speed = 0.0;
	Point p1(1.0, 0.0, 0.0);
	Point p2(0.0, 1.0, 0.0);
	Point p3(0.0, 0.0, 1.0);

	if (peek(Token::left_brace))
		for (;;){
			if (peek("material"))
				material = parseMaterial();
			else if (peek("corner1"))
				p1 = parsePoint();
			else if (peek("corner2"))
				p2 = parsePoint();
			else if (peek("corner3"))
				p3 = parsePoint();
			else if (peek("direction"))
				direction = parseVector();
			else if (peek("speed"))
				speed = parseReal();
			else if (peek(Token::right_brace))
				break;
			else
				throwParseException("Expected `material', `corner1', `corner2', `corner3' or }.");
		}
	//return new Triangle(material, p1, p2, p3);
	return new Triangle(material, p1, p2, p3, direction, speed);
}
Object *Parser::parsePolygonObject()
{
	Material *material = default_material;	
	std::vector<Point> point_list;	
	Vector direction(0.0, 0.0, 0.0);	
	bool is_luminous = false;
	double speed = 0.0;
	if (peek(Token::left_brace))
		for (;;){
			if (peek("material"))
				material = parseMaterial();					
			else if (peek("luminous"))
				is_luminous = parseBoolean();
			else if (peek("points"))
			{
				int num = parseInteger();
				while (num--)
					point_list.push_back(parsePoint());
			}		
			else if (peek("direction"))
				direction = parseVector();
			else if (peek("speed"))
				speed = parseReal();
			else if (peek(Token::right_brace))
				break;
			else
				throwParseException("Expected `material', `points'");
		}
	//Primitive* ret = new Polygon(material, is_luminous, point_list); 
	Primitive* ret = new Polygon(material, is_luminous, point_list, direction, speed);
	return ret;
}
Object *Parser::parseBoxObject()
{
	Material*material = default_material;
	Point corner1(-0.5, -0.5, 0.0);
	Point corner2(0.5, 0.5, 1.0);
	if (peek(Token::left_brace))
		for (;;){
			if (peek("material"))
				material = parseMaterial();
			else if (peek("corner1"))
				corner1 = parsePoint();
			else if (peek("corner2"))
				corner2 = parsePoint();
			else if (peek(Token::right_brace))
				break;
			else
				throwParseException("Expected `material', `corner1', `corner2' or }.");
		}
	return new Box(material, corner1, corner2);
}
Object *Parser::parseBVHObject()
{
	BVH *bvh = new BVH();
	match(Token::left_brace, "Expected a left brace");
	while (!peek(Token::right_brace))
		bvh->addObject(parseObject());
	return bvh;
}

/*Object *Parser::parsePLYObject()
{
	Material*material = default_material;
	string filename = "";
	if (peek(Token::left_brace)){
		for (;;){
			if (peek("material"))
				material = parseMaterial();
			else if (peek("filename"))
				filename = parseString();
			else if (peek(Token::right_brace))
				break;
			else
				throwParseException("Expected `filename' or }.");
		}
	}
	
	
	//return nullptr;
	//return new PLY(material, filename);
}*/

Object *Parser::parseObject()
{
	if (peek("group"))
		return parseGroupObject();
	else if (peek("plane"))
		return parsePlaneObject();
	else if (peek("sphere"))
		return parseSphereObject();
	else if (peek("triangle"))
		return parseTriangleObject();
	else if (peek("polygon"))
		return parsePolygonObject();
	else if (peek("box"))
		return parseBoxObject();
	else if (peek("ply"))
		double tuna = 0;
		//return parsePLYObject();
	else if (peek("bvhgroup"))
		return parseBVHObject();
	else if (peek("instance"))
		double tuna = 0;
	//	return parseInstanceObject();
	else if (next_token.token_type == Token::string)
	{
		map< string, Object * >::iterator found = defined_objects.find(parseString());
		if (found != defined_objects.end())
			return (*found).second;
	}
	throwParseException("Expected an object type.");
	return 0;
}

Parser::Parser(istream &input) : input(input),	line_number(1),	column_number(0), default_material(new LambertianMaterial(Color(1.0, 1.0, 1.0), 0.6, 0.3))
{
	readNextToken();
}

Scene *Parser::parseScene(string &filename)
{
	filename = "image.ppm";
	int xres = 512;
	int yres = 512;
	Scene *scene = new Scene();
	for (;;) {
		if (peek("filename"))
			filename = parseString();
		else if (peek("xres"))
			xres = parseInteger();
		else if (peek("yres"))
			yres = parseInteger();
		else if (peek("maxraydepth"))
			scene->setMaxRayDepth(parseInteger());
		else if (peek("minattenuation"))
			scene->setMinAttenuation(parseReal());
		else if (peek("pixelsamplingfrequency"))
			scene->setPixelSamplingFrequency(parseInteger());
		else if (peek("lenssamplingfrequency"))
			scene->setLensSamplingFrequency(parseInteger());
		else if (peek("timesamplingfrequency"))
			scene->setTimeSamplingFrequency(parseInteger());
		else if (peek("samplingfrequency"))
			scene->setSamplingFequency(parseInteger());
		else if (peek("pathtracingfrequency"))
			scene->setPathTracingFrequency(parseInteger());
		else if (peek("shutter"))
			scene->setShutter(parseReal());
		else if (peek("camera"))
			scene->setCamera(parseCamera());
		else if (peek("background"))
			scene->setBackground(parseBackground());
		else if (peek("ambient"))
			scene->setAmbient(parseColor());
		else if (peek("light"))
			scene->addLight(parseLight());
		else if (peek("scene"))
			scene->setObject(parseObject());
		else if (peek("define")) {
			if (peek("material")) {
				string name(parseString());
				defined_materials.insert(pair< string, Material * >(name, parseMaterial()));
			}
			else if (peek("object")) {
				string name(parseString());
				Object* obj = parseObject();
				obj->preprocess();
				//defined_objects.insert(pair< string, Object * >(name, parseObject()));
				defined_objects.insert(pair< string, Object * >(name, obj));
			}
			else{
				throwParseException("Expected `material', or `object'");
			}
		}
		else if (peek(Token::end_of_file)){
			break;
		}
		else{
			throwParseException("Expected `filename', `xres', `yres', `maxraydepth', `minattenuation', "
				"`camera', `background', `ambient', `light', `scene'"
				"`pixelsamplingfrequency', `lenssamplingfrequency', `timesamplingfrequency', `samplingfrequency', `pathtracingfrequency', or `define'.");

		}
	}

	scene->setImage(new Image(xres, yres));
	return scene;
}

