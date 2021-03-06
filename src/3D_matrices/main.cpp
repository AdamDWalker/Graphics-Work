// tag::C++11check[]
#define STRING2(x) #x
#define STRING(x) STRING2(x)

#if __cplusplus < 201103L
	#pragma message("WARNING: the compiler may not be C++11 compliant. __cplusplus version is : " STRING(__cplusplus))
#endif
// end::C++11check[]

// tag::includes[]
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <string>
#include <cassert>

#include <GL/glew.h>
#include <SDL2/SDL.h>

#define GLM_FORCE_RADIANS // suppress a warning in GLM 0.9.5
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
// end::includes[]

// tag::using[]
// see https://isocpp.org/wiki/faq/Coding-standards#using-namespace-std
// don't use the whole namespace, either use the specific ones you want, or just type std::
using std::cout;
using std::cerr;
using std::endl;
using std::max;
using std::string;
// end::using[]

void resetBall(bool isRedPoint);

// tag::globalVariables[]
std::string exeName;
SDL_Window *win; //pointer to the SDL_Window
SDL_GLContext context; //the SDL_GLContext
int frameCount = 0;
std::string frameLine = "";
// end::globalVariables[]

// tag::loadShader[]
std::string loadShader(const string filePath) {
    std::ifstream fileStream(filePath, std::ios::in | std::ios::binary);
	if (fileStream)
	{
		string fileData( (std::istreambuf_iterator<char>(fileStream)),
		                 (std::istreambuf_iterator<char>()          ));

		cout << "Shader Loaded from " << filePath << endl;
		return fileData;
	}
	else
	{
        cerr << "Shader could not be loaded - cannot read file " << filePath << ". File does not exist." << endl;
        return "";
	}
}
// end::loadShader[]

//our variables
bool done = false;

// tag::vertexData[]
//the data about our geometry
const GLfloat vertexData[] = {
#pragma region
	// ============================ Red + Black Paddle ===============================
	//	  X			Y      Z       R       G       B       A
		// Front Side
		-0.5f,	  0.25f,   -2.5f,   1.0f,   0.0f,   0.0f,   1.0f, // 1
		 0.5f,    0.25f,   -2.5f,   1.0f,   0.0f,   0.0f,   1.0f, // 2
		 0.5f,   -0.25f,   -2.5f,   1.0f,   0.0f,   0.0f,   1.0f, // 3 
		-0.5f,   -0.25f,   -2.5f,   1.0f,   0.0f,   0.0f,   1.0f, // 4
		-0.5f,	  0.25f,   -2.5f,   1.0f,   0.0f,   0.0f,   1.0f, // 1
		 0.5f,   -0.25f,   -2.5f,   1.0f,   0.0f,   0.0f,   1.0f, // 3

		// Back Side
		-0.5f,	  0.25f,   -2.3f,   1.0f,   0.0f,   0.0f,   1.0f, // 5
		 0.5f,    0.25f,   -2.3f,   1.0f,   0.0f,   0.0f,   1.0f, // 6
		 0.5f,   -0.25f,   -2.3f,   1.0f,   0.0f,   0.0f,   1.0f, // 7 
		-0.5f,   -0.25f,   -2.3f,   1.0f,   0.0f,   0.0f,   1.0f, // 8
		-0.5f,	  0.25f,   -2.3f,   1.0f,   0.0f,   0.0f,   1.0f, // 5
		 0.5f,   -0.25f,   -2.3f,   1.0f,   0.0f,   0.0f,   1.0f, // 7

		// Left Side
		-0.5f,	  0.25f,   -2.5f,   0.0f,   0.0f,   0.0f,   1.0f, // 1
		-0.5f,   -0.25f,   -2.5f,   0.0f,   0.0f,   0.0f,   1.0f, // 4
		-0.5f,   -0.25f,   -2.3f,   0.0f,   0.0f,   0.0f,   1.0f, // 8
		-0.5f,	  0.25f,   -2.5f,   0.0f,   0.0f,   0.0f,   1.0f, // 1
		-0.5f,   -0.25f,   -2.3f,   0.0f,   0.0f,   0.0f,   1.0f, // 8
		-0.5f,	  0.25f,   -2.3f,   0.0f,   0.0f,   0.0f,   1.0f, // 5

		// Right Side
		 0.5f,    0.25f,   -2.5f,   0.0f,   0.0f,   0.0f,   1.0f, // 2
		 0.5f,   -0.25f,   -2.5f,   0.0f,   0.0f,   0.0f,   1.0f, // 3
		 0.5f,   -0.25f,   -2.3f,   0.0f,   0.0f,   0.0f,   1.0f, // 7
		 0.5f,    0.25f,   -2.5f,   0.0f,   0.0f,   0.0f,   1.0f, // 2
		 0.5f,   -0.25f,   -2.3f,   0.0f,   0.0f,   0.0f,   1.0f, // 7
		 0.5f,    0.25f,   -2.3f,   0.0f,   0.0f,   0.0f,   1.0f, // 6

		// Top Side
		-0.5f,	  0.25f,   -2.5f,   1.0f,   0.0f,   0.0f,   1.0f, // 1
		 0.5f,    0.25f,   -2.5f,   1.0f,   0.0f,   0.0f,   1.0f, // 2
		-0.5f,	  0.25f,   -2.3f,   1.0f,   0.0f,   0.0f,   1.0f, // 5
		 0.5f,    0.25f,   -2.5f,   1.0f,   0.0f,   0.0f,   1.0f, // 2
		-0.5f,	  0.25f,   -2.3f,   1.0f,   0.0f,   0.0f,   1.0f, // 5
		 0.5f,    0.25f,   -2.3f,   1.0f,   0.0f,   0.0f,   1.0f, // 6

		// Bottom Side
		 0.5f,   -0.25f,   -2.5f,   1.0f,   0.0f,   0.0f,   1.0f, // 3 
		-0.5f,   -0.25f,   -2.5f,   1.0f,   0.0f,   0.0f,   1.0f, // 4
		-0.5f,   -0.25f,   -2.3f,   1.0f,   0.0f,   0.0f,   1.0f, // 8
		 0.5f,   -0.25f,   -2.5f,   1.0f,   0.0f,   0.0f,   1.0f, // 3 
		-0.5f,   -0.25f,   -2.3f,   1.0f,   0.0f,   0.0f,   1.0f, // 8
		 0.5f,   -0.25f,   -2.3f,   1.0f,   0.0f,   0.0f,   1.0f, // 7
	// ============================ End of Cube 1 ===============================
	#pragma endregion Red + Black Paddle

#pragma region
	// ============================ Blue + White paddle ===============================
	//	  X			Y      Z       R       G       B       A
	// Front Side
		-0.5f,	  0.25f,   2.3f,   0.0f,   0.0f,   1.0f,   1.0f, // 1
		 0.5f,    0.25f,   2.3f,   0.0f,   0.0f,   1.0f,   1.0f, // 2
		 0.5f,   -0.25f,   2.3f,   0.0f,   0.0f,   1.0f,   1.0f, // 3 
		-0.5f,   -0.25f,   2.3f,   0.0f,   0.0f,   1.0f,   1.0f, // 4
		-0.5f,	  0.25f,   2.3f,   0.0f,   0.0f,   1.0f,   1.0f, // 1
		 0.5f,   -0.25f,   2.3f,   0.0f,   0.0f,   1.0f,   1.0f, // 3

	// Back Side
		-0.5f,	  0.25f,   2.5f,   0.0f,   0.0f,   1.0f,   1.0f, // 5
		 0.5f,    0.25f,   2.5f,   0.0f,   0.0f,   1.0f,   1.0f, // 6
		 0.5f,   -0.25f,   2.5f,   0.0f,   0.0f,   1.0f,   1.0f, // 7 
		-0.5f,   -0.25f,   2.5f,   0.0f,   0.0f,   1.0f,   1.0f, // 8
		-0.5f,	  0.25f,   2.5f,   0.0f,   0.0f,   1.0f,   1.0f, // 5
 		 0.5f,   -0.25f,   2.5f,   0.0f,   0.0f,   1.0f,   1.0f, // 7

	// Left Side
		-0.5f,	  0.25f,   2.3f,   1.0f,   1.0f,   1.0f,   1.0f, // 1
		-0.5f,   -0.25f,   2.3f,   1.0f,   1.0f,   1.0f,   1.0f, // 4
		-0.5f,   -0.25f,   2.5f,   1.0f,   1.0f,   1.0f,   1.0f, // 8
		-0.5f,	  0.25f,   2.3f,   1.0f,   1.0f,   1.0f,   1.0f, // 1
		-0.5f,   -0.25f,   2.5f,   1.0f,   1.0f,   1.0f,   1.0f, // 8
		-0.5f,	  0.25f,   2.5f,   1.0f,   1.0f,   1.0f,   1.0f, // 5

	// Right Side
		 0.5f,    0.25f,   2.3f,   1.0f,   1.0f,   1.0f,   1.0f, // 2
		 0.5f,   -0.25f,   2.3f,   1.0f,   1.0f,   1.0f,   1.0f, // 3
		 0.5f,   -0.25f,   2.5f,   1.0f,   1.0f,   1.0f,   1.0f, // 7
		 0.5f,    0.25f,   2.3f,   1.0f,   1.0f,   1.0f,   1.0f, // 2
		 0.5f,   -0.25f,   2.5f,   1.0f,   1.0f,   1.0f,   1.0f, // 7
		 0.5f,    0.25f,   2.5f,   1.0f,   1.0f,   1.0f,   1.0f, // 6

	// Top Side
		-0.5f,	  0.25f,   2.3f,   0.0f,   0.0f,   1.0f,   1.0f, // 1
		 0.5f,    0.25f,   2.3f,   0.0f,   0.0f,   1.0f,   1.0f, // 2
		-0.5f,	  0.25f,   2.5f,   0.0f,   0.0f,   1.0f,   1.0f, // 5
		 0.5f,    0.25f,   2.3f,   0.0f,   0.0f,   1.0f,   1.0f, // 2
		-0.5f,    0.25f,   2.5f,   0.0f,   0.0f,   1.0f,   1.0f, // 5
		 0.5f,    0.25f,   2.5f,   0.0f,   0.0f,   1.0f,   1.0f, // 6

	// Bottom Side
		 0.5f,   -0.25f,   2.3f,   0.0f,   0.0f,   1.0f,   1.0f, // 3 
		-0.5f,   -0.25f,   2.3f,   0.0f,   0.0f,   1.0f,   1.0f, // 4
		-0.5f,   -0.25f,   2.5f,   0.0f,   0.0f,   1.0f,   1.0f, // 8
		 0.5f,   -0.25f,   2.3f,   0.0f,   0.0f,   1.0f,   1.0f, // 3 
		-0.5f,   -0.25f,   2.5f,   0.0f,   0.0f,   1.0f,   1.0f, // 8
		 0.5f,   -0.25f,   2.5f,   0.0f,   0.0f,   1.0f,   1.0f, // 7
	// ============================ End of Cube 2 ===============================
#pragma endregion Blue + White Paddle
};

const GLfloat boundsVertexData[] = {
#pragma region 
	// ============================= Left / Right Boundary ===========================
		-0.05f,  0.5f,  -3.0f, 0.0f, 0.0f, 0.0f, 1.0f, // 1
		 0.05f,  0.5f,  -3.0f, 0.0f, 0.0f, 0.0f, 1.0f, // 2
		 0.05f, -0.25f, -3.0f, 0.0f, 0.0f, 0.0f, 1.0f, // 3 
		-0.05f, -0.25f, -3.0f, 0.0f, 0.0f, 0.0f, 1.0f, // 4
		-0.05f,  0.5f,  -3.0f, 0.0f, 0.0f, 0.0f, 1.0f, // 1
		 0.05f, -0.25f, -3.0f, 0.0f, 0.0f, 0.0f, 1.0f, // 3

// Back Side
		-0.05f,  0.5f,  3.0f, 0.2f, 0.2f, 0.2f, 1.0f, // 5
		 0.05f,  0.5f,  3.0f, 0.2f, 0.2f, 0.2f, 1.0f, // 6
		 0.05f, -0.25f, 3.0f, 0.2f, 0.2f, 0.2f, 1.0f, // 7 
		-0.05f, -0.25f, 3.0f, 0.2f, 0.2f, 0.2f, 1.0f, // 8
		-0.05f,  0.5f,  3.0f, 0.2f, 0.2f, 0.2f, 1.0f, // 5
		 0.05f, -0.25f, 3.0f, 0.2f, 0.2f, 0.2f, 1.0f, // 7

// Left Side
		-0.05f,  0.5f,  -3.0f, 0.0f, 0.5f, 0.7f, 1.0f, // 1
		-0.05f, -0.25f, -3.0f, 0.0f, 0.5f, 0.7f, 1.0f, // 4
		-0.05f, -0.25f,  3.0f, 0.0f, 0.5f, 0.7f, 1.0f, // 8
		-0.05f,  0.5f,  -3.0f, 0.0f, 0.5f, 0.7f, 1.0f, // 1
		-0.05f, -0.25f,  3.0f, 0.0f, 0.5f, 0.7f, 1.0f, // 8
		-0.05f,  0.5f,   3.0f, 0.0f, 0.5f, 0.7f, 1.0f, // 5

// Right Side
		0.05f,  0.5f,  -3.0f, 0.0f, 0.5f, 0.7f, 1.0f, // 2
		0.05f, -0.25f, -3.0f, 0.0f, 0.5f, 0.7f, 1.0f, // 3
		0.05f, -0.25f,  3.0f, 0.0f, 0.5f, 0.7f, 1.0f, // 7
		0.05f,  0.5f,  -3.0f, 0.0f, 0.5f, 0.7f, 1.0f, // 2
		0.05f, -0.25f,  3.0f, 0.0f, 0.5f, 0.7f, 1.0f, // 7
		0.05f,  0.5f,   3.0f, 0.0f, 0.5f, 0.7f, 1.0f, // 6

// Top Side
		-0.05f, 0.5f, -3.0f, 0.0f, 0.0f, 0.0f, 1.0f, // 1
		 0.05f, 0.5f, -3.0f, 0.0f, 0.0f, 0.0f, 1.0f, // 2
		-0.05f, 0.5f,  3.0f, 0.0f, 0.0f, 0.0f, 1.0f, // 5
		 0.05f, 0.5f, -3.0f, 0.0f, 0.0f, 0.0f, 1.0f, // 2
		-0.05f, 0.5f,  3.0f, 0.0f, 0.0f, 0.0f, 1.0f, // 5
		 0.05f, 0.5f,  3.0f, 0.0f, 0.0f, 0.0f, 1.0f, // 6

// Bottom Side
		-0.05f, -0.25f, -3.0f, 0.0f, 0.0f, 0.0f, 1.0f, // 3 
		-0.05f, -0.25f, -3.0f, 0.0f, 0.0f, 0.0f, 1.0f, // 4
		-0.05f, -0.25f,  3.0f, 0.0f, 0.0f, 0.0f, 1.0f, // 8
		 0.05f, -0.25f, -3.0f, 0.0f, 0.0f, 0.0f, 1.0f, // 3 
		-0.05f, -0.25f,  3.0f, 0.0f, 0.0f, 0.0f, 1.0f, // 8
		 0.05f, -0.25f,  3.0f, 0.0f, 0.0f, 0.0f, 1.0f, // 7

	#pragma endregion Left / Right Boundary

#pragma region
// ============================= Top / Bottom Boundary ===========================
// Front Side
	-2.5f,  0.5f,  -0.1f,  0.2f,  0.2f,  0.2f, 1.0f, // 1
	 2.5f,  0.5f,  -0.1f,  0.2f,  0.2f,  0.2f, 1.0f, // 2
	 2.5f, -0.25f, -0.1f,  0.2f,  0.2f,  0.2f, 1.0f, // 3 
	-2.5f, -0.25f, -0.1f,  0.2f,  0.2f,  0.2f, 1.0f, // 4
	-2.5f,  0.5f,  -0.1f,  0.2f,  0.2f,  0.2f, 1.0f, // 1
	 2.5f, -0.25f, -0.1f,  0.2f,  0.2f,  0.2f, 1.0f, // 3

// Back Side
	-2.5f,  0.5f,  0.1f, 0.2f, 0.2f, 0.2f, 1.0f, // 5
	 2.5f,  0.5f,  0.1f, 0.2f, 0.2f, 0.2f, 1.0f, // 6
	 2.5f, -0.25f, 0.1f, 0.2f, 0.2f, 0.2f, 1.0f, // 7 
	-2.5f, -0.25f, 0.1f, 0.2f, 0.2f, 0.2f, 1.0f, // 8
	-2.5f,  0.5f,  0.1f, 0.2f, 0.2f, 0.2f, 1.0f, // 5
	 2.5f, -0.25f, 0.1f, 0.2f, 0.2f, 0.2f, 1.0f, // 7

 // Left Side
	-2.5f,  0.5f,  -0.1f, 0.0f, 0.5f, 0.7f, 1.0f, // 1
	-2.5f, -0.25f, -0.1f, 0.0f, 0.5f, 0.7f, 1.0f, // 4
	-2.5f, -0.25f,  0.1f, 0.0f, 0.5f, 0.7f, 1.0f, // 8
	-2.5f,  0.5f,  -0.1f, 0.0f, 0.5f, 0.7f, 1.0f, // 1
	-2.5f, -0.25f,  0.1f, 0.0f, 0.5f, 0.7f, 1.0f, // 8
	-2.5f,  0.5f,   0.1f, 0.0f, 0.5f, 0.7f, 1.0f, // 5

// Right Side
	2.5f,  0.5f,  -0.1f,  0.0f,  0.5f, 0.7f, 1.0f, // 2
	2.5f, -0.25f, -0.1f,  0.0f,  0.5f, 0.7f, 1.0f, // 3
	2.5f,  0.5f,   0.1f,  0.0f,  0.5f, 0.7f, 1.0f, // 7
	2.5f,  0.5f,  -0.1f,  0.0f,  0.5f, 0.7f, 1.0f, // 2
	2.5f, -0.25f,  0.1f,  0.0f,  0.5f, 0.7f, 1.0f, // 7
	2.5f,  0.5f,   0.1f,  0.0f,  0.5f, 0.7f, 1.0f, // 6

// Top Side
	-2.5f, 0.5f, -0.1f, 0.0f, 0.0f, 0.0f, 1.0f, // 1
	 2.5f, 0.5f, -0.1f, 0.0f, 0.0f, 0.0f, 1.0f, // 2
	-2.5f, 0.5f,  0.1f, 0.0f, 0.0f, 0.0f, 1.0f, // 5
	 2.5f, 0.5f, -0.1f, 0.0f, 0.0f, 0.0f, 1.0f, // 2
	-2.5f, 0.5f,  0.1f, 0.0f, 0.0f, 0.0f, 1.0f, // 5
	 2.5f, 0.5f,  0.1f, 0.0f, 0.0f, 0.0f, 1.0f, // 6

// Bottom Side
	-2.5f, -0.25f, -0.1f,  0.0f,  0.0f, 0.0f, 1.0f, // 3 
	-2.5f, -0.25f, -0.1f,  0.0f,  0.0f, 0.0f, 1.0f, // 4
	-2.5f, -0.25f,  0.1f,  0.0f,  0.0f, 0.0f, 1.0f, // 8
	 2.5f, -0.25f, -0.1f,  0.0f,  0.0f, 0.0f, 1.0f, // 3 
	-2.5f, -0.25f,  0.1f,  0.0f,  0.0f, 0.0f, 1.0f, // 8
  	 2.5f, -0.25f,  0.1f,  0.0f,  0.0f, 0.0f, 1.0f, // 7
#pragma endregion Top / Bottom Bounds
};

const GLfloat ballVertexData[] = {
#pragma region 

	//	  X			Y      Z       R       G       B       A
	// Front Side
		-0.1f,  0.1f, -0.1f, 0.0f, 1.0f, 0.0f, 1.0f, // 1
		 0.1f,  0.1f, -0.1f, 0.0f, 1.0f, 0.0f, 1.0f, // 2
		 0.1f, -0.1f, -0.1f, 0.0f, 1.0f, 0.0f, 1.0f, // 3 
		-0.1f, -0.1f, -0.1f, 0.0f, 1.0f, 0.0f, 1.0f, // 4
		-0.1f,  0.1f, -0.1f, 0.0f, 1.0f, 0.0f, 1.0f, // 1
		 0.1f, -0.1f, -0.1f, 0.0f, 1.0f, 0.0f, 1.0f, // 3

												// Back Side
		-0.1f,  0.1f, 0.1f, 1.0f, 1.0f, 1.0f, 1.0f, // 5
		 0.1f,  0.1f, 0.1f, 1.0f, 1.0f, 1.0f, 1.0f, // 6
		 0.1f, -0.1f, 0.1f, 1.0f, 1.0f, 1.0f, 1.0f, // 7 
		-0.1f, -0.1f, 0.1f, 1.0f, 1.0f, 1.0f, 1.0f, // 8
		-0.1f,  0.1f, 0.1f, 1.0f, 1.0f, 1.0f, 1.0f, // 5
		 0.1f, -0.1f, 0.1f, 1.0f, 1.0f, 1.0f, 1.0f, // 7

											   // Left Side
		-0.1f,  0.1f, -0.1f, 0.0f, 0.0f, 0.0f, 1.0f, // 1
		-0.1f, -0.1f, -0.1f, 0.0f, 0.0f, 0.0f, 1.0f, // 4
		-0.1f, -0.1f,  0.1f, 0.0f, 0.0f, 0.0f, 1.0f, // 8
		-0.1f,  0.1f, -0.1f, 0.0f, 0.0f, 0.0f, 1.0f, // 1
		-0.1f, -0.1f,  0.1f, 0.0f, 0.0f, 0.0f, 1.0f, // 8
		-0.1f,  0.1f,  0.1f, 0.0f, 0.0f, 0.0f, 1.0f, // 5

												 // Right Side
		 0.1f,  0.1f, -0.1f,  0.0f,  0.0f,  1.0f,  1.0f, // 2
		 0.1f, -0.1f, -0.1f,  0.0f,  0.0f,  1.0f,  1.0f, // 3
		 0.1f, -0.1f,  0.1f,  0.0f,  0.0f,  1.0f,  1.0f, // 7
		 0.1f,  0.1f, -0.1f,  0.0f,  0.0f,  1.0f,  1.0f, // 2
		 0.1f, -0.1f,  0.1f,  0.0f,  0.0f,  1.0f,  1.0f, // 7
		 0.1f,  0.1f,  0.1f,  0.0f,  0.0f,  1.0f,  1.0f, // 6

													// Top Side
		-0.1f,  0.1f,  -0.1f,  1.0f,  1.0f,  0.0f,  1.0f, // 1
		 0.1f,  0.1f,  -0.1f,  1.0f,  1.0f,  0.0f,  1.0f, // 2
		-0.1f,  0.1f,   0.1f,  1.0f,  1.0f,  0.0f,  1.0f, // 5
		 0.1f,  0.1f,  -0.1f,  1.0f,  1.0f,  0.0f,  1.0f, // 2
		-0.1f,  0.1f,   0.1f,  1.0f,  1.0f,  0.0f,  1.0f, // 5
		 0.1f,  0.1f,   0.1f,  1.0f,  1.0f,  0.0f,  1.0f, // 6

													 // Bottom Side
		 0.1f,  -0.1f, -0.1f,  1.0f,  0.0f,  0.0f,  1.0f, // 3 
		-0.1f,  -0.1f, -0.1f,  1.0f,  0.0f,  0.0f,  1.0f, // 4
		-0.1f,  -0.1f,  0.1f,  1.0f,  0.0f,  0.0f,  1.0f, // 8
		 0.1f,  -0.1f, -0.1f,  1.0f,  0.0f,  0.0f,  1.0f, // 3 
		-0.1f,  -0.1f,  0.1f,  1.0f,  0.0f,  0.0f,  1.0f, // 8
		 0.1f,  -0.1f,  0.1f,  1.0f,  0.0f,  0.0f,  1.0f, // 7

#pragma endregion Ball Data
};

const GLfloat scoreVertexData[] = {
#pragma region
	// Red score block
	-0.025f,	0.025f,   0.0f,   1.0f,   0.0f,   0.0f,   1.0f, // 1
	 0.025f,    0.025f,   0.0f,   1.0f,   0.0f,   0.0f,   1.0f, // 2
	 0.025f,   -0.025f,   0.0f,   1.0f,   0.0f,   0.0f,   1.0f, // 3 
	-0.025f,   -0.025f,   0.0f,   1.0f,   0.0f,   0.0f,   1.0f, // 4
	-0.025f,	0.025f,   0.0f,   1.0f,   0.0f,   0.0f,   1.0f, // 1
	 0.025f,   -0.025f,   0.0f,   1.0f,   0.0f,   0.0f,   1.0f, // 3

	// Blue score block
	-0.025f,	0.025f,   0.0f,   0.0f,   0.0f,   1.0f,   1.0f, // 1
	 0.025f,    0.025f,   0.0f,   0.0f,   0.0f,   1.0f,   1.0f, // 2
	 0.025f,   -0.025f,   0.0f,   0.0f,   0.0f,   1.0f,   1.0f, // 3 
	-0.025f,   -0.025f,   0.0f,   0.0f,   0.0f,   1.0f,   1.0f, // 4
	-0.025f,	0.025f,   0.0f,   0.0f,   0.0f,   1.0f,   1.0f, // 1
	 0.025f,   -0.025f,   0.0f,   0.0f,   0.0f,   1.0f,   1.0f, // 3
#pragma endregion Score Blocks
};

// end::vertexData[]

// tag::gameState[]
//the translation vector we'll pass to our GLSL program
// These are changed in update simulation, the velocity vectors are altered by keypress input to determine movement
glm::vec3 position1 = { 0.0f, 0.0f, 0.0f};
glm::vec3 velocity1 = { 0.0f, 0.0f, 0.0f};

glm::vec3 position2 = { 0.0f, 0.0f , 0.0f};
glm::vec3 velocity2 = { 0.0f, 0.0f, 0.0f };

glm::vec3 ballPosition = { 0.0f, 0.0f, 0.0f };
glm::vec3 ballVelocity = { 2.0f, 0.0f, 1.0f};

glm::vec3 boundPosition = { 0.0f, 0.0f , 0.0f };

glm::vec3 scorePosition = { 0.0f, 0.0f, 0.0f };
// end::gameState[]

// tag::GLVariables[]
//our GL and GLSL variables
//programIDs
GLuint theProgram; //GLuint that we'll fill in to refer to the GLSL program (only have 1 at this point)

//attribute locations
GLint positionLocation; //GLuint that we'll fill in with the location of the `position` attribute in the GLSL
GLint vertexColorLocation; //GLuint that we'll fill in with the location of the `vertexColor` attribute in the GLSL

//uniform location
GLint modelMatrixLocation;
GLint viewMatrixLocation;
GLint projectionMatrixLocation;

// These are for the bats
GLuint vertexDataBufferObject;
GLuint vertexArrayObject;

// These are for the Ball
GLuint vertexDataBufferObject2;
GLuint vertexArrayObject2;

// These are for the Bounds
GLuint vertexDataBufferObject3;
GLuint vertexArrayObject3;

// These are for the score
GLuint vertexDataBufferObject4;
GLuint vertexArrayObject4;

GLfloat rotateAngle = 1.0f;
GLint camView = 1; // This will determine which view the camera uses and will change on keypress
GLfloat speed = 3.0f; // This is here so that I can change the speed of the paddles easier, it also allows me to invert the keypress controls when tracking the opposite bat

glm::mat4 modelMatrix;

// Score tracking
bool isRedPoint; // Who got that point? Gets passed to the reset ball function
bool gameOver = false;
GLuint redScore = 0;
GLuint blueScore = 0;
// end::GLVariables[]


// end Global Variables
/////////////////////////

// tag::initialise[]
void initialise()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
		exit(1);
	}
	cout << "SDL initialised OK!\n";
}
// end::initialise[]

// tag::createWindow[]
void createWindow()
{
	//get executable name, and use as window title
	int beginIdxWindows = exeName.rfind("\\"); //find last occurrence of a backslash
	int beginIdxLinux = exeName.rfind("/"); //find last occurrence of a forward slash
	int beginIdx = max(beginIdxWindows, beginIdxLinux);
	std::string exeNameEnd = exeName.substr(beginIdx + 1);
	const char *exeNameCStr = exeNameEnd.c_str();

	//create window
	win = SDL_CreateWindow(exeNameCStr, 100, 100, 1000, 700, SDL_WINDOW_OPENGL); //same height and width makes the window square ...

	//error handling
	if (win == nullptr)
	{
		std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		exit(1);
	}
	cout << "SDL CreatedWindow OK!\n";
}
// end::createWindow[]

// tag::setGLAttributes[]
void setGLAttributes()
{
	int major = 3;
	int minor = 3;
	cout << "Built for OpenGL Version " << major << "." << minor << endl; //ahttps://en.wikipedia.org/wiki/OpenGL_Shading_Language#Versions
	// set the opengl context version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); //core profile
	cout << "Set OpenGL context to versicreate remote branchon " << major << "." << minor << " OK!\n";
}
// tag::setGLAttributes[]

// tag::createContext[]
void createContext()
{
	setGLAttributes();

	context = SDL_GL_CreateContext(win);
	if (context == nullptr){
		SDL_DestroyWindow(win);
		std::cerr << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		exit(1);
	}
	cout << "Created OpenGL context OK!\n";
}
// end::createContext[]

// tag::initGlew[]
void initGlew()
{
	GLenum rev;
	glewExperimental = GL_TRUE; //GLEW isn't perfect - see https://www.opengl.org/wiki/OpenGL_Loading_Library#GLEW
	rev = glewInit();
	if (GLEW_OK != rev){
		std::cerr << "GLEW Error: " << glewGetErrorString(rev) << std::endl;
		SDL_Quit();
		exit(1);
	}
	else {
		cout << "GLEW Init OK!\n";
	}
}
// end::initGlew[]

// tag::createShader[]
GLuint createShader(GLenum eShaderType, const std::string &strShaderFile)
{
	GLuint shader = glCreateShader(eShaderType);
	//error check
	const char *strFileData = strShaderFile.c_str();
	glShaderSource(shader, 1, &strFileData, NULL);

	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

		const char *strShaderType = NULL;
		switch (eShaderType)
		{
		case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
		case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
		case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
		}

		fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
		delete[] strInfoLog;
	}

	return shader;
}
// end::createShader[]

// tag::createProgram[]
GLuint createProgram(const std::vector<GLuint> &shaderList)
{
	GLuint program = glCreateProgram();

	for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glAttachShader(program, shaderList[iLoop]);

	glLinkProgram(program);

	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}

	for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glDetachShader(program, shaderList[iLoop]);

	return program;
}
// end::createProgram[]

// tag::initializeProgram[]
void initializeProgram()
{
	std::vector<GLuint> shaderList;

	shaderList.push_back(createShader(GL_VERTEX_SHADER, loadShader("vertexShader.glsl")));
	shaderList.push_back(createShader(GL_FRAGMENT_SHADER, loadShader("fragmentShader.glsl")));

	theProgram = createProgram(shaderList);
	if (theProgram == 0)
	{
		cerr << "GLSL program creation error." << std::endl;
		SDL_Quit();
		exit(1);
	}
	else {
		cout << "GLSL program creation OK! GLUint is: " << theProgram << std::endl;
	}

	// tag::glGetAttribLocation[]
	positionLocation = glGetAttribLocation(theProgram, "position");
	vertexColorLocation = glGetAttribLocation(theProgram, "vertexColor");
	// end::glGetAttribLocation[]

	// tag::glGetUniformLocation[]
	modelMatrixLocation = glGetUniformLocation(theProgram, "modelMatrix");
	viewMatrixLocation = glGetUniformLocation(theProgram, "viewMatrix");
	projectionMatrixLocation = glGetUniformLocation(theProgram, "projectionMatrix");

	//only generates runtime code in debug mode
	SDL_assert_release( modelMatrixLocation != -1);
	SDL_assert_release( viewMatrixLocation != -1);
	SDL_assert_release( projectionMatrixLocation != -1);
	// end::glGetUniformLocation[]

	//clean up shaders (we don't need them anymore as they are no in theProgram
	for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
}
// end::initializeProgram[]

// tag::initializeVertexArrayObject[]
//setup a GL object (a VertexArrayObject) that stores how to access data and from where
void initializeVertexArrayObject()
{
	glGenVertexArrays(1, &vertexArrayObject); //create a Vertex Array Object
	cout << "Vertex Array Object created OK! GLUint is: " << vertexArrayObject << std::endl;

	glBindVertexArray(vertexArrayObject); //make the just created vertexArrayObject the active one

		glBindBuffer(GL_ARRAY_BUFFER, vertexDataBufferObject); //bind vertexDataBufferObject

		glEnableVertexAttribArray(positionLocation); //enable attribute at index positionLocation
		glEnableVertexAttribArray(vertexColorLocation); //enable attribute at index vertexColorLocation

		// tag::glVertexAttribPointer[]
		glVertexAttribPointer(positionLocation,    3, GL_FLOAT, GL_FALSE, (7 * sizeof(GL_FLOAT)), (GLvoid *) (0 * sizeof(GLfloat))); //specify that position data contains four floats per vertex, and goes into attribute index positionLocation
		glVertexAttribPointer(vertexColorLocation, 4, GL_FLOAT, GL_FALSE, (7 * sizeof(GL_FLOAT)), (GLvoid *) (3 * sizeof(GLfloat))); //specify that position data contains four floats per vertex, and goes into attribute index vertexColorLocation
		// end::glVertexAttribPointer[]

// ============================================= This is the second VAO -- To be used for the Ball ===================================================
	glBindVertexArray(0); //unbind the vertexArrayObject so we can't change it

		glGenVertexArrays(1, &vertexArrayObject2); //create a Vertex Array Object
		cout << "Vertex Array Object 2 created OK! GLUint is: " << vertexArrayObject2 << std::endl;

		glBindVertexArray(vertexArrayObject2); //make the just created vertexArrayObject the active one
		glBindBuffer(GL_ARRAY_BUFFER, vertexDataBufferObject2); //bind vertexDataBufferObject

		glEnableVertexAttribArray(positionLocation); //enable attribute at index positionLocation
		glEnableVertexAttribArray(vertexColorLocation); //enable attribute at index vertexColorLocation

		// tag::glVertexAttribPointer[]
		glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, (7 * sizeof(GL_FLOAT)), (GLvoid *)(0 * sizeof(GLfloat))); //specify that position data contains four floats per vertex, and goes into attribute index positionLocation
		glVertexAttribPointer(vertexColorLocation, 4, GL_FLOAT, GL_FALSE, (7 * sizeof(GL_FLOAT)), (GLvoid *)(3 * sizeof(GLfloat))); //specify that position data contains four floats per vertex, and goes into attribute index vertexColorLocation
																																// end::glVertexAttribPointer[]
	glBindVertexArray(0); //unbind the vertexArrayObject so we can't change it

// ============================================= This is the third VAO -- To be used for the Bounds ===================================================
	glBindVertexArray(0); //unbind the vertexArrayObject so we can't change it

		glGenVertexArrays(1, &vertexArrayObject3); //create a Vertex Array Object
		cout << "Vertex Array Object 3 created OK! GLUint is: " << vertexArrayObject3 << std::endl;

		glBindVertexArray(vertexArrayObject3); //make the just created vertexArrayObject the active one
		glBindBuffer(GL_ARRAY_BUFFER, vertexDataBufferObject3); //bind vertexDataBufferObject

		glEnableVertexAttribArray(positionLocation); //enable attribute at index positionLocation
		glEnableVertexAttribArray(vertexColorLocation); //enable attribute at index vertexColorLocation

														// tag::glVertexAttribPointer[]
		glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, (7 * sizeof(GL_FLOAT)), (GLvoid *)(0 * sizeof(GLfloat))); //specify that position data contains four floats per vertex, and goes into attribute index positionLocation
		glVertexAttribPointer(vertexColorLocation, 4, GL_FLOAT, GL_FALSE, (7 * sizeof(GL_FLOAT)), (GLvoid *)(3 * sizeof(GLfloat))); //specify that position data contains four floats per vertex, and goes into attribute index vertexColorLocation
	// end::glVertexAttribPointer[]
	glBindVertexArray(0); //unbind the vertexArrayObject so we can't change it


// ============================================= This is the fourth VAO -- To be used for the Score ===================================================
	glBindVertexArray(0); //unbind the vertexArrayObject so we can't change it

		glGenVertexArrays(1, &vertexArrayObject4); //create a Vertex Array Object
		cout << "Vertex Array Object 4 created OK! GLUint is: " << vertexArrayObject4 << std::endl;

		glBindVertexArray(vertexArrayObject4); //make the just created vertexArrayObject the active one
		glBindBuffer(GL_ARRAY_BUFFER, vertexDataBufferObject4); //bind vertexDataBufferObject

		glEnableVertexAttribArray(positionLocation); //enable attribute at index positionLocation
		glEnableVertexAttribArray(vertexColorLocation); //enable attribute at index vertexColorLocation

														// tag::glVertexAttribPointer[]
		glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, (7 * sizeof(GL_FLOAT)), (GLvoid *)(0 * sizeof(GLfloat))); //specify that position data contains four floats per vertex, and goes into attribute index positionLocation
		glVertexAttribPointer(vertexColorLocation, 4, GL_FLOAT, GL_FALSE, (7 * sizeof(GL_FLOAT)), (GLvoid *)(3 * sizeof(GLfloat))); //specify that position data contains four floats per vertex, and goes into attribute index vertexColorLocation
																																	// end::glVertexAttribPointer[]
	glBindVertexArray(0); //unbind the vertexArrayObject so we can't change it

	//cleanup
	glDisableVertexAttribArray(positionLocation); //disable vertex attribute at index positionLocation
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind array buffer

}
// end::initializeVertexArrayObject[]

// tag::initializeVertexBuffer[]
void initializeVertexBuffer()
{
	glGenBuffers(1, &vertexDataBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, vertexDataBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	cout << "vertexDataBufferObject created OK! GLUint is: " << vertexDataBufferObject << std::endl;


	glGenBuffers(1, &vertexDataBufferObject2);

	glBindBuffer(GL_ARRAY_BUFFER, vertexDataBufferObject2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ballVertexData), ballVertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	cout << "vertexDataBufferObject 2 created OK! GLUint is: " << vertexDataBufferObject2 << std::endl;

	glGenBuffers(1, &vertexDataBufferObject3);

	glBindBuffer(GL_ARRAY_BUFFER, vertexDataBufferObject3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boundsVertexData), boundsVertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	cout << "vertexDataBufferObject 3 created OK! GLUint is: " << vertexDataBufferObject3 << std::endl;

	glGenBuffers(1, &vertexDataBufferObject4);

	glBindBuffer(GL_ARRAY_BUFFER, vertexDataBufferObject4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(scoreVertexData), scoreVertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	cout << "vertexDataBufferObject 4 created OK! GLUint is: " << vertexDataBufferObject4 << std::endl;

	initializeVertexArrayObject();
}
// end::initializeVertexBuffer[]

// tag::loadAssets[]
void loadAssets()
{
	initializeProgram(); //create GLSL Shaders, link into a GLSL program, and get IDs of attributes and variables

	initializeVertexBuffer(); //load data into a vertex buffer

	cout << "Loaded Assets OK!\n";
}
// end::loadAssets[]

// tag::handleInput[]
void handleInput()
{
	//Event-based input handling
	//The underlying OS is event-based, so **each** key-up or key-down (for example)
	//generates an event.
	//  - https://wiki.libsdl.org/SDL_PollEvent
	//In some scenarios we want to catch **ALL** the events, not just to present state
	//  - for instance, if taking keyboard input the user might key-down two keys during a frame
	//    - we want to catch based, and know the order
	//  - or the user might key-down and key-up the same within a frame, and we still want something to happen (e.g. jump)
	//  - the alternative is to Poll the current state with SDL_GetKeyboardState

	SDL_Event event; //somewhere to store an event

	//NOTE: there may be multiple events per frame
	while (SDL_PollEvent(&event)) //loop until SDL_PollEvent returns 0 (meaning no more events)
	{
		switch (event.type)
		{
		case SDL_QUIT:
			done = true; //set donecreate remote branch flag if SDL wants to quit (i.e. if the OS has triggered a close event,
							//  - such as window close, or SIGINT
			break;

			//keydown handling - we should to the opposite on key-up for direction controls (generally)
		case SDL_KEYDOWN:
			//Keydown can fire repeatable if key-repeat is on.
			//  - the repeat flag is set on the keyboard event, if this is a repeat event
			//  - in our case, we're going to ignore repeat events
			//  - https://wiki.libsdl.org/SDL_KeyboardEvent
			if (!event.key.repeat)
				switch (event.key.keysym.sym)
				{

					//hit escape to exit
					case SDLK_ESCAPE: done = true;

							case SDLK_a:
								// Move bat one left
								velocity1.x -= speed;
								break;
							case SDLK_d:
								// move bat one right
								velocity1.x += speed;
								break;
							case SDLK_LEFT:
								// move bat 2 left
								velocity2.x -= speed;
								break;
							case SDLK_RIGHT:
								// move bat 2 right
								velocity2.x += speed;
								break;

					case SDLK_SPACE:
						// Change Camera View
						switch (camView) {
							case 1:
								camView = 2;
								break;
							case 2:
								camView = 3;
								break;
							case 3:
								camView = 4;
								break;
							case 4:
								camView = 5;
								break;
							case 5:
								camView = 1;
								break;
							default:
								camView = 1;
								break;
						}
						break;
				}
			break;

		case SDL_KEYUP:
			//Keydown can fire repeatable if key-repeat is on.
			//  - the repeat flag is set on the keyboard event, if this is a repeat event
			//  - in our case, we're going to ignore repeat events
			//  - https://wiki.libsdl.org/SDL_KeyboardEvent
			if (!event.key.repeat)
				switch (event.key.keysym.sym)
				{
					case SDLK_a:
						// Reset bat 1 movement to stop it when key is released
						velocity1.x += speed;
						break;
					case SDLK_d:
						// Reset bat 1 movement to stop it when key is released
						velocity1.x -= speed;
						break;
					case SDLK_LEFT:
						// Reset bat 2 movement to stop when key is released
						velocity2.x += speed;
						break;
					case SDLK_RIGHT:
						// Reset bat 2 movement to stop when key is released
						velocity2.x -= speed;
						break;
				}

			break;
		}
	}
}
// end::handleInput[]

// tag::updateSimulation[]
void updateSimulation(double simLength = 0.02) //update simulation with an amount of time to simulate for (in seconds)
{
	//WARNING - we should calculate an appropriate amount of time to simulate - not always use a constant amount of time
			// see, for example, http://headerphile.blogspot.co.uk/2014/07/part-9-no-more-delays.html

	position1 += float(simLength) * velocity1;
	position2 += float(simLength) * velocity2;
	rotateAngle += simLength * 2;

	ballPosition += float(simLength) * ballVelocity;

	// Check for collisions between the bats and the boundaries
	if (position1.x + 0.5 > 2.5)
		position1.x = 2.0f;
	else if (position1.x - 0.5 < -2.5)
		position1.x = -2.0f;

	if (position2.x + 0.5 > 2.5)
		position2.x = 2.0f;
	else if (position2.x - 0.5 < -2.5)
		position2.x = -2.0f;

	// Check for collision with the ball and the bounds
	if (ballPosition.x + 0.1 > 2.5 || ballPosition.x - 0.1 < -2.5)
		ballVelocity.x *= -1.0f;

	if (ballPosition.z + 0.1 > 3.0)
	{
		resetBall(true);
	}
	if (ballPosition.z - 0.1 < -3.0)
	{
		// Blue gets a point
		resetBall(false);
	}


	// Check for collisions between the ball and the bats
	// If the outer edges of the ball are within the outer edges of the bat X coord, and the Z coords cross then that is a hit
	if ((ballPosition.x + 0.1f > position1.x - 0.5f && ballPosition.x - 0.1f < position1.x + 0.5f) && ballPosition.z - 0.1 < -2.3f)
		ballVelocity.z = 1;
	else if ((ballPosition.x + 0.1f > position2.x - 0.5f && ballPosition.x - 0.1f < position2.x + 0.5f) && ballPosition.z + 0.1 > 2.3f)
		ballVelocity.z = -1;

}
// end::updateSimulation[]

void resetBall(bool isRedPoint)
{
	if (isRedPoint)
		redScore++;
	else
		blueScore++;

	if (redScore >= 5 || blueScore >= 5)
	{
		gameOver = true;
		ballVelocity.x = 0.0f;
		ballVelocity.z = 0.0f;
	}

	ballPosition.x = 0;
	ballPosition.z = 0;

}

// tag::preRender[]
void preRender()
{

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glViewport(0, 0, 1000, 700); //set viewpoint
	glClearColor(0.2f, 0.0f, 0.2f, 1.0f); //set clear colour
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the window (technical the scissor box bounds)
}
// end::preRender[]

// tag::render[]
void render()
{
	glUseProgram(theProgram); //installs the program object specified by program as part of current rendering state

	glBindVertexArray(vertexArrayObject);

	//set projectionMatrix - how we go from 3D to 2D
	glUniformMatrix4fv(projectionMatrixLocation, 1, false, glm::value_ptr(glm::perspective(90.0f, 1.0f, 0.1f, 100.0f))); // http://stackoverflow.com/questions/8115352/glmperspective-explanation

	//set viewMatrix - how we control the view (viewpoint, view direction, etc)

	// I learned Camera stuff from here http://learnopengl.com/#!Getting-started/Camera
	switch (camView)
	{
		case 1:
			speed = 3.0f;
			glUniformMatrix4fv(viewMatrixLocation, 1, false, glm::value_ptr(glm::lookAt(glm::vec3(0.0f, 1.5f, 4.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)))); // Standard behind blue view
			break;
		case 2:
			speed = 3.0f;
			glUniformMatrix4fv(viewMatrixLocation, 1, false, glm::value_ptr(glm::lookAt(glm::vec3(2.0f, 3.5f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)))); // Above and look down view
			break;
		case 3:
			speed = -3.0f;
			glUniformMatrix4fv(viewMatrixLocation, 1, false, glm::value_ptr(glm::lookAt(glm::vec3(position1.x, position2.y + 1.5f, position1.z - 4.0f), position1, glm::vec3(0.0f, 1.0f, 0.0f)))); // Track Red -- Also the controls need inverting here
			break;
		case 4:
			speed = 3.0f;
			glUniformMatrix4fv(viewMatrixLocation, 1, false, glm::value_ptr(glm::lookAt(glm::vec3(position2.x, position2.y + 1.5f, position2.z + 4.0f), position2, glm::vec3(0.0f, 1.0f, 0.0f)))); // Track Blue
			break;
		case 5:
			speed = 3.0f;
			glUniformMatrix4fv(viewMatrixLocation, 1, false, glm::value_ptr(glm::lookAt(glm::vec3(ballPosition.x + 2.0f, ballPosition.y + 3.5f, ballPosition.z), ballPosition, glm::vec3(0.0f, 1.0f, 0.0f)))); // Track the ball
			break;
	}


	// ==================================== Render the Bats ================================
	modelMatrix = glm::translate(glm::mat4(1.0f), position1);
	//modelMatrix = glm::rotate(modelMatrix, rotateAngle, glm::vec3(0, 0, 0));
	glUniformMatrix4fv(modelMatrixLocation, 1, false, glm::value_ptr(modelMatrix));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	modelMatrix = glm::translate(glm::mat4(1.0f), position2);
	glUniformMatrix4fv(modelMatrixLocation, 1, false, glm::value_ptr(modelMatrix));
	glDrawArrays(GL_TRIANGLES, 36, 78 );

	// =================================== Render the Bounds ==================================
	glBindVertexArray(vertexArrayObject3); 

	// ====== Left and Right Bounds =====
	boundPosition.z = 0.0f;
	boundPosition.x = -2.5f;
	modelMatrix = glm::translate(glm::mat4(1.0f), boundPosition);
	glUniformMatrix4fv(modelMatrixLocation, 1, false, glm::value_ptr(modelMatrix));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	boundPosition.x = 2.5f;
	modelMatrix = glm::translate(glm::mat4(1.0f), boundPosition);
	glUniformMatrix4fv(modelMatrixLocation, 1, false, glm::value_ptr(modelMatrix));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// ====== Top and Bottom Bounds ======
	boundPosition.x = 0.0f;
	boundPosition.z = -3.0f;
	modelMatrix = glm::translate(glm::mat4(1.0f), boundPosition);
	glUniformMatrix4fv(modelMatrixLocation, 1, false, glm::value_ptr(modelMatrix));
	glDrawArrays(GL_TRIANGLES, 36, 78);

	boundPosition.z = 3.0f;
	modelMatrix = glm::translate(glm::mat4(1.0f), boundPosition);
	glUniformMatrix4fv(modelMatrixLocation, 1, false, glm::value_ptr(modelMatrix));
	glDrawArrays(GL_TRIANGLES, 36, 78);

	// ==================================== Render the Ball ==================================
	glBindVertexArray(vertexArrayObject2);

	modelMatrix = glm::translate(glm::mat4(1.0f), ballPosition);
	modelMatrix = glm::rotate(modelMatrix, rotateAngle, glm::vec3(1, 1, 1));
	glUniformMatrix4fv(modelMatrixLocation, 1, false, glm::value_ptr(modelMatrix));
	glDrawArrays(GL_TRIANGLES, 0, 36);


	glUniformMatrix4fv(projectionMatrixLocation, 1, false, glm::value_ptr(glm::mat4(1.0f)));
	glUniformMatrix4fv(viewMatrixLocation, 1, false, glm::value_ptr(glm::mat4(1.0f)));

	glBindVertexArray(vertexArrayObject4);

	// Red Score
	scorePosition.y = 0.95f;
	scorePosition.x = -0.95f;
	for (int i = 0; i < redScore; i++)
	{
		modelMatrix = glm::translate(glm::mat4(1.0f), scorePosition);
		//modelMatrix = glm::rotate(modelMatrix, rotateAngle, glm::vec3(0, 1, 0));
		glUniformMatrix4fv(modelMatrixLocation, 1, false, glm::value_ptr(modelMatrix));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		scorePosition.x += 0.08f;
	}

	// Blue Score	
	scorePosition.x = 0.95f;
	for (int i = 0; i < blueScore; i++)
	{
		modelMatrix = glm::translate(glm::mat4(1.0f), scorePosition);
		//modelMatrix = glm::rotate(modelMatrix, rotateAngle, glm::vec3(0, 1, 0));
		glUniformMatrix4fv(modelMatrixLocation, 1, false, glm::value_ptr(modelMatrix));
		glDrawArrays(GL_TRIANGLES, 6, 12);
		scorePosition.x -= 0.08f;
	}

	glBindVertexArray(0);

	glUseProgram(0); //clean up
}
// end::render[]

// tag::postRender[]
void postRender()
{
	SDL_GL_SwapWindow(win);; //present the frame buffer to the display (swapBuffers)
	frameLine += "Frame: " + std::to_string(frameCount++);
	cout << "\r" << frameLine << std::flush;
	frameLine = "";
}
// end::postRender[]

// tag::cleanUp[]
void cleanUp()
{
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(win);
	cout << "Cleaning up OK!\n";
}
// end::cleanUp[]

// tag::main[]
int main( int argc, char* args[] )
{
	exeName = args[0];
	//setup
	//- do just once
	initialise();
	createWindow();

	createContext();

	initGlew();

	glViewport(0, 0, 1000, 700); //should check what the actual window res is?

	//do stuff that only needs to happen once
	//- create shaders
	//- load vertex data
	loadAssets();

	while (!done) //loop until done flag is set)
	{
		handleInput(); // this should ONLY SET VARIABLES

		updateSimulation(); // this should ONLY SET VARIABLES according to simulation

		preRender();

		render(); // this should render the world state according to VARIABLES -

		postRender();

	}

	//cleanup and exit
	cleanUp();
	SDL_Quit();

	return 0;
}
// end::main[]
