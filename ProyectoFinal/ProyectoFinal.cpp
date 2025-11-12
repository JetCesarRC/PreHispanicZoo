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
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "ShaderLight.h"
#include "Camera.h"
#include "Texture.h"
#include "Model.h"
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

// NUEVO: manejo de keyframes encapsulado
#include "Frame.h"

const float toRadians = 3.14159265f / 180.0f;

// --------- Variables animación simples ---------
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;
bool  avanza;
float angulovaria = 0.0f;
float alaAngulo = 0.0f;

// --------- Keyframes (USANDO Frame) ---------
Frame framesAvion("keyframes.txt");
Frame framesAguila("keyframes_aguila.txt");
Frame* activo = &framesAvion;

// control de reproducción / edición
int  i_max_steps = 100;
int  reproducirAnimacion = 0;
bool modoEdicion = false;
bool modoRotacion = false;

// --------- Ventana / render base ---------
Window mainWindow;
std::vector<Mesh*>  meshList;
std::vector<Shader> shaderList;
Camera camera;

// --------- Texturas ---------
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

// --------- Modelos ---------
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
Model arbolAviario_M;
Model pajaro_M;
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

// --------- Skybox ---------
Skybox skybox;
Skybox nightSkybox;

// --------- Materiales ---------
Material Material_brillante;
Material Material_opaco;

// --------- Timing ---------
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// --------- Luces ---------
DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight  spotLights[MAX_SPOT_LIGHTS];

// Shaders
static const char* vShader = "Shaders/ShaderLight.vert";
static const char* fShader = "Shaders/ShaderLight.frag";

// Prototipo (reutilizamos tu firma)
void inputKeyframes(bool* keys);

// ----------------- Utilidades -----------------
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount,
    GLfloat* vertices, unsigned int verticeCount,
    unsigned int vLength, unsigned int normalOffset)
{
    for (size_t i = 0; i < indiceCount; i += 3)
    {
        unsigned int in0 = indices[i] * vLength;
        unsigned int in1 = indices[i + 1] * vLength;
        unsigned int in2 = indices[i + 2] * vLength;
        glm::vec3 v1(vertices[in1] - vertices[in0],
            vertices[in1 + 1] - vertices[in0 + 1],
            vertices[in1 + 2] - vertices[in0 + 2]);
        glm::vec3 v2(vertices[in2] - vertices[in0],
            vertices[in2 + 1] - vertices[in0 + 1],
            vertices[in2 + 2] - vertices[in0 + 2]);
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

// ----------------- Geometrías básicas -----------------
void CreateObjects()
{
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    GLfloat vertices[] = {
        // x      y      z         u     v        nx    ny    nz
        -1.0f, -1.0f, -0.6f,    0.0f, 0.0f,     0.0f, 0.0f, 0.0f,
         0.0f, -1.0f,  1.0f,    0.5f, 0.0f,     0.0f, 0.0f, 0.0f,
         1.0f, -1.0f, -0.6f,    1.0f, 0.0f,     0.0f, 0.0f, 0.0f,
         0.0f,  1.0f,  0.0f,    0.5f, 1.0f,     0.0f, 0.0f, 0.0f
    };

    unsigned int floorIndices[] = {
        0, 2, 1,
        1, 2, 3
    };

    GLfloat floorVertices[] = {
        -10.0f, 0.0f, -10.0f,   0.0f, 0.0f,     0.0f, -1.0f, 0.0f,
         10.0f, 0.0f, -10.0f,   10.0f, 0.0f,    0.0f, -1.0f, 0.0f,
        -10.0f, 0.0f,  10.0f,   0.0f, 10.0f,    0.0f, -1.0f, 0.0f,
         10.0f, 0.0f,  10.0f,   10.0f, 10.0f,   0.0f, -1.0f, 0.0f
    };

    unsigned int vegetacionIndices[] = {
        0, 1, 2,  0, 2, 3,
        4, 5, 6,  4, 6, 7
    };

    GLfloat vegetacionVertices[] = {
        -0.5f, -0.5f, 0.0f,     0.0f, 0.0f,     0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,     1.0f, 0.0f,     0.0f, 0.0f, 0.0f,
         0.5f,  0.5f, 0.0f,     1.0f, 1.0f,     0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f,     0.0f, 1.0f,     0.0f, 0.0f, 0.0f,

         0.0f, -0.5f, -0.5f,    0.0f, 0.0f,     0.0f, 0.0f, 0.0f,
         0.0f, -0.5f,  0.5f,    1.0f, 0.0f,     0.0f, 0.0f, 0.0f,
         0.0f,  0.5f,  0.5f,    1.0f, 1.0f,     0.0f, 0.0f, 0.0f,
         0.0f,  0.5f, -0.5f,    0.0f, 1.0f,     0.0f, 0.0f, 0.0f,
    };

    unsigned int flechaIndices[] = { 0,1,2, 0,2,3 };
    GLfloat flechaVertices[] = {
        -0.5f, 0.0f, 0.5f,      0.0f, 0.0f,     0.0f, -1.0f, 0.0f,
         0.5f, 0.0f, 0.5f,      1.0f, 0.0f,     0.0f, -1.0f, 0.0f,
         0.5f, 0.0f,-0.5f,      1.0f, 1.0f,     0.0f, -1.0f, 0.0f,
        -0.5f, 0.0f,-0.5f,      0.0f, 1.0f,     0.0f, -1.0f, 0.0f,
    };

    unsigned int scoreIndices[] = { 0,1,2, 0,2,3 };
    GLfloat scoreVertices[] = {
        -0.5f, 0.0f, 0.5f,      0.0f, 0.0f,     0.0f, -1.0f, 0.0f,
         0.5f, 0.0f, 0.5f,      1.0f, 0.0f,     0.0f, -1.0f, 0.0f,
         0.5f, 0.0f,-0.5f,      1.0f, 1.0f,     0.0f, -1.0f, 0.0f,
        -0.5f, 0.0f,-0.5f,      0.0f, 1.0f,     0.0f, -1.0f, 0.0f,
    };

    unsigned int numeroIndices[] = { 0,1,2, 0,2,3 };
    GLfloat numeroVertices[] = {
        -0.5f, 0.0f, 0.5f,      0.0f, 0.67f,    0.0f, -1.0f, 0.0f,
         0.5f, 0.0f, 0.5f,      0.25f,0.67f,    0.0f, -1.0f, 0.0f,
         0.5f, 0.0f,-0.5f,      0.25f,1.0f,     0.0f, -1.0f, 0.0f,
        -0.5f, 0.0f,-0.5f,      0.0f, 1.0f,     0.0f, -1.0f, 0.0f,
    };

    Mesh* obj1 = new Mesh(); obj1->CreateMesh(vertices, indices, 32, 12); meshList.push_back(obj1);
    Mesh* obj2 = new Mesh(); obj2->CreateMesh(vertices, indices, 32, 12); meshList.push_back(obj2);
    Mesh* obj3 = new Mesh(); obj3->CreateMesh(floorVertices, floorIndices, 32, 6); meshList.push_back(obj3);
    Mesh* obj4 = new Mesh(); obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12); meshList.push_back(obj4);
    Mesh* obj5 = new Mesh(); obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6); meshList.push_back(obj5);
    Mesh* obj6 = new Mesh(); obj6->CreateMesh(scoreVertices, scoreIndices, 32, 6); meshList.push_back(obj6);
    Mesh* obj7 = new Mesh(); obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6); meshList.push_back(obj7);
}

void CreateShaders()
{
    Shader* shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
}

// ----------------- INPUT KEYFRAMES (usa Frame) -----------------
void inputKeyframes(bool* keys)
{
    // Cambiar "track" activo
    if (keys[GLFW_KEY_F1]) activo = &framesAvion;
    if (keys[GLFW_KEY_F2]) activo = &framesAguila;

    static bool pressed1 = false;
    if (keys[GLFW_KEY_1]) {
        if (!pressed1) {
            modoEdicion = !modoEdicion;
            modoRotacion = false;
            printf(modoEdicion ? "Modo edición activado.\n" : "Modo edición desactivado.\n");
            pressed1 = true;
        }
    }
    else pressed1 = false;

    static bool pressedR = false;
    if (modoEdicion && keys[GLFW_KEY_R]) {
        if (!pressedR) {
            modoRotacion = !modoRotacion;
            printf(modoRotacion ? "Modo rotación activado.\n" : "Modo traslación activado.\n");
            pressedR = true;
        }
    }
    else pressedR = false;

    if (modoEdicion) {
        float vel = 0.05f, velRot = 1.5f;
        if (!modoRotacion) {
            if (keys[GLFW_KEY_H]) activo->setMovX(activo->getMovX() + vel);
            if (keys[GLFW_KEY_K]) activo->setMovX(activo->getMovX() - vel);
            if (keys[GLFW_KEY_U]) activo->setMovY(activo->getMovY() + vel);
            if (keys[GLFW_KEY_J]) activo->setMovY(activo->getMovY() - vel);
        }
        else {
            if (keys[GLFW_KEY_LEFT])  activo->setGiro(activo->getGiro() - velRot);
            if (keys[GLFW_KEY_RIGHT]) activo->setGiro(activo->getGiro() + velRot);
        }
    }

    // Guardar
    static bool pressedL = false;
    if (keys[GLFW_KEY_L]) {
        if (!pressedL) {
            activo->saveCurrent(activo->getMovX(), activo->getMovY(), activo->getGiro());
            pressedL = true;
        }
    }
    else pressedL = false;

    // Modificar último
    static bool pressedM = false;
    if (keys[GLFW_KEY_M]) {
        if (!pressedM) {
            activo->modifyLast(activo->getMovX(), activo->getMovY(), activo->getGiro());
            pressedM = true;
        }
    }
    else pressedM = false;

    // Borrar último
    static bool pressedB = false;
    if (keys[GLFW_KEY_B]) {
        if (!pressedB) {
            activo->deleteLast();
            pressedB = true;
        }
    }
    else pressedB = false;

    // Reproducir
    if (keys[GLFW_KEY_SPACE]) {
        if (reproducirAnimacion < 1) {
            activo->loadFromDisk();
            activo->startPlayback(i_max_steps);
            reproducirAnimacion++;
        }
    }
    else reproducirAnimacion = 0;
}

// ----------------- MAIN -----------------
int main()
{
    mainWindow = Window(1366, 768);
    mainWindow.Initialise();

    CreateObjects();
    CreateShaders();

    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        -60.0f, 0.0f, 0.5f, 0.5f);

    // --- Carga de texturas (idéntico a tu versión) ---
    pisoTexture = Texture("Textures/Nordic_Moss_se4rwei_1K_BaseColor.jpg");     pisoTexture.LoadTextureA();
    ArcoTexture = Texture("Textures/Mossy_Stone_Floor_veilfjxn_1K_BaseColor.jpg"); ArcoTexture.LoadTextureA();
    PuertaTexture = Texture("Textures/Rusty_Metal_Sheet_tj4kedvcw_1K_BaseColor.jpg"); PuertaTexture.LoadTextureA();
    lumbreraTexture = Texture("Textures/lumbrera.png");   lumbreraTexture.LoadTextureA();
    aguilaTexture = Texture("Textures/Fogel_Mat_Diffuse_Color.png"); aguilaTexture.LoadTextureA();
    habitat_ajoloteTexture = Texture("Textures/rock_wall_13_diff_4k.jpg"); habitat_ajoloteTexture.LoadTextureA();
    nidoTexture = Texture("Textures/wood_table_diff_4k.jpg"); nidoTexture.LoadTextureA();
    troncoTexture = Texture("Textures/pine_bark_diff_4k.png");  troncoTexture.LoadTextureA();
    jaguarTexture = Texture("Textures/Jaguar_BaseColor.png");   jaguarTexture.LoadTextureA();
    arbolTexture = Texture("Textures/bark_willow_diff_4k.png"); arbolTexture.LoadTextureA();
    piedrasTexture = Texture("Textures/CavePlatform4_Base_Specular.png"); piedrasTexture.LoadTextureA();
    ringTexture = Texture("Textures/Lona.png"); ringTexture.LoadTextureA();
    axoloteTexture = Texture("Textures/ajoloteTexture.png"); axoloteTexture.LoadTextureA();
    piramideTexture = Texture("Textures/Stone_Wall_tinifauaw_1K_BaseColor.jpg"); piramideTexture.LoadTextureA();
    arbolAviarioTexture = Texture("Textures/Tree_Bark_vimmdcofw_1K_BaseColor.jpg"); arbolAviarioTexture.LoadTextureA();
    pajaroTexture = Texture("Textures/12213_bird_diffuse.jpg"); pajaroTexture.LoadTextureA();
    arbusto1Texture = Texture("Textures/TX_AlexandraPalm_ALB.jpg"); arbusto1Texture.LoadTextureA();
    arbusto2Texture = Texture("Textures/3 leaves_Color.png"); arbusto2Texture.LoadTextureA();
    arbusto3Texture = Texture("Textures/Grass.jpg"); arbusto3Texture.LoadTextureA();
    arbol2Texture = Texture("Textures/tree2.low_Material_BaseColor.jpg"); arbol2Texture.LoadTextureA();
    aguaTexture = Texture("Textures/3DS - Bravely Second_ End Layer - Environment - Water.png"); aguaTexture.LoadTextureA();
    cuadroTexture = Texture("Textures/blue_metal_plate_diff_4k.png"); cuadroTexture.LoadTextureA();
    AlbercaJaTexture = Texture("Textures/3DS - Bravely Second_ End Layer - Environment - Water.png"); AlbercaJaTexture.LoadTextureA();

    aguila_Letrero = Texture("Textures/aguila_letrero.png");           aguila_Letrero.LoadTextureA();
    ajolotes_Letrero = Texture("Textures/ajolotes_Letrero.png");         ajolotes_Letrero.LoadTextureA();
    Aviario_Letrero = Texture("Textures/Aviario_Letrero.png");          Aviario_Letrero.LoadTextureA();
    Jaguar_Letrero = Texture("Textures/Jaguares_Letrero.png");         Jaguar_Letrero.LoadTextureA();
    Piramide_Letrero = Texture("Textures/Piramide_Letrero.png");         Piramide_Letrero.LoadTextureA();
    RingDeLucha_Letrero = Texture("Textures/RingDeLucha_Letrero.png");    RingDeLucha_Letrero.LoadTextureA();

    // --- Carga de modelos  ---
    Arco_M = Model();           Arco_M.LoadModel("Models/Arco.fbx");
    Letrero_M = Model();        Letrero_M.LoadModel("Models/Letrero.fbx");
    Puerta_Derecha_M = Model(); Puerta_Derecha_M.LoadModel("Models/Puerta_Der.fbx");
    Puerta_Izquierda_M = Model(); Puerta_Izquierda_M.LoadModel("Models/Puerta_Izq.fbx");
    lumbrera = Model();         lumbrera.LoadModel("Models/Lumbrera.fbx");
    aguila_M = Model();         aguila_M.LoadModel("Models/aguila.fbx");
    alaDer_M = Model();         alaDer_M.LoadModel("Models/alaDerecha.fbx");
    alaIzq_M = Model();         alaIzq_M.LoadModel("Models/alaIzquierda.fbx");
    habitat_ajolote_M = Model(); habitat_ajolote_M.LoadModel("Models/habitat_ajolote.fbx");
    nido_M = Model();           nido_M.LoadModel("Models/nido.fbx");
    tronco_M = Model();         tronco_M.LoadModel("Models/tronco.fbx");
    jaguar_M = Model();         jaguar_M.LoadModel("Models/jaguar.fbx");
    arbol_M = Model();          arbol_M.LoadModel("Models/arbol.fbx");
    piedras_M = Model();        piedras_M.LoadModel("Models/piedras.fbx");
    ring_M = Model();           ring_M.LoadModel("Models/Ring.fbx");
    axolote_M = Model();        axolote_M.LoadModel("Models/axolote.fbx");
    axoloteCola_M = Model();    axoloteCola_M.LoadModel("Models/colaAxolote.fbx");
    axolotePataDerecha_M = Model(); axolotePataDerecha_M.LoadModel("Models/pataDerechaAxolote.fbx");
    axolotePataIzquierda_M = Model(); axolotePataIzquierda_M.LoadModel("Models/pataIzquierdaAxolote.fbx");
    axolotePataTraseraDerecha_M = Model(); axolotePataTraseraDerecha_M.LoadModel("Models/pataDerechaTraseraAxolote.fbx");
    axolotePataTraseraIzquierda_M = Model(); axolotePataTraseraIzquierda_M.LoadModel("Models/pataIzquierdaTraseraAxolote.fbx");
    piramide_M = Model();       piramide_M.LoadModel("Models/piramide.fbx");
    arbolAviario_M = Model();   arbolAviario_M.LoadModel("Models/arbolAves.fbx");
    arbusto1_M = Model();       arbusto1_M.LoadModel("Models/Arbustos/arbusto1.fbx");
    arbusto2_M = Model();       arbusto2_M.LoadModel("Models/Arbustos/arbusto2.fbx");
    arbusto3_M = Model();       arbusto3_M.LoadModel("Models/Arbustos/flor.fbx");
    arbol2_M = Model();         arbol2_M.LoadModel("Models/arbol2.fbx");
    agua_M = Model();           agua_M.LoadModel("Models/agua.fbx");
    reja_M = Model();           reja_M.LoadModel("Models/reja-ajolote.fbx");
    jaula_M = Model();          jaula_M.LoadModel("Models/jaula.fbx");
    cuadro_M = Model();         cuadro_M.LoadModel("Models/cuadro.fbx");
    AlbercaJa_M = Model();      AlbercaJa_M.LoadModel("Models/alberca.fbx");
    farola_M = Model();         farola_M.LoadModel("Models/farolota.fbx");
    LetreroHabitad_M = Model(); LetreroHabitad_M.LoadModel("Models/LetreroHabitad.fbx");

    // --- Skyboxes ---
    std::vector<std::string> skyboxFaces, skyboxFacesNight;
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

    // Luz direccional inicial (noche)
    mainLight = DirectionalLight(
        0.2f, 0.25f, 0.35f,
        0.3f, 0.2f,
        -0.5f, -1.0f, -0.3f
    );

    // Point lights
    unsigned int pointLightCount = 0;

    pointLights[pointLightCount] = PointLight(1.0f, 0.3f, 0.8f, 4.0f, 10.0f, 165.0f, 6.0f, 100.0f, 1.0, 0.014, 0.0007);
    int idxFarola1 = pointLightCount++;

    pointLights[pointLightCount] = PointLight(0.6f, 0.0f, 1.0f, 4.0f, 10.0f, -85.0f, 6.0f, 210.0f, 1.0, 0.014, 0.0007);
    int idxFarola2 = pointLightCount++;

    pointLights[pointLightCount] = PointLight(1.0f, 0.1f, 0.1f, 4.0f, 10.0f, -85.0f, 6.0f, 40.0f, 1.0, 0.014, 0.0007);
    int idxFarola3 = pointLightCount++;

    pointLights[pointLightCount] = PointLight(0.0f, 0.0f, 1.0f, 4.0f, 10.0f, 165.0f, 6.0f, -90.0f, 1.0, 0.014, 0.0007);
    int idxFarola4 = pointLightCount++;

    // Spotlights
    unsigned int spotLightCount = 0;

    spotLights[spotLightCount] = SpotLight(1.0f, 0.95f, 0.85f, 2.0f, 15.0f, 38.0f, 9.0f, -117.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.005f, 0.0002f, 25.0f);
    int idxLumbrera1 = spotLightCount++;

    spotLights[spotLightCount] = SpotLight(1.0f, 0.95f, 0.85f, 2.0f, 15.0f, 49.0f, 9.0f, -117.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.005f, 0.0002f, 25.0f);
    int idxLumbrera2 = spotLightCount++;

    GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
        uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
    GLuint uniformColor = 0;

    glm::mat4 projection = glm::perspective(45.0f,
        (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

    movCoche = 0.0f;
    movOffset = 0.01f;
    rotllanta = 0.0f;
    rotllantaOffset = 10.0f;

    // Controles de keyframes
    printf("\n=====================================\n");
    printf("      CONTROLES DE KEYFRAMES\n");
    printf("=====================================\n");
    printf(" [F1]         Utilizar el helicoptero\n");
    printf(" [F2]         Utilizar el aguila\n");
    printf(" [ESPACIO]    Reproducir animación del archivo activo\n");
    printf(" [1]          Activar/Desactivar modo edición\n");
    printf("    ├─ H / K  Mover en eje X  (H = +X / K = -X)\n");
    printf("    ├─ U / J  Mover en eje Y  (U = +Y / J = -Y)\n");
    printf("    └─ R      Activar/Desactivar modo rotación\n");
    printf(" [Flechas Izq/Der] Rotar (modo rotación)\n");
    printf(" [L]          Guardar frame actual en archivo\n");
    printf(" [M]          Modificar último frame guardado\n");
    printf(" [B]          Borrar último frame del archivo\n");
    printf("=====================================\n\n");

    glm::mat4 model(1.0);
    glm::mat4 modelaux(1.0);
    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec2 toffset = glm::vec2(0.0f, 0.0f);
    glm::vec3 lowerLight = glm::vec3(0.0f);

    float radio = 5.0f;
    float velocidadGiro = 0.1f;
    float centroX = -25.0f, centroZ = 260.0f;
    float centro2X = 30.0f, centro2Z = 270.0f;
    float posX, posZ, pos2x, pos2z;

    bool   isDay = true;
    double lastSwitchTime = 0.0;
    const  double switchInterval = 30.0; // segundos

    // ---- LOOP PRINCIPAL ----
    while (!mainWindow.getShouldClose())
    {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        deltaTime += (now - lastTime) / limitFPS;
        lastTime = now;

        angulovaria += 0.5f * deltaTime;
        alaAngulo = 15.0f * sin(angulovaria / 1.6f);

        if (movCoche < 30.0f) movCoche -= movOffset * deltaTime;
        rotllanta += rotllantaOffset * deltaTime;

        // Entrada
        glfwPollEvents();
        camera.keyControl(mainWindow.getsKeys(), deltaTime);
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

        // Keyframes (leer teclas y actualizar animaciones)
        inputKeyframes(mainWindow.getsKeys());
        framesAvion.animateStep(i_max_steps);
        framesAguila.animateStep(i_max_steps);

        // Clear & toggle día/noche
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (now - lastSwitchTime >= switchInterval) {
            isDay = !isDay;
            lastSwitchTime = now;

            if (isDay) {
                mainLight = DirectionalLight(1.0f, 0.75f, 0.45f, 1.0f, 0.8f, 0.0f, -0.8f, -0.3f);
                for (int i = 0; i < 4; i++) pointLights[i].TurnOff();
                spotLights[idxLumbrera1].TurnOff();
                spotLights[idxLumbrera2].TurnOff();
            }
            else {
                mainLight = DirectionalLight(0.2f, 0.25f, 0.35f, 0.3f, 0.2f, -0.5f, -1.0f, -0.3f);
                for (int i = 0; i < 4; i++) pointLights[i].TurnOn();
                spotLights[idxLumbrera1].TurnOn();
                spotLights[idxLumbrera2].TurnOn();
            }
        }

        if (isDay)  skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
        else        nightSkybox.DrawSkybox(camera.calculateViewMatrix(), projection);

        shaderList[0].UseShader();

        uniformModel = shaderList[0].GetModelLocation();
        uniformProjection = shaderList[0].GetProjectionLocation();
        uniformView = shaderList[0].GetViewLocation();
        uniformEyePosition = shaderList[0].GetEyePositionLocation();
        uniformColor = shaderList[0].getColorLocation();
        uniformTextureOffset = shaderList[0].getOffsetLocation();
        uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
        uniformShininess = shaderList[0].GetShininessLocation();

        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
        glUniform3f(uniformEyePosition,
            camera.getCameraPosition().x,
            camera.getCameraPosition().y,
            camera.getCameraPosition().z);

        shaderList[0].SetDirectionalLight(&mainLight);
        shaderList[0].SetPointLights(pointLights, 4);
        shaderList[0].SetSpotLights(spotLights, 2);

        // --- Piso ---
        glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(glm::vec2(0.0f)));
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(20.0f, -2.0f, 70.0f));
        model = glm::scale(model, glm::vec3(24.5f, 1.0f, 24.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniform3fv(uniformColor, 1, glm::value_ptr(glm::vec3(1.0f)));
        pisoTexture.UseTexture();
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[2]->RenderMesh();

        // --- FAROLA 1 (ROSA) ---
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(165.0f, -2.0f, 100.0f));
        model = glm::scale(model, glm::vec3(2.0f));
        model = glm::rotate(model, -90 * toRadians, glm::vec3(1, 0, 0));
        model = glm::rotate(model, 180 * toRadians, glm::vec3(0, 0, 1));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        PuertaTexture.UseTexture();
        farola_M.RenderModel();
        {
            glm::vec4 posLuz = model * glm::vec4(0.0f, 5.0f, 0.0f, 1.0f);
            pointLights[idxFarola1].SetPosition(glm::vec3(posLuz));
        }

        // --- FAROLA 2 (AMARILLA) ---
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(-85.0f, -2.0f, 210.0f));
        model = glm::scale(model, glm::vec3(2.0f));
        model = glm::rotate(model, -90 * toRadians, glm::vec3(1, 0, 0));
        model = glm::rotate(model, -305 * toRadians, glm::vec3(0, 0, 1));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        PuertaTexture.UseTexture();
        farola_M.RenderModel();
        {
            glm::vec4 posLuz = model * glm::vec4(0.0f, 5.0f, 0.0f, 1.0f);
            pointLights[idxFarola2].SetPosition(glm::vec3(posLuz));
        }

        // --- FAROLA 3 (ROJA) ---
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(-85.0f, -2.0f, 40.0f));
        model = glm::scale(model, glm::vec3(2.0f));
        model = glm::rotate(model, -90 * toRadians, glm::vec3(1, 0, 0));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        PuertaTexture.UseTexture();
        farola_M.RenderModel();
        {
            glm::vec4 posLuz = model * glm::vec4(0.0f, 5.0f, 0.0f, 1.0f);
            pointLights[idxFarola3].SetPosition(glm::vec3(posLuz));
        }

        // --- FAROLA 4 (AZUL) ---
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(165.0f, -2.0f, -90.0f));
        model = glm::scale(model, glm::vec3(2.0f));
        model = glm::rotate(model, -90 * toRadians, glm::vec3(1, 0, 0));
        model = glm::rotate(model, -180 * toRadians, glm::vec3(0, 0, 1));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        PuertaTexture.UseTexture();
        farola_M.RenderModel();
        {
            glm::vec4 posLuz = model * glm::vec4(0.0f, 5.0f, 0.0f, 1.0f);
            pointLights[idxFarola4].SetPosition(glm::vec3(posLuz));
        }

        // -------- Letreros (ya corregidos con Texture y modelo) --------
        // Águila
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-80.0f, 5.0f, 118.0f));
        model = glm::scale(model, glm::vec3(0.5f));
        model = glm::rotate(model, -90.0f * toRadians, glm::vec3(1, 0, 0));
        model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0, 0, 1));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        aguila_Letrero.UseTexture();
        LetreroHabitad_M.RenderModel();

        // Ajolote
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-50.0f, 5.0f, 210.0f));
        model = glm::scale(model, glm::vec3(0.5f));
        model = glm::rotate(model, -90.0f * toRadians, glm::vec3(1, 0, 0));
        model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0, 0, 1));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        ajolotes_Letrero.UseTexture();
        LetreroHabitad_M.RenderModel();

        // Aviario
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-80.0f, 5.0f, -80.0f));
        model = glm::scale(model, glm::vec3(0.5f));
        model = glm::rotate(model, -90.0f * toRadians, glm::vec3(1, 0, 0));
        model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0, 0, 1));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Aviario_Letrero.UseTexture();
        LetreroHabitad_M.RenderModel();

        // Jaguar
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(135.0f, 5.0f, -80.0f));
        model = glm::scale(model, glm::vec3(0.5f));
        model = glm::rotate(model, -90.0f * toRadians, glm::vec3(1, 0, 0));
        model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0, 0, 1));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Jaguar_Letrero.UseTexture();
        LetreroHabitad_M.RenderModel();

        // Pirámide
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(88.0f, 5.0f, 195.0f));
        model = glm::scale(model, glm::vec3(0.5f));
        model = glm::rotate(model, -90.0f * toRadians, glm::vec3(1, 0, 0));
        model = glm::rotate(model, -135.0f * toRadians, glm::vec3(0, 0, 1));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Piramide_Letrero.UseTexture();
        LetreroHabitad_M.RenderModel();

        // Ring de lucha
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-4.0f, 5.0f, -6.0f));
        model = glm::scale(model, glm::vec3(0.5f));
        model = glm::rotate(model, -90.0f * toRadians, glm::vec3(1, 0, 0));
        model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0, 0, 1));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        RingDeLucha_Letrero.UseTexture();
        LetreroHabitad_M.RenderModel();

        // Entrada / Arco
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(20.0f, -2.0f, -110.0f));
        model = glm::scale(model, glm::vec3(2.0f, 4.0f, 2.0f));
        model = glm::rotate(model, -90 * toRadians, glm::vec3(1, 0, 0));
        modelaux = model;
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        ArcoTexture.UseTexture();
        Arco_M.RenderModel();

        // Puertas
        model = modelaux;
        model = glm::translate(model, glm::vec3(-2.1f, 0.0f, 3.4f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        PuertaTexture.UseTexture();
        Puerta_Derecha_M.RenderModel();

        model = modelaux;
        model = glm::translate(model, glm::vec3(3.1f, 0.0f, 3.4f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        PuertaTexture.UseTexture();
        Puerta_Izquierda_M.RenderModel();

        // Letrero (estructura)
        model = modelaux;
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Letrero_M.RenderModel();

        // Lumbreras
        {
            model = glm::mat4(1.0);
            model = glm::translate(model, glm::vec3(38.0, -2.0f, -117.0f));
            model = glm::scale(model, glm::vec3(3.0f));
            model = glm::rotate(model, -90 * toRadians, glm::vec3(1, 0, 0));
            model = glm::rotate(model, 180 * toRadians, glm::vec3(0, 0, 1));
            modelaux = model;
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            lumbreraTexture.UseTexture();
            lumbrera.RenderModel();
            glm::vec4 posLuz = model * glm::vec4(0.0f, 5.0f, 0.0f, 1.0f);
            spotLights[idxLumbrera1].SetPos(glm::vec3(posLuz));
        }
        {
            model = modelaux;
            model = glm::translate(model, glm::vec3(11.0f, 0.0f, 0.0f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            lumbreraTexture.UseTexture();
            lumbrera.RenderModel();
            glm::vec4 posLuz = model * glm::vec4(0.0f, 5.0f, 0.0f, 1.0f);
            spotLights[idxLumbrera2].SetPos(glm::vec3(posLuz));
        }

        // Águila (usa framesAguila)
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(-120.0f, 51.0f, 115.0f));
        model = glm::scale(model, glm::vec3(4.0f));
        model = glm::rotate(model, -90 * toRadians, glm::vec3(1, 0, 0));
        model = glm::translate(model, glm::vec3(framesAguila.getMovX(), framesAguila.getMovY(), 0.0f));
        model = glm::rotate(model, glm::radians(framesAguila.getGiro()), glm::vec3(0, 0, 1));
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

        // Nido / Jaula
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(-120.0f, -2.0f, 118.0f));
        model = glm::scale(model, glm::vec3(3.0f, 7.0f, 3.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        nidoTexture.UseTexture();
        nido_M.RenderModel();

        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(-140.0f, -2.0f, 118.0f));
        model = glm::scale(model, glm::vec3(29.0f, 20.0f, 45.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        PuertaTexture.UseTexture();
        jaula_M.RenderModel();

        // Hábitat ajolote
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(5.0f, -2.0f, 250.0f));
        model = glm::scale(model, glm::vec3(6.6f));
        model = glm::rotate(model, 180 * toRadians, glm::vec3(0, 1, 0));
        modelaux = model;
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        habitat_ajoloteTexture.UseTexture();
        habitat_ajolote_M.RenderModel();

        model = modelaux;
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 5.0f));
        model = glm::scale(model, glm::vec3(3.0f, 1.0f, 1.0f));
        model = glm::rotate(model, -90 * toRadians, glm::vec3(1, 0, 0));
        model = glm::rotate(model, 90 * toRadians, glm::vec3(0, 0, 1));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        nidoTexture.UseTexture();
        reja_M.RenderModel();

        model = modelaux;
        model = glm::translate(model, glm::vec3(4.2f, -0.8f, -0.6f));
        model = glm::rotate(model, 180 * toRadians, glm::vec3(0, 1, 0));
        model = glm::scale(model, glm::vec3(1.034f, 1.0f, 1.3f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        aguaTexture.UseTexture();
        agua_M.RenderModel();

        // Ajolotes en trayectoria circular
        posX = centroX + radio * cos(angulovaria * velocidadGiro);
        posZ = centroZ + radio * sin(angulovaria * velocidadGiro);
        pos2x = centro2X + 3 * cos(angulovaria * velocidadGiro + 3.14f);
        pos2z = centro2Z + 3 * sin(angulovaria * velocidadGiro + 3.14f);

        // Ajolote 1
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(posX, 1.5f, posZ));
        model = glm::scale(model, glm::vec3(1.5f));
        model = glm::rotate(model, 90 * toRadians, glm::vec3(0, 1, 0));
        modelaux = model;
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        axoloteTexture.UseTexture();
        axolote_M.RenderModel();

        model = modelaux; axoloteCola_M.RenderModel();

        model = modelaux;
        model = glm::translate(model, glm::vec3(0.8f, 0.0f, 0.5f));
        model = glm::rotate(model, 30.0f * toRadians, glm::vec3(0, 0, 1));
        model = glm::rotate(model, 0.6f * sin(angulovaria / 10), glm::vec3(0, 0, 1));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        axolotePataDerecha_M.RenderModel();

        model = modelaux;
        model = glm::translate(model, glm::vec3(0.8f, 0.0f, -0.5f));
        model = glm::rotate(model, 30.0f * toRadians, glm::vec3(0, 0, 1));
        model = glm::rotate(model, 0.6f * sin(angulovaria / 10), glm::vec3(0, 0, 1));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        axolotePataIzquierda_M.RenderModel();

        model = modelaux;
        model = glm::translate(model, glm::vec3(-0.5f, -0.2f, 0.4f));
        model = glm::rotate(model, -40.0f * toRadians, glm::vec3(0, 0, 1));
        model = glm::rotate(model, 0.5f * sin(angulovaria / 10), glm::vec3(0, 0, 1));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        axolotePataTraseraDerecha_M.RenderModel();

        model = modelaux;
        model = glm::translate(model, glm::vec3(-0.5f, -0.2f, -0.4f));
        model = glm::rotate(model, -40.0f * toRadians, glm::vec3(0, 0, 1));
        model = glm::rotate(model, 0.5f * sin(angulovaria / 10), glm::vec3(0, 0, 1));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        axolotePataTraseraIzquierda_M.RenderModel();

        // Ajolote 2 (idéntico con pos2x/pos2z)
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(pos2x, 1.5f, pos2z));
        model = glm::scale(model, glm::vec3(1.5f));
        model = glm::rotate(model, 90 * toRadians, glm::vec3(0, 1, 0));
        modelaux = model;
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        axoloteTexture.UseTexture();
        axolote_M.RenderModel();

        model = modelaux; axoloteCola_M.RenderModel();

        model = modelaux;
        model = glm::translate(model, glm::vec3(0.8f, 0.0f, 0.5f));
        model = glm::rotate(model, 30.0f * toRadians, glm::vec3(0, 0, 1));
        model = glm::rotate(model, 0.6f * sin(angulovaria / 10), glm::vec3(0, 0, 1));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        axolotePataDerecha_M.RenderModel();

        model = modelaux;
        model = glm::translate(model, glm::vec3(0.8f, 0.0f, -0.5f));
        model = glm::rotate(model, 30.0f * toRadians, glm::vec3(0, 0, 1));
        model = glm::rotate(model, 0.6f * sin(angulovaria / 10), glm::vec3(0, 0, 1));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        axolotePataIzquierda_M.RenderModel();

        model = modelaux;
        model = glm::translate(model, glm::vec3(-0.5f, -0.2f, 0.4f));
        model = glm::rotate(model, -40.0f * toRadians, glm::vec3(0, 0, 1));
        model = glm::rotate(model, 0.5f * sin(angulovaria / 10), glm::vec3(0, 0, 1));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        axolotePataTraseraDerecha_M.RenderModel();

        model = modelaux;
        model = glm::translate(model, glm::vec3(-0.5f, -0.2f, -0.4f));
        model = glm::rotate(model, -40.0f * toRadians, glm::vec3(0, 0, 1));
        model = glm::rotate(model, 0.5f * sin(angulovaria / 10), glm::vec3(0, 0, 1));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        axolotePataTraseraIzquierda_M.RenderModel();

        // Hábitat jaguar
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(165.0f, 2.0f, -12.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        troncoTexture.UseTexture();
        tronco_M.RenderModel();

        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(200.0f, 30.3f, -7.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        jaguarTexture.UseTexture();
        jaguar_M.RenderModel();

        model = glm::translate(model, glm::vec3(-20.0f, -13.0f, 20.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        jaguarTexture.UseTexture();
        jaguar_M.RenderModel();

        model = glm::translate(model, glm::vec3(-20.0f, -0.7f, -55.0f));
        model = glm::rotate(model, -90 * toRadians, glm::vec3(0, 1, 0));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        jaguarTexture.UseTexture();
        jaguar_M.RenderModel();

        model = glm::translate(model, glm::vec3(100.0f, -20.0f, 0.0f));
        model = glm::rotate(model, -90 * toRadians, glm::vec3(0, 1, 0));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        jaguarTexture.UseTexture();
        jaguar_M.RenderModel();

        // Vegetación y demás
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(177.0f, -2.0f, 26.0f));
        model = glm::scale(model, glm::vec3(5.0f));
        model = glm::rotate(model, -90 * toRadians, glm::vec3(1, 0, 0));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        arbolTexture.UseTexture();
        arbol_M.RenderModel();

        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(180.0f, -2.0f, -63.0f));
        model = glm::scale(model, glm::vec3(5.0f));
        model = glm::rotate(model, -90 * toRadians, glm::vec3(1, 0, 0));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        arbolTexture.UseTexture();
        arbol_M.RenderModel();

        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(190.0f, 10.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.7f));
        model = glm::rotate(model, -90 * toRadians, glm::vec3(0, 1, 0));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        piedrasTexture.UseTexture();
        piedras_M.RenderModel();

        // Pirámide
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(160.0f, -2.0f, 215.0f));
        model = glm::scale(model, glm::vec3(55.0f));
        model = glm::rotate(model, -90 * toRadians, glm::vec3(1, 0, 0));
        model = glm::rotate(model, 45 * toRadians, glm::vec3(0, 0, 1));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        piramideTexture.UseTexture();
        piramide_M.RenderModel();

        // Arbustos
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(175.0f, -2.0f, 140.0f));
        model = glm::scale(model, glm::vec3(15.0f));
        model = glm::rotate(model, -90 * toRadians, glm::vec3(1, 0, 0));
        modelaux = model;
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        arbusto1Texture.UseTexture();
        arbusto1_M.RenderModel();

        model = modelaux; model = glm::translate(model, glm::vec3(-3.0f, -1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); arbusto1Texture.UseTexture(); arbusto1_M.RenderModel();
        model = modelaux; model = glm::translate(model, glm::vec3(-5.0f, -3.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); arbusto1Texture.UseTexture(); arbusto1_M.RenderModel();

        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(90.0f, -2.0f, 270.0f));
        model = glm::scale(model, glm::vec3(50.0f));
        model = glm::rotate(model, -90 * toRadians, glm::vec3(1, 0, 0));
        modelaux = model;
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        arbusto2Texture.UseTexture();
        arbusto2_M.RenderModel();

        model = modelaux; model = glm::translate(model, glm::vec3(-0.2f, 0.3f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); arbusto2Texture.UseTexture(); arbusto2_M.RenderModel();
        model = modelaux; model = glm::translate(model, glm::vec3(-0.4f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model)); arbusto2Texture.UseTexture(); arbusto2_M.RenderModel();

        // Ring
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(20.0f, 0.0f, 40.0f));
        model = glm::scale(model, glm::vec3(8.0f));
        model = glm::rotate(model, 180 * toRadians, glm::vec3(0, 1, 0));
        model = glm::rotate(model, -90 * toRadians, glm::vec3(1, 0, 0));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        ringTexture.UseTexture();
        ring_M.RenderModel();

        // Aviario
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(-120.0f, -2.0f, 100.0f));
        model = glm::scale(model, glm::vec3(7.0f));
        model = glm::rotate(model, -90 * toRadians, glm::vec3(1, 0, 0));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        arbolAviarioTexture.UseTexture();
        arbolAviario_M.RenderModel();

        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(-125.0f, 20.0f, 105.0f));
        model = glm::rotate(model, -90 * toRadians, glm::vec3(1, 0, 0));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        pajaroTexture.UseTexture();
        pajaro_M.RenderModel();

        // Arbustos adicionales
        model = glm::mat4(1.0);
        model = glm::scale(model, glm::vec3(2.0f));
        model = glm::rotate(model, -90 * toRadians, glm::vec3(1, 0, 0));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        arbusto1Texture.UseTexture();
        arbusto1_M.RenderModel();

        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(10.0f));
        model = glm::rotate(model, -90 * toRadians, glm::vec3(1, 0, 0));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        arbusto2Texture.UseTexture();
        arbusto2_M.RenderModel();

        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(5.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f));
        model = glm::rotate(model, -90 * toRadians, glm::vec3(1, 0, 0));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        arbusto3Texture.UseTexture();
        arbusto3_M.RenderModel();

        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(20.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f));
        model = glm::rotate(model, -90 * toRadians, glm::vec3(1, 0, 0));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        arbol2Texture.UseTexture();
        arbol2_M.RenderModel();

        // Cuadro
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(160.0f, 5.0f, 3.0f));
        model = glm::scale(model, glm::vec3(13.0f, 16.0f, 19.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        cuadroTexture.UseTexture();
        cuadro_M.RenderModel();

        // Alberca
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(160.0f, -2.0f, 60.0f));
        model = glm::scale(model, glm::vec3(5.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        aguaTexture.UseTexture();
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        AlbercaJa_M.RenderModel();

        glUseProgram(0);
        mainWindow.swapBuffers();
    }

    return 0;
}
