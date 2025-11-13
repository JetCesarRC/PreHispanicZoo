#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>
#include <random>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//Para archivo
#include <fstream>
#include <sstream>

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para animación
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;
bool avanza;
float angulovaria = 0.0f;
float alaAngulo = 0.0f;

//variables para keyframes
float reproduciranimacion, habilitaranimacion, guardoFrame, reinicioFrame, ciclo, ciclo2, contador = 0;


Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;
// ===================================================
// Configuración de la cámara de seguimiento de Alex
// ===================================================
FollowCamera alexCam(glm::vec3(0.0f, 30.0f, 50.0f), 0.1f);  // offset y sensibilidad
glm::vec3 alexPos(0.0f, -2.0f, -100.0f);                   // posición inicial de Alex
float alexYaw = 0.0f;                                      // rotación del modelo
bool usarCamaraAlex = false;                               // alternar entre cámaras


// ----------------------------------------
// CÁMARAS ADICIONALES
// ----------------------------------------
// Cámara aérea: posición alta y movimiento en XZ
AerialCamera camAerea(glm::vec3(250.0f, 220.0f, -10.0f), 5.0f);
FixedCamera camInteres(glm::vec3(200.0f, 60.0f, -30.0f), glm::vec3(190.0f, 36.0f, -10.0f)); // zona jaguares
FixedCamera camPiramide(
	glm::vec3(100.0f, 100.0f, 190.0f),
	glm::vec3(40.0f, 50.0f, 130.0f)
);
FixedCamera camAviario(glm::vec3(-200.0f, 80.0f, 200.0f), glm::vec3(-100.0f, 40.0f, 90.0f)); // vista aviario


int modoCamara = 0; // 0 = libre, 1 = seguimiento, 2 = aérea, 3 = interés

Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;
Texture ArcoTexture;
Texture PuertaTexture;
Texture lumbreraTexture;
Texture aguilaTexture;
Texture habitat_ajoloteTexture;
Texture nidoTexture;
Texture troncoTexture;
Texture jaguarTexture;
Texture arbolTexture;
Texture piedrasTexture;
Texture ringTexture;
Texture axoloteTexture;
Texture piramideTexture;
Texture arbolAviarioTexture;
Texture pajaroTexture;
Texture arbusto1Texture;
Texture arbusto2Texture;
Texture arbusto3Texture;
Texture arbol2Texture;
Texture aguaTexture;
Texture cuadroTexture;
Texture AlbercaJaTexture;
Texture aguila_Letrero;
Texture ajolotes_Letrero;
Texture Aviario_Letrero;
Texture Jaguar_Letrero;
Texture Piramide_Letrero;
Texture RingDeLucha_Letrero;
Texture bancaTexture;
Texture npc1Texture;
Texture npc2Texture;

Texture alexTexture;

Texture domoTexture;
Texture palmaTexture;
Texture	casaPajaroTexture;
Texture demoParrotTexture;
Texture demoParrotTexture2;
Texture troncoAviarioTexture;
Texture aveAzulTexture;
Texture guacamayaRojaTexture;
Texture guacamayaRojaTexture2;

Texture poTexture;


Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;
Model Arco_M;
Model Letrero_M;
Model Puerta_Derecha_M;
Model Puerta_Izquierda_M;
Model lumbrera;
Model aguila_M;
Model alaDer_M;
Model alaIzq_M;
Model habitat_ajolote_M;
Model nido_M;
Model tronco_M;

Model jaguar_M;
Model colaJa_M;
Model pataFDer_M;
Model pataFIzq_M;
Model pataTDer_M;
Model pataTIzq_M;


Model arbol_M;
Model piedras_M;
Model ring_M;
Model piramide_M;
Model axolote_M;
Model axoloteCola_M;
Model axolotePataDerecha_M;
Model axolotePataIzquierda_M;
Model axolotePataTraseraDerecha_M;
Model axolotePataTraseraIzquierda_M;
Model arbusto1_M;
Model arbusto2_M;
Model arbusto3_M;
Model arbol2_M;
Model agua_M;
Model reja_M;
Model jaula_M;
Model cuadro_M;
Model AlbercaJa_M;
Model farola_M;
Model LetreroHabitad_M;
Model banca_M;
Model bote_M;


/*
	NPCS
*/

Model cuerpo1;
Model brazoDer1;
Model brazoIzq1;
Model piernaDer1;
Model piernaIzq1;

Model cuerpo2;
Model brazoDer2;
Model brazoIzq2;
Model piernaDer2;
Model piernaIzq2;

/*
====Avatar====
*/

Model aCuerpo_M;
Model aBrazoDer_M;
Model aBrazoIzq_M;
Model aPiernaDer_M;
Model aPiernaIzq_M;


Model arbolAviario_M;
Model pajaro_M;
Model domo_M;
Model palma_M;
Model casaPajaro_M;
Model demoParrot_M;
Model troncoAviario_M;
Model aveAzul_M;
Model guacamayaRoja_M;

Model PoBody;
Model BraDerpo;
Model BraIzqPo;
Model PataIzqPo;
Model PataDerPo;


Skybox skybox;
Skybox nightSkybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;



// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

//función para teclado de keyframes 
void inputKeyframes(bool* keys);

//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};
	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};


	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6);

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7);

}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


///////////////////////////////KEYFRAMES/////////////////////


bool animacion = false;

//NEW// Keyframes
float posXavion = 2.0, posYavion = 5.0, posZavion = -3.0;

//Número de cuadros máximos
//Número de pasos entre cuadros para interpolación, a mayor número , más lento será el movimiento
struct FRAME {
	float movX;
	float movY;
	float movXInc;
	float movYInc;
	float giro;
	float giroInc;
};

#define MAX_FRAMES 100
// Variables globales de control
std::string archivoActual = "keyframes_ajolote.txt";

// Avión
// Ajolote
FRAME KeyFrameAjolote[MAX_FRAMES];
int FrameIndexAjolote = 0;
float movAjolote_x = 0, movAjolote_y = 0, giroAjolote = 0;
int playIndexAjolote = 0, i_curr_stepsAjolote = 0;
bool playAjolote = false;


// Águila
FRAME KeyFrameAguila[MAX_FRAMES];
int FrameIndexAguila = 0;
float movAguila_x = 0, movAguila_y = 0, giroAguila = 0;
int playIndexAguila = 0, i_curr_stepsAguila = 0;
bool playAguila = false;

// Control de teclas
int reproducirAnimacion = 0;
bool modoEdicion = false;
bool modoRotacion = false;

int i_max_steps = 100;
bool play = false;
int playIndex = 0;

// --- REINICIAR AL PRIMER FRAME ---
void resetElements(FRAME* KeyFrame, float& movX, float& movY, float& giro)
{
	movX = KeyFrame[0].movX;
	movY = KeyFrame[0].movY;
	giro = KeyFrame[0].giro;
}

// --- CALCULAR INCREMENTOS ---
void interpolation(FRAME* KeyFrame, int playIndex, int i_max_steps)
{
	KeyFrame[playIndex].movXInc = (KeyFrame[playIndex + 1].movX - KeyFrame[playIndex].movX) / i_max_steps;
	KeyFrame[playIndex].movYInc = (KeyFrame[playIndex + 1].movY - KeyFrame[playIndex].movY) / i_max_steps;
	KeyFrame[playIndex].giroInc = (KeyFrame[playIndex + 1].giro - KeyFrame[playIndex].giro) / i_max_steps;
}

// --- ANIMAR ---
void animate(FRAME* KeyFrame, int& playIndex, int& FrameIndex, int& i_curr_steps,
	int i_max_steps, bool& play,
	float& movX, float& movY, float& giro)
{
	if (!play) return;

	if (i_curr_steps >= i_max_steps) // fin de interpolación
	{
		playIndex++;
		if (playIndex > FrameIndex - 2) // fin de animación total
		{
			printf("terminó la animación\n");
			playIndex = 0;
			play = false;
		}
		else
		{
			i_curr_steps = 0;
			interpolation(KeyFrame, playIndex, i_max_steps);
		}
	}
	else
	{
		movX += KeyFrame[playIndex].movXInc;
		movY += KeyFrame[playIndex].movYInc;
		giro += KeyFrame[playIndex].giroInc;
		i_curr_steps++;
	}
}

///////////////* FIN KEYFRAMES*////////////////////////////

/*
============================= Archivo =============================
*/


// --- GUARDAR / AÑADIR ---
void appendKeyframeToFile(const std::string& archivo, const FRAME& frame, int index)
{
	std::ofstream file(archivo, std::ios::app);
	if (!file.is_open()) {
		printf("Error al abrir %s para escribir.\n", archivo.c_str());
		return;
	}
	file << index << " " << frame.movX << " " << frame.movY << " " << frame.giro << "\n";
	file.close();
	printf("Frame %d agregado al archivo %s.\n", index, archivo.c_str());
}

void saveFrame(const std::string& archivo, FRAME* KeyFrame, int& FrameIndex,
	float movX, float movY, float giro)
{
	KeyFrame[FrameIndex].movX = movX;
	KeyFrame[FrameIndex].movY = movY;
	KeyFrame[FrameIndex].giro = giro;

	appendKeyframeToFile(archivo, KeyFrame[FrameIndex], FrameIndex);
	FrameIndex++;
}

// --- LEER DESDE ARCHIVO ---
void leerArchivo(const std::string& archivo, FRAME* KeyFrame, int& FrameIndex)
{
	std::ifstream file(archivo);
	if (!file.is_open()) {
		printf("No se encontró %s, se creará al guardar.\n", archivo.c_str());
		return;
	}

	FrameIndex = 0;
	std::string line;
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		int idx;
		FRAME temp;
		if (iss >> idx >> temp.movX >> temp.movY >> temp.giro) {
			KeyFrame[idx] = temp;
			FrameIndex = std::max(FrameIndex, idx + 1);
		}
	}
	file.close();
	printf("Se cargaron %d frames desde %s\n", FrameIndex, archivo.c_str());
}

// --- MODIFICAR ---
void actualizarArchivo(const std::string& archivo, int index, const FRAME& newFrame)
{
	std::ifstream fileIn(archivo);
	if (!fileIn.is_open()) {
		printf("No se puede abrir %s para modificar.\n", archivo.c_str());
		return;
	}

	std::vector<std::string> lines;
	std::string line;
	while (std::getline(fileIn, line)) lines.push_back(line);
	fileIn.close();

	std::ofstream fileOut(archivo);
	if (!fileOut.is_open()) {
		printf("No se puede escribir %s\n", archivo.c_str());
		return;
	}

	bool found = false;
	for (auto& l : lines) {
		std::istringstream iss(l);
		int idx; float x, y, g;
		if (iss >> idx >> x >> y >> g) {
			if (idx == index) {
				fileOut << index << " " << newFrame.movX << " " << newFrame.movY << " " << newFrame.giro << "\n";
				found = true;
			}
			else fileOut << l << "\n";
		}
	}
	fileOut.close();
	if (found)
		printf("Frame %d modificado correctamente en %s.\n", index, archivo.c_str());
	else
		printf("Frame %d no encontrado en %s.\n", index, archivo.c_str());
}

// --- BORRAR ---
void borrarFrame(const std::string& archivo, int index)
{
	std::ifstream fileIn(archivo);
	if (!fileIn.is_open()) {
		printf("No se puede abrir %s para borrar.\n", archivo.c_str());
		return;
	}

	std::vector<std::string> lines;
	std::string line;
	while (std::getline(fileIn, line)) lines.push_back(line);
	fileIn.close();

	std::ofstream fileOut(archivo);
	if (!fileOut.is_open()) {
		printf("No se puede escribir %s\n", archivo.c_str());
		return;
	}

	bool found = false;
	for (auto& l : lines) {
		std::istringstream iss(l);
		int idx;
		if (iss >> idx) {
			if (idx == index) {
				found = true;
				continue;
			}
			fileOut << l << "\n";
		}
	}
	fileOut.close();
	if (found)
		printf("Frame %d eliminado correctamente de %s.\n", index, archivo.c_str());
	else
		printf("Frame %d no encontrado en %s.\n", index, archivo.c_str());
}




int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	/*
	=======================================
			   T E X T U R E S
	=======================================
	*/

	pisoTexture = Texture("Textures/Nordic_Moss_se4rwei_1K_BaseColor.jpg");
	pisoTexture.LoadTextureA();
	ArcoTexture = Texture("Textures/Mossy_Stone_Floor_veilfjxn_1K_BaseColor.jpg");
	ArcoTexture.LoadTextureA();
	PuertaTexture = Texture("Textures/Rusty_Metal_Sheet_tj4kedvcw_1K_BaseColor.jpg");
	PuertaTexture.LoadTextureA();
	lumbreraTexture = Texture("Textures/lumbrera.png");
	lumbreraTexture.LoadTextureA();
	aguilaTexture = Texture("Textures/Fogel_Mat_Diffuse_Color.png");
	aguilaTexture.LoadTextureA();
	habitat_ajoloteTexture = Texture("Textures/rock_wall_13_diff_4k.jpg");
	habitat_ajoloteTexture.LoadTextureA();
	nidoTexture = Texture("Textures/wood_table_diff_4k.jpg");
	nidoTexture.LoadTextureA();
	troncoTexture = Texture("Textures/pine_bark_diff_4k.png");
	troncoTexture.LoadTextureA();
	jaguarTexture = Texture("Textures/Jaguar_BaseColor.png");
	jaguarTexture.LoadTextureA();
	arbolTexture = Texture("Textures/bark_willow_diff_4k.png");
	arbolTexture.LoadTextureA();
	piedrasTexture = Texture("Textures/CavePlatform4_Base_Specular.png");
	piedrasTexture.LoadTextureA();
	ringTexture = Texture("Textures/Lona.png");
	ringTexture.LoadTextureA();
	axoloteTexture = Texture("Textures/ajoloteTexture.png");
	axoloteTexture.LoadTextureA();
	piramideTexture = Texture("Textures/Stone_Wall_tinifauaw_1K_BaseColor.jpg");
	piramideTexture.LoadTextureA();
	arbolAviarioTexture = Texture("Textures/Tree_Bark_vimmdcofw_1K_BaseColor.jpg");
	arbolAviarioTexture.LoadTextureA();
	pajaroTexture = Texture("Textures/12213_bird_diffuse.jpg");
	pajaroTexture.LoadTextureA();
	arbusto1Texture = Texture("Textures/TX_AlexandraPalm_ALB.jpg");
	arbusto1Texture.LoadTextureA();
	arbusto2Texture = Texture("Textures/3 leaves_Color.png");
	arbusto2Texture.LoadTextureA();
	arbusto3Texture = Texture("Textures/Grass.jpg");
	arbusto3Texture.LoadTextureA();
	arbol2Texture = Texture("Textures/tree2.low_Material_BaseColor.jpg");
	arbol2Texture.LoadTextureA();
	aguaTexture = Texture("Textures/3DS - Bravely Second_ End Layer - Environment - Water.png");
	aguaTexture.LoadTextureA();
	cuadroTexture = Texture("Textures/blue_metal_plate_diff_4k.png"); // Ajusta la extensión según tu archivo
	cuadroTexture.LoadTextureA();
	AlbercaJaTexture = Texture("Textures/3DS - Bravely Second_ End Layer - Environment - Water.png");
	AlbercaJaTexture.LoadTextureA();
	aguila_Letrero = Texture("Textures/aguila_letrero.png");
	aguila_Letrero.LoadTextureA();
	ajolotes_Letrero = Texture("Textures/ajolotes_Letrero.png");
	ajolotes_Letrero.LoadTextureA();
	Aviario_Letrero = Texture("Textures/Aviario_Letrero.png");
	Aviario_Letrero.LoadTextureA();
	Jaguar_Letrero = Texture("Textures/Jaguares_Letrero.png");
	Jaguar_Letrero.LoadTextureA();
	Piramide_Letrero = Texture("Textures/Piramide_Letrero.png");
	Piramide_Letrero.LoadTextureA();
	RingDeLucha_Letrero = Texture("Textures/RingDeLucha_Letrero.png");
	RingDeLucha_Letrero.LoadTextureA();
	bancaTexture = Texture("Textures/Stone_Wall_ujdiddfew_1K_BaseColor.jpg");
	bancaTexture.LoadTextureA();

	npc1Texture = Texture("Textures/Pedrito.jpg");
	npc1Texture.LoadTextureA();
	npc2Texture = Texture("Textures/Nathan.jpg");
	npc2Texture.LoadTexture();

	alexTexture = Texture("Textures/Alex.jpg");
	alexTexture.LoadTexture();

	domoTexture = Texture("Textures/PSP - LittleBigPlanet - Glass - Blue Clear Glass.png");
	domoTexture.LoadTextureA();
	palmaTexture = Texture("Textures/Tree_Palm_DIFF.png");
	palmaTexture.LoadTextureA();
	casaPajaroTexture = Texture("Textures/TexturaMadera.jpg");
	casaPajaroTexture.LoadTextureA();
	demoParrotTexture = Texture("Textures/demo_parrot.png");
	demoParrotTexture.LoadTextureA();
	demoParrotTexture2 = Texture("Textures/demo_parrot_blue.png");
	demoParrotTexture2.LoadTextureA();
	troncoAviarioTexture = Texture("Textures/TexturaDeArbol.jpg");
	troncoAviarioTexture.LoadTextureA();
	aveAzulTexture = Texture("Textures/fadb6b67.png");
	aveAzulTexture.LoadTextureA();
	guacamayaRojaTexture = Texture("Textures/pk1_par1.png");
	guacamayaRojaTexture.LoadTextureA();
	guacamayaRojaTexture2 = Texture("Textures/k1_umi14.png");
	guacamayaRojaTexture2.LoadTextureA();

	poTexture = Texture("Textures/Po.png");
	poTexture.LoadTextureA();

	/*
	=======================================
				M O D E L S
	=======================================
	*/

	Arco_M = Model();
	Arco_M.LoadModel("Models/Arco.fbx");
	Letrero_M = Model();
	Letrero_M.LoadModel("Models/Letrero.fbx");
	Puerta_Derecha_M = Model();
	Puerta_Derecha_M.LoadModel("Models/Puerta_Der.fbx");
	Puerta_Izquierda_M = Model();
	Puerta_Izquierda_M.LoadModel("Models/Puerta_Izq.fbx");
	lumbrera = Model();
	lumbrera.LoadModel("Models/Lumbrera.fbx");
	aguila_M = Model();
	aguila_M.LoadModel("Models/aguila.fbx");
	alaDer_M = Model();
	alaDer_M.LoadModel("Models/alaDerecha.fbx");
	alaIzq_M = Model();
	alaIzq_M.LoadModel("Models/alaIzquierda.fbx");
	habitat_ajolote_M = Model();
	habitat_ajolote_M.LoadModel("Models/habitat_ajolote.fbx");
	nido_M = Model();
	nido_M.LoadModel("Models/nido.fbx");
	tronco_M = Model();
	tronco_M.LoadModel("Models/tronco.fbx");

	jaguar_M = Model();
	jaguar_M.LoadModel("Models/jaguar/CuerpoJa.fbx");
	colaJa_M = Model();
	colaJa_M.LoadModel("Models/jaguar/ColaJa.fbx");
	pataFDer_M = Model();
	pataFDer_M.LoadModel("Models/jaguar/PataFDer.fbx");
	pataFIzq_M = Model();
	pataFIzq_M.LoadModel("Models/jaguar/PataFIzq.fbx");
	pataTDer_M = Model();
	pataTDer_M.LoadModel("Models/jaguar/PataTDer.fbx");
	pataTIzq_M = Model();
	pataTIzq_M.LoadModel("Models/jaguar/PataTIzq.fbx");



	arbol_M = Model();
	arbol_M.LoadModel("Models/arbol.fbx");
	piedras_M = Model();
	piedras_M.LoadModel("Models/piedras.fbx");
	ring_M = Model();
	ring_M.LoadModel("Models/Ring.fbx");
	axolote_M = Model();
	axolote_M.LoadModel("Models/axolote.fbx");
	axoloteCola_M = Model();
	axoloteCola_M.LoadModel("Models/colaAxolote.fbx");
	axolotePataDerecha_M = Model();
	axolotePataDerecha_M.LoadModel("Models/pataDerechaAxolote.fbx");
	axolotePataIzquierda_M = Model();
	axolotePataIzquierda_M.LoadModel("Models/pataIzquierdaAxolote.fbx");
	axolotePataTraseraDerecha_M = Model();
	axolotePataTraseraDerecha_M.LoadModel("Models/pataDerechaTraseraAxolote.fbx");
	axolotePataTraseraIzquierda_M = Model();
	axolotePataTraseraIzquierda_M.LoadModel("Models/pataIzquierdaTraseraAxolote.fbx");
	piramide_M = Model();
	piramide_M.LoadModel("Models/piramide.fbx");
	arbolAviario_M = Model();
	arbolAviario_M.LoadModel("Models/arbolAves.fbx");
	arbusto1_M = Model();
	arbusto1_M.LoadModel("Models/Arbustos/arbusto1.fbx");
	arbusto2_M = Model();
	arbusto2_M.LoadModel("Models/Arbustos/arbusto2.fbx");
	arbusto3_M = Model();
	arbusto3_M.LoadModel("Models/Arbustos/flor.fbx");
	arbol2_M = Model();
	arbol2_M.LoadModel("Models/arbol2.fbx");
	agua_M = Model();
	agua_M.LoadModel("Models/agua.fbx");
	reja_M = Model();
	reja_M.LoadModel("Models/reja-ajolote.fbx");
	jaula_M = Model();
	jaula_M.LoadModel("Models/jaula.fbx");
	cuadro_M = Model();
	cuadro_M.LoadModel("Models/cuadro.fbx");
	AlbercaJa_M = Model();
	AlbercaJa_M.LoadModel("Models/alberca.fbx");
	farola_M = Model();
	farola_M.LoadModel("Models/farolota.fbx");
	LetreroHabitad_M = Model();
	LetreroHabitad_M.LoadModel("Models/LetreroHabitad.fbx");
	banca_M = Model();
	banca_M.LoadModel("Models/banca.fbx");
	bote_M = Model();
	bote_M.LoadModel("Models/bote_basura.fbx");

	cuerpo1 = Model();
	cuerpo1.LoadModel("Models/NPC1/cuerpo.fbx");
	brazoDer1 = Model();
	brazoDer1.LoadModel("Models/NPC1/brazoDer.fbx");
	brazoIzq1 = Model();
	brazoIzq1.LoadModel("Models/NPC1/brazoIzq.fbx");
	piernaDer1 = Model();
	piernaDer1.LoadModel("Models/NPC1/piernaDer.fbx");
	piernaIzq1 = Model();
	piernaIzq1.LoadModel("Models/NPC1/piernaIzq.fbx");

	cuerpo2 = Model();
	cuerpo2.LoadModel("Models/NPC2/cuerpo.fbx");
	brazoDer2 = Model();
	brazoDer2.LoadModel("Models/NPC2/brazoDer.fbx");
	brazoIzq2 = Model();
	brazoIzq2.LoadModel("Models/NPC2/brazoIzq.fbx");
	piernaDer2 = Model();
	piernaDer2.LoadModel("Models/NPC2/piernaDer.fbx");
	piernaIzq2 = Model();
	piernaIzq2.LoadModel("Models/NPC2/piernaIzq.fbx");

	aCuerpo_M = Model();
	aCuerpo_M.LoadModel("Models/Avatar/AlexBody.fbx");
	aBrazoDer_M = Model();
	aBrazoDer_M.LoadModel("Models/Avatar/BraDerAlex.fbx");
	aBrazoIzq_M = Model();
	aBrazoIzq_M.LoadModel("Models/Avatar/BraIzqAlex.fbx");
	aPiernaDer_M = Model();
	aPiernaDer_M.LoadModel("Models/Avatar/PataDerAlex.fbx");
	aPiernaIzq_M = Model();
	aPiernaIzq_M.LoadModel("Models/Avatar/PataIzqAlex.fbx");

	domo_M = Model();
	domo_M.LoadModel("Models/Domo.fbx");
	palma_M = Model();
	palma_M.LoadModel("Models/PalmaMaceta.fbx");
	casaPajaro_M = Model();
	casaPajaro_M.LoadModel("Models/CasaPajaro.fbx");
	demoParrot_M = Model();
	demoParrot_M.LoadModel("Models/demo_parrot.fbx");
	troncoAviario_M = Model();
	troncoAviario_M.LoadModel("Models/Tronco2.fbx");
	pajaro_M = Model();
	pajaro_M.LoadModel("Models/pajaro.fbx");
	aveAzul_M = Model();
	aveAzul_M.LoadModel("Models/AveAzul.fbx");
	guacamayaRoja_M = Model();
	guacamayaRoja_M.LoadModel("Models/Guacamaya.fbx");

	PoBody = Model();
	PoBody.LoadModel("Models/po/PoBody.fbx");
	BraDerpo = Model();
	BraDerpo.LoadModel("Models/po/BraDerPo.fbx");
	BraIzqPo = Model();
	BraIzqPo.LoadModel("Models/po/BraIzqPo.fbx");
	PataDerPo = Model();
	PataDerPo.LoadModel("Models/po/PataDerPo.fbx");
	PataIzqPo = Model();
	PataIzqPo.LoadModel("Models/po/PataIzqPo.fbx");




	std::vector<std::string> skyboxFaces;
	std::vector<std::string> skyboxFacesNight;

	skyboxFaces.push_back("Textures/Skybox/nx.png");
	skyboxFaces.push_back("Textures/Skybox/px.png");
	skyboxFaces.push_back("Textures/Skybox/ny.png");
	skyboxFaces.push_back("Textures/Skybox/py.png");
	skyboxFaces.push_back("Textures/Skybox/nz.png");
	skyboxFaces.push_back("Textures/Skybox/pz.png");

	skyboxFacesNight.push_back("Textures/Night/nx.png");
	skyboxFacesNight.push_back("Textures/Night/px.png");
	skyboxFacesNight.push_back("Textures/Night/ny.png");
	skyboxFacesNight.push_back("Textures/Night/py.png");
	skyboxFacesNight.push_back("Textures/Night/nz.png");
	skyboxFacesNight.push_back("Textures/Night/pz.png");

	skybox = Skybox(skyboxFaces);
	nightSkybox = Skybox(skyboxFacesNight);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	mainLight = DirectionalLight(
		0.2f, 0.25f, 0.35f,   // color ambiente azulado
		0.2f, 0.1f,           // intensidades ambiente/difusa
		-0.5f, -1.0f, -0.3f   // dirección desde arriba
	);

	//contador de luces puntuales
	unsigned int pointLightCount = 0;

	// Farola 1 - Rosa
	pointLights[pointLightCount] = PointLight(
		1.0f, 0.3f, 0.8f,   // color rosa brillante
		4.0f, 10.0f,          // intensidades ambiente/difusa (MUY altas)
		165.0f, 6.0f, 100.0f, // posición más elevada
		1.0, 0.014, 0.0007   // atenuación mínima → más radio
	);
	int idxFarola1 = pointLightCount++;

	// Farola 2 - Amarilla
	pointLights[pointLightCount] = PointLight(
		0.0f, 1.0f, 0.0f,   // amarillo cálido intenso
		4.0f, 10.0f,
		-85.0f, 6.0f, 210.0f,
		1.0, 0.014, 0.0007
	);
	int idxFarola2 = pointLightCount++;

	// Farola 3 - Roja
	pointLights[pointLightCount] = PointLight(
		1.0f, 0.1f, 0.1f,   // rojo intenso
		4.0f, 10.0f,
		-85.0f, 6.0f, 40.0f,
		1.0, 0.014, 0.0007
	);
	int idxFarola3 = pointLightCount++;

	// Farola 4 - Azul
	pointLights[pointLightCount] = PointLight(
		0.0f, 0.0f, 1.0f,   // azul frío brillante
		4.0f, 10.0f,
		165.0f, 6.0f, -90.0f,
		1.0, 0.014, 0.0007
	);
	int idxFarola4 = pointLightCount++;



	unsigned int spotLightCount = 0;

	// --- REFLECTORES SUPERIORES ---
// Spotlight 1 → sobre el ring principal
	spotLights[spotLightCount] = SpotLight(
		1.0f, 0.97f, 0.9f,      // luz blanca cálida
		2.0f, 6.0f,            // intensidades ambiente/difusa
		20.0f, 40.0f, 40.0f,    // posición (encima del ring)
		0.0f, -1.0f, 0.0f,      // dirección hacia abajo
		1.0f, 0.004f, 0.00015f, // atenuación suave (mayor alcance)
		60.0f                   // ángulo del haz
	);
	int idxReflectorRing = spotLightCount++;

	// Spotlight 2 → sobre el área de jaguares
	spotLights[spotLightCount] = SpotLight(
		1.0f, 0.83f, 0.45f,
		2.0f, 6.0f,
		190.0f, 50.0f, -10.0f,   // posición (sobre hábitat jaguares)
		0.0f, -1.0f, 0.0f,
		1.0f, 0.004f, 0.00015f,
		80.0f
	);
	int idxReflectorJaguares = spotLightCount++;




	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	movCoche = 0.0f;
	movOffset = 0.01f;
	rotllanta = 0.0f;
	rotllantaOffset = 10.0f;
	glm::vec3 posblackhawk = glm::vec3(2.0f, 0.0f, 0.0f);

	//---------PARA TENER KEYFRAMES GUARDADOS NO VOLATILES QUE SIEMPRE SE UTILIZARAN SE DECLARAN AQUÍ


	//Se agregan nuevos frames 


	printf("\n=====================================\n");
	printf("      CONTROLES DE KEYFRAMES\n");
	printf("=====================================\n");
	printf(" [F1]		 Utilizar el helicoptero\n");
	printf(" [F2]		 Utilizar el aguila\n");
	printf(" [ESPACIO]   Reproducir animación desde keyframes.txt\n");
	printf(" [0]         Reiniciar animación para volver a reproducir\n");
	printf(" [1]         Activar/Desactivar modo edición\n");
	printf("    ├─ H / K Mover en eje X  (H = +X / K = -X)\n");
	printf("    ├─ U / J Mover en eje Y  (U = +Y / J = -Y)\n");
	printf("    └─ R     Activar/Desactivar modo rotación\n");
	printf(" [Flecha Izq/Der]	Rotar el modelo en eje Y\n");
	printf(" [L]         Guardar frame actual en keyframes.txt\n");
	printf(" [M]         Modificar último frame guardado\n");
	printf(" [B]         Borrar último frame del archivo\n");
	printf("=====================================\n\n");



	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);
	glm::vec3 lowerLight = glm::vec3(0.0f, 0.0f, 0.0f);

	float radio = 5.0f;             // radio del movimiento circular
	float velocidadGiro = 0.1f;     // velocidad del giro
	float centroX = -25.0f;          // centro de la circunferencia
	float centroZ = 260.0f;         // centro de la circunferencia
	float centro2X = 30.0f;          // centro de la circunferencia
	float centro2Z = 270.0f;         // centro de la circunferencia
	float posX;
	float posZ;
	float pos2x;
	float pos2z;
	float rotY;

	float anguloExt;

	bool isDay = false;
	double lastSwitchTime = 0.0;
	const double switchInterval = 30.0; // 2 minutos (120 segundos)
	glm::vec4 posLuz;
	glm::vec4 posLuz2;
	glm::vec4 posLuz3;
	glm::vec4 posLuz4;
	glm::mat4 modelBote;

	// Movimiento cuadrado para NPC1
	static float npc1_movX = 0.0f;
	static float npc1_movZ = 0.0f;
	static int npc1_direccion = 0;      // 0=frente(Z+), 1=derecha(X+), 2=atrás(Z-), 3=izquierda(X-)
	static float npc1_velocidad = 0.1f; // velocidad de movimiento
	static float npc1_limite = 150.0f;   // tamaño del cuadrado
	static float npc1_rotActual = 0.0f; // rotación actual del cuerpo
	static float npc1_rotObjetivo = 0.0f; // rotación hacia la que gira
	static float npc1_velRot = 2.0f;    // velocidad de giro (°/frame)

	// Movimiento cuadrado para NPC2
	static float npc2_movX = 0.0f;
	static float npc2_movZ = 0.0f;
	static int npc2_direccion = 0;       // 0=+Z, 1=-X, 2=-Z, 3=+X
	static float npc2_velocidad = 0.1f;  // velocidad de desplazamiento
	static float npc2_limite = 150.0f;    // tamaño del cuadrado
	static float npc2_rotActual = 0.0f;  // rotación actual
	static float npc2_rotObjetivo = 0.0f;// rotación deseada
	static float npc2_velRot = 2.0f;     // velocidad del giro (grados por frame)

	// Control manual de reflectores
	bool reflectoresEncendidos = true;
	static bool tecla9Presionada = false;
	static bool tecla0Presionada = false;

	// --- Animación jaguares ---
	bool animarJaguares = false;   // control por teclado
	float anguloJaguar = 0.0f;
	float pataAngulo = 0.0f;
	bool subiendoJaguar = true;
	float velocidad;

	glm::mat4 viewMatrix;
	glm::vec3 camPos;
	camPos = camAerea.getPosition();

	glm::mat4 skyboxView;


	// -----------------------------------------
// ANIMACIÓN DE PO EN TRAYECTORIA "L"
// -----------------------------------------
	float poPosX = -50.0f;    // posición inicial X
	float poPosZ = 0.0f;      // posición inicial Z
	float poRotY = 180.0f;    // orientación del cuerpo
	float poVelocidad = 0.3f; // velocidad de movimiento
	float poLimiteZ = 150.0f;  // límite de avance en Z
	float poLimiteX = 100.0f;  // límite de avance en X

	// 0 = avanzando en Z, 1 = avanzando en X, 2 = regresando en X, 3 = regresando en Z
	int poFase = 0;

	bool* keys;
	float mouseX;
	float mouseY;
	//leerArchivo();


	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		angulovaria += 0.5f * deltaTime;
		alaAngulo = 15.0f * sin(angulovaria / 1.6);
		anguloExt = 20.0f * sin(angulovaria / 10);



		if (movCoche < 30.0f)
		{
			movCoche -= movOffset * deltaTime;
		}
		rotllanta += rotllantaOffset * deltaTime;


		//Recibir eventos del usuario
		glfwPollEvents();
		keys = mainWindow.getsKeys();
		mouseX = mainWindow.getXChange();
		mouseY = mainWindow.getYChange();

		static bool pressedF3 = false;
		if (keys[GLFW_KEY_F3]) {
			if (!pressedF3) {
				modoCamara = (modoCamara + 1) % 6;
				pressedF3 = true;

				switch (modoCamara) {
				case 0:
					usarCamaraAlex = false;
					printf("Cámara LIBRE ACTIVADA\n");
					break;
				case 1:
					usarCamaraAlex = true;
					printf("Cámara de SEGUIMIENTO ACTIVADA\n");
					break;
				case 2:
					printf("Cámara AÉREA ACTIVADA\n");
					camAerea.setPosition(glm::vec3(180.0f, 120.0f, -10.0f));  // ✅ reposicionar
					break;

				case 3:
					printf("Cámara FIJA DE INTERÉS ACTIVADA\n");
					break;
				case 4:
					printf("Cámara PIRÁMIDE ACTIVADA\n");
					break;
				case 5:
					printf("Cámara AVIARIO ACTIVADA\n");
					break;
				}

			}
		}
		else {
			pressedF3 = false;
		}




		//-------Para Keyframes
		inputKeyframes(mainWindow.getsKeys());

		animate(KeyFrameAjolote, playIndexAjolote, FrameIndexAjolote, i_curr_stepsAjolote,
			i_max_steps, playAjolote, movAjolote_x, movAjolote_y, giroAjolote);

		animate(KeyFrameAguila, playIndexAguila, FrameIndexAguila, i_curr_stepsAguila,
			i_max_steps, playAguila, movAguila_x, movAguila_y, giroAguila);


		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (now - lastSwitchTime >= switchInterval) {
			isDay = !isDay;
			lastSwitchTime = now;

			if (isDay) {
				mainLight = DirectionalLight(
					1.0f, 0.75f, 0.45f,
					1.0f, 0.8f,
					0.0f, -0.8f, -0.3f
				);

				// Apagar todas las farolas
				for (int i = 0; i < 4; i++)
					pointLights[i].TurnOff();
				if (reflectoresEncendidos) {
					spotLights[idxReflectorRing].TurnOff();
					spotLights[idxReflectorJaguares].TurnOff();
				}

			}
			else {
				mainLight = DirectionalLight(
					0.2f, 0.25f, 0.35f,
					0.3f, 0.2f,
					-0.5f, -1.0f, -0.3f
				);

				for (int i = 0; i < 4; i++)
					pointLights[i].TurnOn();
				if (reflectoresEncendidos) {
					spotLights[idxReflectorRing].TurnOn();
					spotLights[idxReflectorJaguares].TurnOn();
				}
			}
		}

		if (modoCamara == 0) {
			camera.keyControl(keys, deltaTime);
			camera.mouseControl(mouseX, mouseY);
			viewMatrix = camera.calculateViewMatrix();
			camPos = camera.getCameraPosition();
			usarCamaraAlex = false;
		}

		// --- Cámara de seguimiento (Alex - 3ª persona) ---
		else if (modoCamara == 1) {
			float vel = 0.3f;

			if (keys[GLFW_KEY_W]) alexPos += glm::vec3(sin(glm::radians(alexYaw)), 0, cos(glm::radians(alexYaw))) * vel;
			if (keys[GLFW_KEY_S]) alexPos -= glm::vec3(sin(glm::radians(alexYaw)), 0, cos(glm::radians(alexYaw))) * vel;
			if (keys[GLFW_KEY_A]) alexPos -= glm::vec3(cos(glm::radians(alexYaw)), 0, -sin(glm::radians(alexYaw))) * vel;
			if (keys[GLFW_KEY_D]) alexPos += glm::vec3(cos(glm::radians(alexYaw)), 0, -sin(glm::radians(alexYaw))) * vel;

			alexYaw += mouseX * 0.1f;
			if (alexYaw > 360.0f) alexYaw -= 360.0f;
			if (alexYaw < 0.0f) alexYaw += 360.0f;

			alexCam.update(alexPos, mouseX);
			viewMatrix = alexCam.getViewMatrix();
			camPos = alexCam.getPosition();
			usarCamaraAlex = true;
		}

		// --- Cámara aérea / dron ---
		else if (modoCamara == 2) {
			camAerea.keyControl(keys, deltaTime);
			viewMatrix = camAerea.getViewMatrix();
			camPos = camAerea.getPosition();
			usarCamaraAlex = false;
		}

		// --- Cámara fija (punto de interés) ---
		else if (modoCamara == 3) {
			viewMatrix = camInteres.getViewMatrix();
			camPos = glm::vec3(200.0f, 60.0f, -30.0f);
			usarCamaraAlex = false;
		}

		else if (modoCamara == 4) { // Pirámide
			viewMatrix = camPiramide.getViewMatrix();
			camPos = glm::vec3(70.0f, 60.0f, 160.0f);
		}
		else if (modoCamara == 5) { // Aviario
			viewMatrix = camAviario.getViewMatrix();
			camPos = glm::vec3(-140.0f, 70.0f, 130.0f);
		}


		skyboxView = glm::mat4(glm::mat3(viewMatrix)); // elimina traslación

		if (isDay)
			skybox.DrawSkybox(skyboxView, projection);
		else
			nightSkybox.DrawSkybox(skyboxView, projection);



		/*glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniform3f(uniformEyePosition, camPos.x, camPos.y, camPos.z);*/

		// --- Movimiento en trayectoria cuadrada del NPC1 ---
		switch (npc1_direccion) {
		case 0: // Avanza hacia Z+
			npc1_movZ += npc1_velocidad;
			if (npc1_movZ >= npc1_limite) {
				npc1_direccion = 1;
				npc1_rotObjetivo = 90.0f;
			}
			break;
		case 1: // Avanza hacia X+
			npc1_movX += npc1_velocidad;
			if (npc1_movX >= npc1_limite) {
				npc1_direccion = 2;
				npc1_rotObjetivo = 180.0f;
			}
			break;
		case 2: // Avanza hacia Z-
			npc1_movZ -= npc1_velocidad;
			if (npc1_movZ <= 0.0f) {
				npc1_direccion = 3;
				npc1_rotObjetivo = 270.0f;
			}
			break;
		case 3: // Avanza hacia X-
			npc1_movX -= npc1_velocidad;
			if (npc1_movX <= 0.0f) {
				npc1_direccion = 0;
				npc1_rotObjetivo = 0.0f;
			}
			break;
		}

		// --- Rotación suave (interpolación)
		if (fabs(npc1_rotObjetivo - npc1_rotActual) > 0.5f) {
			if (npc1_rotActual < npc1_rotObjetivo)
				npc1_rotActual += npc1_velRot;
			else
				npc1_rotActual -= npc1_velRot;
		}

		// --- Movimiento en trayectoria cuadrada del NPC2 ---
		switch (npc2_direccion) {
		case 0: // Avanza hacia +Z
			npc2_movZ += npc2_velocidad;
			if (npc2_movZ >= npc2_limite) {
				npc2_direccion = 1;
				npc2_rotObjetivo = 270.0f; // gira a -X
			}
			break;

		case 1: // Avanza hacia -X
			npc2_movX -= npc2_velocidad;
			if (npc2_movX <= -npc2_limite) {
				npc2_direccion = 2;
				npc2_rotObjetivo = 180.0f; // gira a -Z
			}
			break;

		case 2: // Avanza hacia -Z
			npc2_movZ -= npc2_velocidad;
			if (npc2_movZ <= 0.0f) {
				npc2_direccion = 3;
				npc2_rotObjetivo = 90.0f; // gira a +X
			}
			break;

		case 3: // Avanza hacia +X
			npc2_movX += npc2_velocidad;
			if (npc2_movX >= 0.0f) {
				npc2_direccion = 0;
				npc2_rotObjetivo = 0.0f; // vuelve al frente (+Z)
			}
			break;
		}

		// --- Rotación suave ---
		if (fabs(npc2_rotObjetivo - npc2_rotActual) > 0.5f) {
			if (npc2_rotActual < npc2_rotObjetivo)
				npc2_rotActual += npc2_velRot;
			else
				npc2_rotActual -= npc2_velRot;
		}

		// --- Control de reflectores con teclado ---
		//bool* keys = mainWindow.getsKeys();

		// Tecla 9 → Apagar reflectores
		if (keys[GLFW_KEY_9]) {
			if (!tecla9Presionada) {
				reflectoresEncendidos = false;
				spotLights[idxReflectorRing].TurnOff();
				spotLights[idxReflectorJaguares].TurnOff();
				printf("Reflectores apagados manualmente.\n");
				tecla9Presionada = true;
			}
		}
		else tecla9Presionada = false;

		// Tecla 0 → Encender reflectores
		if (keys[GLFW_KEY_0]) {
			if (!tecla0Presionada) {
				reflectoresEncendidos = true;
				spotLights[idxReflectorRing].TurnOn();
				spotLights[idxReflectorJaguares].TurnOn();
				printf("Reflectores encendidos manualmente.\n");
				tecla0Presionada = true;
			}
		}
		else tecla0Presionada = false;


		static bool tecla8Presionada = false;
		if (keys[GLFW_KEY_8]) {
			if (!tecla8Presionada) {
				animarJaguares = !animarJaguares;
				printf(animarJaguares ? "Animación jaguares ACTIVADA\n" : "Animación jaguares DESACTIVADA\n");
				tecla8Presionada = true;
			}
		}
		else tecla8Presionada = false;

		if (animarJaguares) {
			velocidad = deltaTime / 2;  // velocidad de inclinación del cuerpo

			if (subiendoJaguar) {
				anguloJaguar += velocidad;
				if (anguloJaguar >= 45.0f) {
					anguloJaguar = 45.0f;
					subiendoJaguar = false;  // empieza a bajar
				}
			}
			else {
				anguloJaguar -= velocidad;
				if (anguloJaguar <= 0.0f) {
					anguloJaguar = 0.0f;
					subiendoJaguar = true;   // vuelve a subir
				}
			}
			pataAngulo = 30.0f * sin(angulovaria / 2);
		}



		// -----------------------------------------
// ANIMACIÓN DE PO EN TRAYECTORIA EN "L"
// -----------------------------------------
		switch (poFase) {
		case 0: // Avanza en +Z
			poPosZ += poVelocidad;
			if (poPosZ >= poLimiteZ) {
				poFase = 1;
				poRotY = -90.0f; // gira hacia la derecha (ahora mira +X)
			}
			break;

		case 1: // Avanza en +X
			poPosX += poVelocidad;
			if (poPosX >= poLimiteX) {
				poFase = 2;
				poRotY = -270.0f; // gira para volver (ahora mira -X)
			}
			break;

		case 2: // Regresa en -X
			poPosX -= poVelocidad;
			if (poPosX <= -40.0f) {
				poFase = 3;
				poRotY = 0.0f; // gira para regresar por -Z
			}
			break;

		case 3: // Regresa en -Z
			poPosZ -= poVelocidad;
			if (poPosZ <= 0.0f) {
				poFase = 0;
				poRotY = 180.0f; // vuelve a mirar al frente (inicio)
			}
			break;
		}





		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		// ===================================================
		// ACTUALIZAR UNIFORMS DE CÁMARA SEGÚN MODO ACTIVO
		// ===================================================
		if (modoCamara == 0) { // Libre
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
			glUniform3f(uniformEyePosition,
				camera.getCameraPosition().x,
				camera.getCameraPosition().y,
				camera.getCameraPosition().z);
		}

		else if (modoCamara == 1) { // 3ª persona (Alex)
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(alexCam.getViewMatrix()));
			glUniform3f(uniformEyePosition,
				alexCam.getPosition().x,
				alexCam.getPosition().y,
				alexCam.getPosition().z);
		}

		else if (modoCamara == 2) { // Aérea
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camAerea.getViewMatrix()));
			glUniform3f(uniformEyePosition,
				camAerea.getPosition().x,
				camAerea.getPosition().y,
				camAerea.getPosition().z);
		}

		else if (modoCamara == 3) { // Fija
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camInteres.getViewMatrix()));
			glUniform3f(uniformEyePosition,
				200.0f, 60.0f, -30.0f); // misma posición definida arriba
		}

		else if (modoCamara == 4) {
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camPiramide.getViewMatrix()));
			glUniform3f(uniformEyePosition, 70.0f, 60.0f, 160.0f);
		}
		else if (modoCamara == 5) {
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camAviario.getViewMatrix()));
			glUniform3f(uniformEyePosition, -140.0f, 70.0f, 130.0f);
		}

		// luz ligada a la cámara de tipo flash
		/*lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());*/

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		model = glm::mat4(1.0);
		modelaux = glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		toffset = glm::vec2(0.0f, 0.0f);

		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, -2.0f, 70.0f));
		model = glm::scale(model, glm::vec3(24.5f, 1.0f, 24.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		/*
		=================
		Kung fu Panda
		=================
		*/
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(poPosX, 1.0f, poPosZ));
		model = glm::scale(model, glm::vec3(3.0f));
		model = glm::rotate(model, glm::radians(poRotY), glm::vec3(0.0f, 1.0f, 0.0f)); // rotación automática al regresar
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		poTexture.UseTexture();
		PoBody.RenderModel();

		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BraDerpo.RenderModel();

		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BraIzqPo.RenderModel();

		model = modelaux;
		model = glm::rotate(model, -glm::radians(anguloExt), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PataDerPo.RenderModel();

		model = modelaux;
		model = glm::rotate(model, glm::radians(anguloExt), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PataIzqPo.RenderModel();





		/*Avatar Alex*/
		model = glm::mat4(1.0);
		model = glm::translate(model, alexPos);
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(alexYaw), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(6.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		alexTexture.UseTexture();
		aCuerpo_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.2f, 1.1f, 0.2f));

		model = glm::rotate(model, glm::radians(anguloExt), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		aBrazoDer_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.2f, 1.1f, 0.2f));

		model = glm::rotate(model, -glm::radians(anguloExt), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		aBrazoIzq_M.RenderModel();

		model = modelaux;

		model = glm::rotate(model, -glm::radians(anguloExt), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		aPiernaDer_M.RenderModel();

		model = modelaux;


		model = glm::rotate(model, glm::radians(anguloExt), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		aPiernaIzq_M.RenderModel();

		/*	NPCS	*/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-50.0f + npc1_movX, 4.3f, -30.0f + npc1_movZ));
		model = glm::rotate(model, glm::radians(npc1_rotActual), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		npc1Texture.UseTexture();
		cuerpo1.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.5f));
		model = glm::rotate(model, glm::radians(anguloExt), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brazoDer1.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.2f, 0.0f, 0.5f));
		model = glm::rotate(model, -glm::radians(anguloExt), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brazoIzq1.RenderModel();

		model = modelaux;
		model = glm::rotate(model, -glm::radians(anguloExt), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		piernaDer1.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.2f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(anguloExt), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		piernaIzq1.RenderModel();

		/*
			NPC 2
		*/

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(90.0f, -2.0f, -40.0f)); // posición base del NPC2
		model = glm::translate(model, glm::vec3(npc2_movX, 0.0f, npc2_movZ)); // movimiento cuadrado
		model = glm::rotate(model, glm::radians(npc2_rotActual), glm::vec3(0.0f, 1.0f, 0.0f)); // rotación suave
		model = glm::scale(model, glm::vec3(7.0f));

		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		npc2Texture.UseTexture();
		cuerpo2.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.23f, 1.55f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(anguloExt), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brazoDer2.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.23f, 1.55f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(anguloExt), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brazoIzq2.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.1f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(anguloExt), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		piernaDer2.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.08f, 0.98f, 0.0f));
		model = glm::rotate(model, -glm::radians(anguloExt), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		piernaIzq2.RenderModel();


		/*ILUMINACION*/
// --- FAROLA 1 (ROSA) ---
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(145.0f, -2.0f, 100.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(2.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuertaTexture.UseTexture();
		farola_M.RenderModel();

		// Luz hija jerárquica (rosa)
		{
			posLuz = model * glm::vec4(0.0f, 5.0f, 0.0f, 1.0f);
			pointLights[idxFarola1].SetPosition(glm::vec3(posLuz));
		}

		// --- BOTE hijo de la farola 1 ---
		{
			modelBote = modelaux;
			//modelBote = glm::translate(modelBote, glm::vec3(0.0f, -5.5f, 3.5f)); // al pie de la farola
			modelBote = glm::scale(modelBote, glm::vec3(0.15f));
			modelBote = glm::rotate(modelBote, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBote));
			PuertaTexture.UseTexture();
			bote_M.RenderModel();
		}


		// --- FAROLA 2 (MORADA) ---
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-85.0f, -2.0f, 210.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(2.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -305 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuertaTexture.UseTexture();
		farola_M.RenderModel();

		// Luz hija jerárquica (morada)

		posLuz2 = model * glm::vec4(0.0f, 5.0f, 0.0f, 1.0f);
		pointLights[idxFarola2].SetPosition(glm::vec3(posLuz2));


		// --- BOTE hijo de la farola 2 ---

		modelBote = modelaux;
		modelBote = glm::scale(modelBote, glm::vec3(0.15f));
		modelBote = glm::rotate(modelBote, -305 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBote));
		PuertaTexture.UseTexture();
		bote_M.RenderModel();




		// --- FAROLA 3 (ROJA) ---
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-85.0f, -2.0f, 40.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(2.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuertaTexture.UseTexture();
		farola_M.RenderModel();

		// Luz hija jerárquica (roja)

		glm::vec4 posLuz3 = model * glm::vec4(0.0f, 5.0f, 0.0f, 1.0f);
		pointLights[idxFarola3].SetPosition(glm::vec3(posLuz3));


		// --- BOTE hijo de la farola 3 ---

		modelBote = modelaux;
		modelBote = glm::scale(modelBote, glm::vec3(0.15f));
		modelBote = glm::rotate(modelBote, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBote));
		PuertaTexture.UseTexture();
		bote_M.RenderModel();


		// --- FAROLA 4 (AZUL) ---
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(145.0f, -2.0f, -90.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(2.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuertaTexture.UseTexture();
		farola_M.RenderModel();

		// Luz hija jerárquica (azul)

		posLuz4 = model * glm::vec4(0.0f, 5.0f, 0.0f, 1.0f);
		pointLights[idxFarola4].SetPosition(glm::vec3(posLuz4));


		// --- BOTE hijo de la farola 4 ---

		modelBote = modelaux;
		modelBote = glm::scale(modelBote, glm::vec3(0.15f));
		modelBote = glm::rotate(modelBote, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelBote));
		PuertaTexture.UseTexture();
		bote_M.RenderModel();



		//letro del habitat del aguila
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-80.0f, 5.0f, 118.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		aguila_Letrero.UseTexture();
		LetreroHabitad_M.RenderModel();
		//letro del habitat del ajolote
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-50.0f, 5.0f, 210.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ajolotes_Letrero.UseTexture();
		LetreroHabitad_M.RenderModel();
		//letro del habitat del aviario
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-80.0f, 5.0f, -80.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Aviario_Letrero.UseTexture();
		LetreroHabitad_M.RenderModel();
		//letro del habitat del jaguar
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(135.0f, 5.0f, -60.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Jaguar_Letrero.UseTexture();
		LetreroHabitad_M.RenderModel();
		//letro del habitat de la piramide
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(88.0f, 5.0f, 195.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -135 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Piramide_Letrero.UseTexture();
		LetreroHabitad_M.RenderModel();
		//letro del habitat del ring de lucha
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.0f, 5.0f, -6.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		RingDeLucha_Letrero.UseTexture();
		LetreroHabitad_M.RenderModel();

		//Entrada
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, -2.0f, -110.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		ArcoTexture.UseTexture();
		Arco_M.RenderModel();

		//Puertas
		//Puerta derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(-2.1f, 0.0f, 3.4f));
		//model = glm::rotate(model, -glm::radians(anguloPuerta), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuertaTexture.UseTexture();
		Puerta_Derecha_M.RenderModel();
		//Puerta izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(3.1f, 0.0f, 3.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuertaTexture.UseTexture();
		Puerta_Izquierda_M.RenderModel();

		//Letrero
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Letrero_M.RenderModel();

		// --- LUMBRERA 1 ---
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(38.0, -2.0f, -117.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lumbreraTexture.UseTexture();
		lumbrera.RenderModel();


		// --- LUMBRERA 2 ---
		model = modelaux;
		model = glm::translate(model, glm::vec3(11.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lumbreraTexture.UseTexture();
		lumbrera.RenderModel();

		//aguila
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-120.0f, 51.0f, 115.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(movAguila_x, movAguila_y, 0.0f));
		model = glm::rotate(model, glm::radians(giroAguila), glm::vec3(0, 0, 1));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		aguilaTexture.UseTexture();
		aguila_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.3f, -1.0f, 1.9f));
		model = glm::rotate(model, glm::radians(alaAngulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		alaDer_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.3f, -1.0f, 2.0f));
		model = glm::rotate(model, -glm::radians(alaAngulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		alaIzq_M.RenderModel();

		//nido
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-120.0f, -2.0f, 118.0f));
		model = glm::scale(model, glm::vec3(3.0f, 7.0f, 3.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		nidoTexture.UseTexture();
		nido_M.RenderModel();

		//jaula
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-140.0f, -2.0f, 118.0f));
		model = glm::scale(model, glm::vec3(29.0f, 20.0f, 45.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuertaTexture.UseTexture();
		jaula_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-120.0f, -2.0f, 100.0f));
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbol2Texture.UseTexture();
		arbol2_M.RenderModel();

		//habitat ajolote
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.0f, -2.0f, 250.0f));
		model = glm::scale(model, glm::vec3(6.6f, 6.6f, 6.5f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		habitat_ajoloteTexture.UseTexture();
		habitat_ajolote_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 5.0f));
		model = glm::scale(model, glm::vec3(3.0f, 1.0f, 1.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(.0f, 0.0f, 1.0f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		nidoTexture.UseTexture();
		reja_M.RenderModel();


		model = modelaux;
		model = glm::translate(model, glm::vec3(4.2f, -0.8f, -0.6f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.034f, 1.0f, 1.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		aguaTexture.UseTexture();
		agua_M.RenderModel();


		//ajolote

		pos2x = centro2X + 3 * cos(angulovaria * velocidadGiro + 3.14f);
		pos2z = centro2Z + 3 * sin(angulovaria * velocidadGiro + 3.14f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(centroX + movAjolote_x, 1.5f, centroZ + movAjolote_y));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, glm::radians(giroAjolote), glm::vec3(0.0f, 1.0f, 0.0f));

		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		axoloteTexture.UseTexture();
		axolote_M.RenderModel();

		//cola
		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		axoloteTexture.UseTexture();
		axoloteCola_M.RenderModel();

		//pata derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.8f, 0.0f, 0.5f));
		model = glm::rotate(model, 30.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 0.6f * sin(angulovaria / 10), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		axoloteTexture.UseTexture();
		axolotePataDerecha_M.RenderModel();

		//pata izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.8f, 0.0f, -0.5f));
		model = glm::rotate(model, 30.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 0.6f * sin(angulovaria / 10), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		axoloteTexture.UseTexture();
		axolotePataIzquierda_M.RenderModel();

		//pata trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.5f, -0.2f, 0.4f));
		model = glm::rotate(model, -40.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 0.5f * sin(angulovaria / 10), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		axoloteTexture.UseTexture();
		axolotePataTraseraDerecha_M.RenderModel();

		//pata trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.5f, -0.2f, -0.4f));
		model = glm::rotate(model, -40.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 0.5f * sin(angulovaria / 10), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		axoloteTexture.UseTexture();
		axolotePataTraseraIzquierda_M.RenderModel();

		//ajolote2
		model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(25.0f, 1.0f, 190.0f));
		model = glm::translate(model, glm::vec3(pos2x, 1.5f, pos2z));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, rotY, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		axoloteTexture.UseTexture();
		axolote_M.RenderModel();


		//cola
		model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		axoloteTexture.UseTexture();
		axoloteCola_M.RenderModel();

		//pata derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.8f, 0.0f, 0.5f));
		model = glm::rotate(model, 30.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 0.6f * sin(angulovaria / 10), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		axoloteTexture.UseTexture();
		axolotePataDerecha_M.RenderModel();

		//pata izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.8f, 0.0f, -0.5f));
		model = glm::rotate(model, 30.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 0.6f * sin(angulovaria / 10), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		axoloteTexture.UseTexture();
		axolotePataIzquierda_M.RenderModel();

		//pata trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.5f, -0.2f, 0.4f));
		model = glm::rotate(model, -40.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 0.5f * sin(angulovaria / 10), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		axoloteTexture.UseTexture();
		axolotePataTraseraDerecha_M.RenderModel();

		//pata trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.5f, -0.2f, -0.4f));
		model = glm::rotate(model, -40.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 0.5f * sin(angulovaria / 10), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		axoloteTexture.UseTexture();
		axolotePataTraseraIzquierda_M.RenderModel();

		/*
		==========================================================
		#									                     #
		#         H A B I T A D  J A G U A R E S                 #
		#														 #
		==========================================================

		*/
		//tronco
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(165.0f, 2.0f, -12.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		troncoTexture.UseTexture();
		tronco_M.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(200.0f, 34.3f, -7.0f));
		model = glm::rotate(model, -glm::radians(anguloJaguar), glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		jaguarTexture.UseTexture();
		jaguar_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.37f, -4.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		colaJa_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.9f, -1.3f, 1.5f));
		model = glm::rotate(model, glm::radians(pataAngulo), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pataFDer_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.7f, -1.1f, 1.5f));
		model = glm::rotate(model, glm::radians(-pataAngulo), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pataFIzq_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.9f, -0.8f, -3.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pataTDer_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.7f, -0.8f, -3.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pataTIzq_M.RenderModel();



		// ==========================================
		// JAGUAR 2 
		// ==========================================
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(165.0f, 15.0f, 6.0f));
		model = glm::rotate(model, -glm::radians(anguloJaguar), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		jaguarTexture.UseTexture();
		jaguar_M.RenderModel();

		// Cola
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.37f, -4.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		colaJa_M.RenderModel();

		// Patas delanteras
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.9f, -1.3f, 1.5f));
		model = glm::rotate(model, glm::radians(pataAngulo), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pataFDer_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.7f, -1.1f, 1.5f));
		model = glm::rotate(model, glm::radians(-pataAngulo), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pataFIzq_M.RenderModel();

		// Patas traseras
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.9f, -0.8f, -3.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pataTDer_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.7f, -0.8f, -3.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pataTIzq_M.RenderModel();


		// ==========================================
		// JAGUAR 3
		// ==========================================
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(170.0f, 21.0f, -45.0f));
		model = glm::rotate(model, -glm::radians(anguloJaguar), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		jaguarTexture.UseTexture();
		jaguar_M.RenderModel();

		// Cola
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.37f, -4.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		colaJa_M.RenderModel();

		// Patas delanteras
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.9f, -1.3f, 1.5f));
		model = glm::rotate(model, glm::radians(pataAngulo), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pataFDer_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.7f, -1.1f, 1.5f));
		model = glm::rotate(model, glm::radians(-pataAngulo), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pataFIzq_M.RenderModel();

		// Patas traseras
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.9f, -0.8f, -3.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pataTDer_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.7f, -0.8f, -3.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pataTIzq_M.RenderModel();


		// ==========================================
		// JAGUAR 4
		// ==========================================
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(170.0f, 2.0f, 60.0f));
		model = glm::rotate(model, -glm::radians(anguloJaguar), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		jaguarTexture.UseTexture();
		jaguar_M.RenderModel();

		// Cola
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.37f, -4.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		colaJa_M.RenderModel();

		// Patas delanteras
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.9f, -1.3f, 1.5f));
		model = glm::rotate(model, glm::radians(pataAngulo), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pataFDer_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.7f, -1.1f, 1.5f));
		model = glm::rotate(model, glm::radians(-pataAngulo), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pataFIzq_M.RenderModel();

		// Patas traseras
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.9f, -0.8f, -3.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pataTDer_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.7f, -0.8f, -3.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pataTIzq_M.RenderModel();


		//árbol
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(177.0f, -2.0f, 26.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbolTexture.UseTexture();
		arbol_M.RenderModel();
		//árbol 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(180.0f, -2.0f, -63.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbolTexture.UseTexture();
		arbol_M.RenderModel();
		//Rocas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(190.0f, 10.0f, -0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		piedrasTexture.UseTexture();
		piedras_M.RenderModel();

		//Piramide
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(160.0f, -2.0f, 215.0f));
		model = glm::scale(model, glm::vec3(55.0f, 55.0f, 55.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		piramideTexture.UseTexture();
		piramide_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(175.0f, -2.0f, 140.0f));
		model = glm::scale(model, glm::vec3(15.0f, 15.0f, 15.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbusto1Texture.UseTexture();
		arbusto1_M.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbusto1Texture.UseTexture();
		arbusto1_M.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(-5.0f, -3.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbusto1Texture.UseTexture();
		arbusto1_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(90.0f, -2.0f, 270.0f));
		model = glm::scale(model, glm::vec3(50.0f, 50.0f, 50.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbusto2Texture.UseTexture();
		arbusto2_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.2f, 0.3f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbusto2Texture.UseTexture();
		arbusto2_M.RenderModel();
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.4f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arbusto2Texture.UseTexture();
		arbusto2_M.RenderModel();



		//Ring
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, 0.0f, 40.0f));
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ringTexture.UseTexture();
		ring_M.RenderModel();

		//Banca
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, -2.0f, -20.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuertaTexture.UseTexture();
		banca_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(80.0f, -2.0f, 40.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuertaTexture.UseTexture();
		banca_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-40.0f, -2.0f, 40.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuertaTexture.UseTexture();
		banca_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, -2.0f, 100.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PuertaTexture.UseTexture();
		banca_M.RenderModel();


		//Aviario

		glm::mat4 aviaryBase = glm::mat4(1.0f);
		glm::vec3 newAviaryPosition = glm::vec3(-10.5f, 0.0f, -4.0f); // Nueva posición base del grupo
		float aviaryScale = 1.1f; // Nueva escala (85% del original)

		aviaryBase = glm::translate(aviaryBase, newAviaryPosition);
		aviaryBase = glm::scale(aviaryBase, glm::vec3(aviaryScale));

		//AVIARIO

		// 3 ÁRBOLES AVIARIO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-150.0f, -2.0f, -45.0f));
		model = glm::scale(model, glm::vec3(5.5f, 5.5f, 5.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		arbolAviarioTexture.UseTexture();
		arbolAviario_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-140.0f, -2.0f, -45.0f));
		model = glm::scale(model, glm::vec3(5.5f, 5.5f, 5.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		arbolAviarioTexture.UseTexture();
		arbolAviario_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-130.0f, -2.0f, -55.0f));
		model = glm::scale(model, glm::vec3(5.5f, 5.5f, 5.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		arbolAviarioTexture.UseTexture();
		arbolAviario_M.RenderModel();



		//INICIO: 6 ÁRBOLES AVIARIO (Manual)
		arbolAviarioTexture.UseTexture();

		// --- 3 Árboles en la zona original (con más separación)

		// Árbol 1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-150.0f, -2.0f, -40.0f));
		model = glm::scale(model, glm::vec3(5.5f, 5.5f, 5.5f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		arbolAviario_M.RenderModel();

		// Árbol 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f, -2.0f, -35.0f));
		model = glm::scale(model, glm::vec3(5.5f, 5.5f, 5.5f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		arbolAviario_M.RenderModel();


		// --- 3 Árboles en la zona +Z

		// Árbol 4
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-140.0f, -2.0f, 5.0f));
		model = glm::scale(model, glm::vec3(5.5f, 5.5f, 5.5f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		arbolAviario_M.RenderModel();

		// Árbol 5
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-125.0f, -2.0f, 10.0f));
		model = glm::scale(model, glm::vec3(5.5f, 5.5f, 5.5f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		arbolAviario_M.RenderModel();

		// Árbol 6
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f, -2.0f, 10.0f));
		model = glm::scale(model, glm::vec3(5.5f, 5.5f, 5.5f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		arbolAviario_M.RenderModel();


		// FIN DE ÁRBOLES AVIARIO


		// GUACAMAYA Roja

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-90.0f, 10.0f, -15.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		guacamayaRojaTexture2.UseTexture();
		guacamayaRojaTexture.UseTexture();
		guacamayaRoja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-80.0f, 10.0f, -25.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		guacamayaRojaTexture2.UseTexture();
		guacamayaRojaTexture.UseTexture();
		guacamayaRoja_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-70.0f, 10.0f, -15.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		guacamayaRojaTexture2.UseTexture();
		guacamayaRojaTexture.UseTexture();
		guacamayaRoja_M.RenderModel();

		// PÁJAROS AZULES (Suelo y Ramas)
		pajaroTexture.UseTexture();

		// 2 PÁJAROS EN SUELO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-160.0f, -2.0f, -30.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		pajaro_M.RenderModel();

		//Pajaro sobrepuesto al arbol
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-135.0f, -2.0f, -40.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		pajaro_M.RenderModel();

		// --- 2 PÁJAROS EN RAMAS ---
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-155.0f, 20.0f, -40.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(25.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		pajaro_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-145.0f, 22.0f, -50.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(245.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		pajaro_M.RenderModel();

		// --- FIN  PÁJAROS AZULES

		// FIN GUACAMAYA ROJA

		// 3 PÁJAROS AVIARIO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-155.0f, 21.5f, -56.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model));
		pajaroTexture.UseTexture();
		pajaro_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-155.0f, -2.0f, -45.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -135 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model));
		pajaroTexture.UseTexture();
		pajaro_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-158.0f, -2.0f, -45.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model));
		pajaroTexture.UseTexture();
		pajaro_M.RenderModel();
		// FIN DE PÁJAROS AVIARIO

		//Dibuja TODOS los Pájaros Azules (Suelo y Ramas)
		pajaroTexture.UseTexture();

		//DOMO
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-120.0f, -2.0f, -20.0f));
		model = glm::scale(model, glm::vec3(1.6f, 1.6f, 1.6f));
		model = glm::rotate(model, glm::radians(88.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		domoTexture.UseTexture();
		domo_M.RenderModel();

		// 4 Palmas Macetas 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-60.0f, -2.0f, -20.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		palmaTexture.UseTexture();
		palma_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-60.0f, -2.0f, -23.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		palma_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-68.0f, -2.0f, -49.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		palma_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-70.0f, -2.0f, -51.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		palma_M.RenderModel();
		//Fin Palma Macetas

		//CASAS PAJAROS aleatorias con posiciones fijas

		//  10 CASAS DE PÁJAROS ---
		casaPajaroTexture.UseTexture();

		// Fila 1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-120.0f, -2.0f, -30.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		casaPajaro_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-110.0f, -2.0f, -30.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		casaPajaro_M.RenderModel();

		// Fila 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-120.0f, -2.0f, -40.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		casaPajaro_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-110.0f, -2.0f, -40.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		casaPajaro_M.RenderModel();

		// Fila 3
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-120.0f, -2.0f, -50.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		casaPajaro_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-110.0f, -2.0f, -50.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		casaPajaro_M.RenderModel();

		// Fila 4
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-120.0f, -2.0f, -60.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		casaPajaro_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-110.0f, -2.0f, -60.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		casaPajaro_M.RenderModel();

		// Fila 5 (2 casas)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-120.0f, -2.0f, -70.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		casaPajaro_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-110.0f, -2.0f, -70.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		casaPajaro_M.RenderModel();
		// --- FIN: 34 CASAS DE PÁJAROS (Manual) ---

		//Fin CASAS PAJAROS

		// LOROS
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-95.0f, 1.0f, -5.0f));
		model = glm::scale(model, glm::vec3(0.125f, 0.125f, 0.125f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		demoParrotTexture.UseTexture();
		demoParrot_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-90.0f, 1.0f, -5.0f));
		model = glm::scale(model, glm::vec3(0.125f, 0.125f, 0.125f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		demoParrotTexture2.UseTexture();
		demoParrot_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f, 1.0f, -5.0f));
		model = glm::scale(model, glm::vec3(0.125f, 0.125f, 0.125f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		demoParrotTexture2.UseTexture();
		demoParrot_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-105.0f, 1.0f, -5.0f));
		model = glm::scale(model, glm::vec3(0.125f, 0.125f, 0.125f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		demoParrotTexture.UseTexture();
		demoParrot_M.RenderModel();


		//Fin loros



		//AVES AZULES
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-155.0f, 3.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		aveAzulTexture.UseTexture();
		aveAzul_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-165.0f, 3.0f, 5.0f));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		aveAzulTexture.UseTexture();
		aveAzul_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-160.0f, 3.0f, 10.0f));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		aveAzulTexture.UseTexture();
		aveAzul_M.RenderModel();
		//Fin aves azules

		//Empleo de troncos y árboles adicionales
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-85.0f, -16.5f, -7.5f));
		model = glm::scale(model, glm::vec3(8.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		troncoAviarioTexture.UseTexture();
		troncoAviario_M.RenderModel();

		// 20 flores rosas (aleatorias)

		// --- INICIO: 12 FLORES ROSAS (Manual) ---
		arbusto3Texture.UseTexture();

		// Fila 1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-165.0f, -2.0f, 12.0f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		arbusto3_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-160.0f, -2.0f, 15.0f));
		model = glm::rotate(model, glm::radians(80.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		arbusto3_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-155.0f, -2.0f, 15.0f));
		model = glm::rotate(model, glm::radians(120.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		arbusto3_M.RenderModel();

		// Fila 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-165.0f, -2.0f, 5.0f));
		model = glm::rotate(model, glm::radians(240.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		arbusto3_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-160.0f, -2.0f, 5.0f));
		model = glm::rotate(model, glm::radians(280.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		arbusto3_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-155.0f, -2.0f, 5.0f));
		model = glm::rotate(model, glm::radians(320.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		arbusto3_M.RenderModel();

		// Fila 3
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-165.0f, -2.0f, -10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		arbusto3_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-160.0f, -2.0f, -10.0f));
		model = glm::rotate(model, glm::radians(130.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		arbusto3_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-155.0f, -2.0f, -10.0f));
		model = glm::rotate(model, glm::radians(170.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		arbusto3_M.RenderModel();

		// Fila 4
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-165.0f, -2.0f, -15.0f));
		model = glm::rotate(model, glm::radians(290.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		arbusto3_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-160.0f, -2.0f, -15.0f));
		model = glm::rotate(model, glm::radians(330.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		arbusto3_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-155.0f, -2.0f, -15.0f));
		model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		arbusto3_M.RenderModel();

		// --- FIN: 12 FLORES ROSAS (Manual) ---

		// --- INICIO: 15 ARBUSTOS (1 y 2) MANUALES ---

		// Fila 1 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-115.0f, -2.0f, -35.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		arbusto1Texture.UseTexture();
		arbusto1_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-105.0f, -2.0f, -35.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		arbusto2Texture.UseTexture();
		arbusto2_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-95.0f, -2.0f, -35.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		arbusto1Texture.UseTexture();
		arbusto1_M.RenderModel();

		// Fila 2 (Z = -45.0f)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-115.0f, -2.0f, -45.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		arbusto2Texture.UseTexture();
		arbusto2_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-105.0f, -2.0f, -45.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		arbusto1Texture.UseTexture();
		arbusto1_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-95.0f, -2.0f, -45.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		arbusto2Texture.UseTexture();
		arbusto2_M.RenderModel();


		// Fila 3 (Z = -55.0f)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-115.0f, -2.0f, -55.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		arbusto1Texture.UseTexture();
		arbusto1_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-105.0f, -2.0f, -55.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		arbusto2Texture.UseTexture();
		arbusto2_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-95.0f, -2.0f, -55.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		arbusto1Texture.UseTexture();
		arbusto1_M.RenderModel();



		// Fila 4 (Z = -65.0f)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-115.0f, -2.0f, -65.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		arbusto2Texture.UseTexture();
		arbusto2_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-105.0f, -2.0f, -65.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		arbusto1Texture.UseTexture();
		arbusto1_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-95.0f, -2.0f, -65.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		arbusto2Texture.UseTexture();
		arbusto2_M.RenderModel();


		// Fila 5 (Z = -75.0f)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-115.0f, -2.0f, -70.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		arbusto1Texture.UseTexture();
		arbusto1_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-105.0f, -2.0f, -70.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		arbusto2Texture.UseTexture();
		arbusto2_M.RenderModel();

		model = glm::mat4(1.L);
		model = glm::translate(model, glm::vec3(-90.0f, -2.0f, -65.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aviaryBase * model)); // MODIFICADO
		arbusto1Texture.UseTexture();
		arbusto1_M.RenderModel();

		// Cuadro - agregar en la posición deseada
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(160.0f, 5.0f, 3.0f)); // Posición X, Y, Z
		model = glm::scale(model, glm::vec3(13.0f, 16.0f, 19.0f)); // Escala ajustable
		model = glm::rotate(model, 0.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación si es necesario
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cuadroTexture.UseTexture();
		//Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		cuadro_M.RenderModel();



		// Alberca/Estanque
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(160.0f, -2.0f, 60.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		aguaTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		AlbercaJa_M.RenderModel();


		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}


void inputKeyframes(bool* keys)
{
	// --- CAMBIAR MODELO ACTIVO ---
	if (keys[GLFW_KEY_F1]) {
		archivoActual = "keyframes_ajolote.txt";

	}
	if (keys[GLFW_KEY_F2]) {
		archivoActual = "keyframes_aguila.txt";
	}
	FRAME* KeyFrameActivo;
	int* FrameIndexActivo;
	float* movX;
	float* movY;
	float* giro;
	int* playIndex;
	int* i_curr_steps;
	bool* play;

	if (archivoActual == "keyframes_ajolote.txt") {
		KeyFrameActivo = KeyFrameAjolote;
		FrameIndexActivo = &FrameIndexAjolote;
		movX = &movAjolote_x;
		movY = &movAjolote_y;
		giro = &giroAjolote;
		playIndex = &playIndexAjolote;
		i_curr_steps = &i_curr_stepsAjolote;
		play = &playAjolote;
	}
	else {
		KeyFrameActivo = KeyFrameAguila;
		FrameIndexActivo = &FrameIndexAguila;
		movX = &movAguila_x;
		movY = &movAguila_y;
		giro = &giroAguila;
		playIndex = &playIndexAguila;
		i_curr_steps = &i_curr_stepsAguila;
		play = &playAguila;
	}
	static bool pressed1 = false;
	if (keys[GLFW_KEY_1]) {
		if (!pressed1) {
			modoEdicion = !modoEdicion;
			modoRotacion = false;
			printf(modoEdicion ? "Modo edicion activado.\n" : "Modo edición desactivado.\n");
			pressed1 = true;
		}
	}
	else pressed1 = false;

	static bool pressedR = false;
	if (modoEdicion && keys[GLFW_KEY_R]) {
		if (!pressedR) {
			modoRotacion = !modoRotacion;
			printf(modoRotacion ? "Modo rotacion activado.\n" : "Modo traslación activado.\n");
			pressedR = true;
		}
	}
	else pressedR = false;

	if (modoEdicion) {
		float vel = 0.05f, velRot = 1.5f;
		if (!modoRotacion) {
			if (keys[GLFW_KEY_H]) *movX += vel;
			if (keys[GLFW_KEY_K]) *movX -= vel;
			if (keys[GLFW_KEY_U]) *movY += vel;
			if (keys[GLFW_KEY_J]) *movY -= vel;
		}
		else {
			if (keys[GLFW_KEY_LEFT]) *giro -= velRot;
			if (keys[GLFW_KEY_RIGHT]) *giro += velRot;
		}
	}

	// --- GUARDAR FRAME ---
	static bool pressedL = false;
	if (keys[GLFW_KEY_L]) {
		if (!pressedL) {
			saveFrame(archivoActual, KeyFrameActivo, *FrameIndexActivo, *movX, *movY, *giro);
			pressedL = true;
		}
	}
	else pressedL = false;

	// --- MODIFICAR FRAME ---
	static bool pressedM = false;
	if (keys[GLFW_KEY_M]) {
		if (!pressedM && *FrameIndexActivo > 0) {
			int modIndex = *FrameIndexActivo - 1;
			(KeyFrameActivo)[modIndex].movX = *movX;
			(KeyFrameActivo)[modIndex].movY = *movY;
			(KeyFrameActivo)[modIndex].giro = *giro;
			actualizarArchivo(archivoActual, modIndex, KeyFrameActivo[modIndex]);
			pressedM = true;
		}
	}
	else pressedM = false;

	// --- BORRAR FRAME ---
	static bool pressedB = false;
	if (keys[GLFW_KEY_B]) {
		if (!pressedB && *FrameIndexActivo > 0) {
			borrarFrame(archivoActual, *FrameIndexActivo - 1);
			(*FrameIndexActivo)--;
			pressedB = true;
		}
	}
	else pressedB = false;

	// --- REPRODUCIR ANIMACIÓN ---
	if (keys[GLFW_KEY_SPACE]) {
		if (reproducirAnimacion < 1) {
			if (!*play) {
				leerArchivo(archivoActual, KeyFrameActivo, *FrameIndexActivo);
				if (*FrameIndexActivo > 1) {
					resetElements(KeyFrameActivo, *movX, *movY, *giro);
					interpolation(KeyFrameActivo, *playIndex, i_max_steps);
					*play = true;
					*playIndex = 0;
					*i_curr_steps = 0;
					reproducirAnimacion++;
				}
				else
					printf("No hay suficientes frames en %s para reproducir.\n", archivoActual.c_str());
			}
		}
	}
	else reproducirAnimacion = 0;
}