

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <time.h>

#include "shader.h"

#include "pointLight.h"
#include "sphere2.h"
#include "cube.h"
#include "dLight.h"
#include "SpotLight.h"
#include "tree.h"
#include "player.h"
#include "curveObject.h"
#include "stb_image.h"


#include <iostream>

//using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
//void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model, float r, float g, float b);
void drawLine(Shader& ourShader, glm::mat4 model, float lineFromCamera[]);
bool checkFront();
float jump(int t);
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);
void addTree(glm::vec2 pos, int height);

//void bed(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether);


// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;




//Shooting..!
bool trigger = false;

//Background Color
float t = 0.0f;
float timeForColor;
bool waitForDayNight = false;
bool setTime = false;

//Cube Calculation..!(1000*1000*1000 grid)
vector <vector < glm::vec3 >> SpawnPos(4, vector<glm::vec3>()); 
vector <glm::vec3> SpawnDir;

vector <vector<vector<bool>>> isCube(1000, vector<vector<bool> >(1000, vector<bool>(1000, false)));
vector <vector<vector<int>>> isCubeType(1000, vector<vector<int> >(1000, vector<int>(1000, -1)));

//player
glm ::vec3 playerFacePos;
glm::vec3 playerFaceDir;
float Yaw, Roll, Pitch;
bool isPlayer = true;
int boxLeft[3] = { 5,5,5 };
int currentBoxType = 0;
bool onAir = false;
int JumpCalTime = 1;
int jumpCount = 1;


// camera
glm::vec3 camPos = glm::vec3(0.0f, 4.5f, 5.0f);
//Camera camera(camPos);


Player player(camPos);
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;




// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(2.00f,  2.50f,  7.0f),
    glm::vec3(-2.0f,  2.5f,  7.0f),
    glm::vec3(-2.0f,  2.5f,  3.0f),
    glm::vec3(2.0f,   2.5f,  3.0f)
};



glm::vec3 lightPositions[] = {
        glm::vec3(2.25f, 1.6f, -1.7f),
        glm::vec3(0.0, 2.0f, 0.0f)
};

glm::vec3 lightDirections[] = {
    glm::vec3(0.0f, -1.0f, 0.0f)
};


DirectionalLight dLight = DirectionalLight(-0.2f, -1.0f, -0.3f);

SpotLight spotLight(lightPositions[1], lightDirections[0], 8.5f, 15.5f, glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, 1);

DirectionalLight dLight2 = DirectionalLight(-0.2f, -1.0f, -0.3f);

SpotLight spotLight2(lightPositions[1], lightDirections[0], 8.5f, 15.5f, glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, 1);



PointLight pointlight1(

    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    0.5f, 0.5f, 0.5f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);
PointLight pointlight2(

    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2       // light number
);
PointLight pointlight3(

    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    3       // light number
);
PointLight pointlight4(

    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    4       // light number
);

PointLight pointlight5(

    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    0.5f, 0.5f, 0.5f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);
PointLight pointlight6(

    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2       // light number
);
PointLight pointlight7(

    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    3       // light number
);
PointLight pointlight8(

    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    4       // light number
);


// light settings
bool pointLightOn = false;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;
bool dLightbool = true;
bool SpotlightBool = false;


// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

int main()
{   
    
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    //Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShadingTest.fs");
    Shader lightingShader2("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShadingTest.fs");
    //Shader lightingShader("vertexShaderForGouraudShading.vs", "fragmentShaderForGouraudShading.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");
    Shader lihgtWithTexture( "vertexShaderForPhongShadingWithTexture.vs","fragmentShaderForPhongShadingWithTexture.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    float cube_vertices[] = {
        // positions      // normals
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,

        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };

    unsigned int cubeVAO, cubeVBO, cubeEBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);




    //texture
    string diffuseMapPath = "container2.png";
    string specularMapPath = "Black.png.";

    string acacia_log = "acacia_log.png";
    string acacia_log_top = "acacia_log_top";
    string Merged_acacia = "Merged_acacia.png";
    string dirtMarge = "dirtMarge.png";
    string MergedGeass = "MergedGeass.png";
    string MergedLeaf = "MergedLeaf.png";
    string sun4 = "sun4.jpg";

    unsigned int diffMap = loadTexture(Merged_acacia.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int diffMap1 = loadTexture(dirtMarge.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int diffMap2 = loadTexture(MergedGeass.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int diffMap3 = loadTexture(MergedLeaf.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int diffMap4 = loadTexture(sun4.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    unsigned int specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    float lineFromCamera[6];


    //lien VBO,VAO

    unsigned int lineVBO, lineVAO;
    glGenVertexArrays(1, &lineVAO);
    glGenBuffers(1, &lineVBO);

    glBindVertexArray(lineVAO);

    // Bind VBO and copy vertex data
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lineFromCamera), lineFromCamera, GL_STATIC_DRAW);

    // Specify the vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);







    //Sphere sphere = Sphere();
   Sphere sun = Sphere(1.0f, 36, 18, glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f, diffMap4, specMap, 0.0f, 0.0f, 1.0f, 1.0f);

    Cube cube2 = Cube(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    cube2.setTextureProperty(diffMap, specMap);
    
    
    for (int i = 0; i < 70; ++i) {	
        // Random x and y coordinates within a certain range, adjust as needed
        float randomX = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 60.0 - 30.0; // Range: -30.0 to 30.0
        float randomY = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 60.0 - 30.0; // Range: -30.0 to 30.0

        // Assuming treeParameter is also random, adjust as needed
        int randomParameter = rand() % 4 + 5; // Random parameter between 0 and 9
        
        float x = randomX - floor(randomX);
        float y = randomY - floor(randomY);
        
        //cout<< randomX-x<<" "<< randomY-y <<endl;
        
        
        // Call the addTree function with the random coordinates and parameter
        addTree(glm::vec2(randomX-x, randomY-y), randomParameter);
    }


    Cube wood(glm::vec3(0.341, 0.075, 0.075), glm::vec3(0.341, 0.341, 0.341));
    wood.setTextureProperty(diffMap, specMap);
    Cube grass(glm::vec3(0.043, 0.91, 0.141), glm::vec3(0.043, 0.91, 0.141));
    grass.setTextureProperty(diffMap2, specMap);
    Cube soil(glm::vec3(0.902, 0.855, 0.463), glm::vec3(0.902, 0.855, 0.463));
    soil.setTextureProperty(diffMap1, specMap);
    Cube leaf(glm::vec3(0.902, 0.855, 0.463), glm::vec3(0.902, 0.855, 0.463));
    leaf.setTextureProperty(diffMap3, specMap);


    //Land
    float gridSize = 30;
    for (float i = -gridSize; i < gridSize; i = i + 1.0f)
    {
        for (float j = -gridSize; j < gridSize; j = j + 1.0f)
        {   

            isCube[(int)i + 500][0 + 500][(int)j + 500] = true;
            isCube[(int)i + 500][-1 + 500][(int)j + 500] = true;
            isCubeType[(int)i + 500][0 + 500][(int)j + 500] = 1;
            isCubeType[(int)i + 500][-1 + 500][(int)j + 500] = 2;

            SpawnPos[1].push_back(glm::vec3(i, 0.0, j));
            SpawnPos[2].push_back(glm::vec3(i, -1.0, j));

            
        }
    }


    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //ourShader.use();
    //lightingShader.use();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
       
        glm::vec4 dawnColor(0.6f, 0.6f, 0.7f, 1.0f);
        glm::vec4 duskColor(0.01f, 0.01f, 0.02f, 1.0f); // Darker blue
        glm::vec4 currentColor;
        
        //calculation for directional light color and background color...!
        if(isPlayer)
        {
            
            float x = sin(glm::radians(t));


            if (t > 89 && t < 90) {
                waitForDayNight = true;
                if (!setTime) {
                    timeForColor = currentFrame;
                    setTime = true;
                }

                if (currentFrame - timeForColor > 15.0) {
                    t++;
                    waitForDayNight = false;
                    setTime = false;
                }

            }

            if (t < 89 && !waitForDayNight)
                t += .05;
            if (t > 90 && !waitForDayNight)
                t += .05;


            if (t > 179 && t < 180) {
                waitForDayNight = true;
                if (!setTime) {
                    timeForColor = currentFrame;
                    setTime = true;
                }

                if (currentFrame - timeForColor > 15.0) {
                    t++;
                    if (t >= 180) {
                        t = 0.0;
                    }
                    waitForDayNight = false;
                    setTime = false;
                }

            }

            currentColor = dawnColor * (1.0f - abs(x)) + duskColor * abs(x);
            glClearColor(currentColor.r, currentColor.g, currentColor.b, currentColor.a);

            ////////
        }
        else
        {   
            float x = 0;
            currentColor = dawnColor * (1.0f - abs(x)) + duskColor * abs(x);
            glClearColor(currentColor.r, currentColor.g, currentColor.b, currentColor.a);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader2.setVec3("viewPos", player.camera.Position);
        lightingShader.setVec3("viewPos", player.camera.Position);
        lihgtWithTexture.setVec3("viewPos", player.camera.Position);



        // point light 1
        pointlight1.setUpPointLight(lihgtWithTexture);
        // point light 2
        pointlight2.setUpPointLight(lihgtWithTexture);
        // point light 3
        pointlight3.setUpPointLight(lihgtWithTexture);
        // point light 4
        pointlight4.setUpPointLight(lihgtWithTexture);


        dLight.ambient = currentColor;
        dLight.diffuse = currentColor;
        dLight.setUpDirectionalLight(lihgtWithTexture);


        spotLight.setSpotLight(player.camera.Position, player.camera.Front, 8.5f, 15.5f, glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, 1);

        spotLight.setUpLight2(lihgtWithTexture);


        // activate shader
        lightingShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(player.camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = player.camera.GetViewMatrix();
        //glm::mat4 view = basic_camera.createViewMatrix();
        lightingShader.setMat4("view", view);


        lihgtWithTexture.use();
        lihgtWithTexture.setMat4("projection", projection);
        lihgtWithTexture.setMat4("view", view);
        

        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;

        lightingShader.setMat4("model", model);
        lihgtWithTexture.setMat4("model", model);
        
        
        glm::mat4 modelForSphere = glm::mat4(1.0f);
        modelForSphere = glm::translate(model, glm::vec3(5.0f, 12.0f, -5.0f));
        modelForSphere = glm::scale(modelForSphere, glm::vec3(1.5f, 1.5f, 1.5f));
        sun.drawSphereWithTexture(lihgtWithTexture, modelForSphere);


        glm::mat4 modelForCube = glm::mat4(1.0f);
        lihgtWithTexture.use();
       

        lihgtWithTexture.use();
        int i = 0;
        for (auto& a : SpawnPos[0]) {

           
            modelForCube = glm::translate(model, glm::vec3(a.x - .5, a.y - .5, a.z - .5));
           // if (trigger)
            //   a += SpawnDir[i] * glm::vec3 (1);
            wood.drawCubeWithTexture(lihgtWithTexture, modelForCube);
            i++;
              
        }

        for (auto& a : SpawnPos[1]) {


            modelForCube = glm::translate(model, glm::vec3(a.x - .5, a.y - .5, a.z - .5));
            grass.drawCubeWithTexture(lihgtWithTexture, modelForCube);
            

        }

        for (auto& a : SpawnPos[2]) {


            modelForCube = glm::translate(model, glm::vec3(a.x - .5, a.y - .5, a.z - .5));
            soil.drawCubeWithTexture(lihgtWithTexture, modelForCube);


        }

        for (auto& a : SpawnPos[3]) {


            modelForCube = glm::translate(model, glm::vec3(a.x - .5, a.y - .5, a.z - .5));
            leaf.drawCubeWithTexture(lihgtWithTexture, modelForCube);


        }

 
        
        //line draw..!
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        ourShader.setMat4("model", model);
        ourShader.setVec3("color", glm::vec3(1.0f, 1.0f, 0.0f));

        lineFromCamera[0] = player.camera.ProcessMouseClick().x;
        lineFromCamera[1] = player.camera.ProcessMouseClick().y -.02 ;
        lineFromCamera[2] = player.camera.ProcessMouseClick().z;
        lineFromCamera[3] = player.camera.ProcessMouseClick().x;
        lineFromCamera[4] = player.camera.ProcessMouseClick().y + .02;
        lineFromCamera[5] = player.camera.ProcessMouseClick().z;

        glBindVertexArray(lineVAO);
        glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(lineFromCamera), lineFromCamera, GL_STATIC_DRAW);
        glLineWidth(8);
        glDrawArrays(GL_LINES, 0, 2);
        glBindVertexArray(0);

        
      

        


        // we now draw as many light bulbs as we have point lights.
        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < 2; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            ourShader.setMat4("model", model);
            ourShader.setVec3("color", glm::vec3(0.8f, 0.8f, 0.8f));
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            //glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        
        //Floor detection..!

        player.foot = player.camera.Position.y - 1.5;
        if (!isCube[(int)floor(player.camera.Position.x + .5) + 500]
            [(int)ceil(player.foot - .5) + 500]
            [(int)floor(player.camera.Position.z + .5) + 500] && isPlayer) {
            player.camera.Position.y -=.1f;
            jumpCount = 0;
            //cout << "no floor" << endl;;
        }
        else
        {
            jumpCount = 1;
        }

        
        //Jump
        
        if (onAir) {

            player.camera.Position.y += jump(JumpCalTime);
            JumpCalTime++;
        }



        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}




// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{      
    bool isBlock = checkFront();
    
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
       
        player = Player(camPos);
    }

    if(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        {
            isPlayer = true;
            player.camera.Position = playerFacePos;
            player.camera.Front = playerFaceDir;
            player.camera.Yaw = Yaw;
            player.camera.Roll = Roll;
            player.camera.Pitch = Pitch;

        }
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
        isPlayer = false;
        playerFacePos = player.camera.Position;
        playerFaceDir = player.camera.Front;
        Yaw = player.camera.Yaw;
        Roll = player.camera.Roll;
        Pitch = player.camera.Pitch;
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
        //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    if (!isPlayer) 
     {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ) {
            player.camera.ProcessKeyboard(FORWARD, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            player.camera.ProcessKeyboard(BACKWARD, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            player.camera.ProcessKeyboard(LEFT, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            player.camera.ProcessKeyboard(RIGHT, deltaTime);
        }
        if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            player.camera.ProcessKeyboard(P_UP, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            player.camera.ProcessKeyboard(P_DOWN, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            player.camera.ProcessKeyboard(Y_LEFT, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            player.camera.ProcessKeyboard(Y_RIGHT, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            player.camera.ProcessKeyboard(R_RIGHT, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            player.camera.ProcessKeyboard(R_LEFT, deltaTime);
        }
    }
    
        
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && !isBlock) {
            player.camera.MovementSpeed = 5.0f;
            player.camera.ProcessKeyboardPlayer(FORWARD, deltaTime);
            player.camera.MovementSpeed = 2.5f;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            player.camera.MovementSpeed = 5.0f;
            player.camera.ProcessKeyboardPlayer(BACKWARD, deltaTime);
            player.camera.MovementSpeed = 2.5f;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            player.camera.MovementSpeed = 5.0f;
            player.camera.ProcessKeyboardPlayer(LEFT, deltaTime);
            player.camera.MovementSpeed = 2.5f;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            player.camera.MovementSpeed = 5.0f;
            player.camera.ProcessKeyboardPlayer(RIGHT, deltaTime);
            player.camera.MovementSpeed = 2.5f;
        }
    
     {
         if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && !isBlock) {
             player.camera.ProcessKeyboardPlayer(FORWARD, deltaTime);
         }
         if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
             player.camera.ProcessKeyboardPlayer(BACKWARD, deltaTime);
         }
         if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
             player.camera.ProcessKeyboardPlayer(LEFT, deltaTime);
         }
         if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
             player.camera.ProcessKeyboardPlayer(RIGHT, deltaTime);
         }
     }
    

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && jumpCount == 1) {
        onAir = true;
        jumpCount--;
    }

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) currentBoxType = 0;
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) currentBoxType = 1;
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) currentBoxType = 2;

   /* if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        
    }*/

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{   
    
    if (key == GLFW_KEY_0 && action == GLFW_PRESS)
    {
        if (pointLightOn)
        {
            pointlight1.turnOff();
            pointlight2.turnOff();
            pointlight3.turnOff();
            pointlight4.turnOff();
            pointLightOn = !pointLightOn;
        }
        else
        {
            pointlight1.turnOn();
            pointlight2.turnOn();
            pointlight3.turnOn();
            pointlight4.turnOn();
            pointLightOn = !pointLightOn;
        }
    }

    if (!isPlayer)
    if (key == GLFW_KEY_9 && action == GLFW_PRESS)
    {
        if (dLightbool)
        {
            dLight.turnOff();
            dLightbool =! dLightbool;
        }
        else
        {
            dLight.turnOn();
            dLightbool = !dLightbool;
        }
    }

    if (isPlayer)
    if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        if (SpotlightBool)
        {
            spotLight.turnOff();
            SpotlightBool = !SpotlightBool;
        }
        else
        {
            spotLight.turnOn();
            SpotlightBool = !SpotlightBool;
        }
    }
   
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    player.camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{   
    if(!isPlayer)
        player.camera.ProcessMouseScroll(static_cast<float>(yoffset));
    if (isPlayer) {
        float x = (float)(((int)player.camera.distance + static_cast<int>(yoffset)) % 4);
        player.camera.distance = max((float)2.0, x);
    }

}

void drawLine(Shader& ourShader,glm :: mat4 model,float lineFromCamera[]) {}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
   
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        //std::cout << "Clicked" << endl;

        int x = (int)floor(player.camera.ProcessMouseClick().x+.5), y = (int)floor(player.camera.ProcessMouseClick().y + .5), z = (int)floor(player.camera.ProcessMouseClick().z + .5);
        //int x1 = (int)floor(player.camera.ProcessMouseClick1().x + .5), y1 = (int)floor(player.camera.ProcessMouseClick1().y + .5), z1 = (int)floor(player.camera.ProcessMouseClick1().z + .5);

        //cout << "In" << x << " " << y << " " << z << ":";
        //cout << x1<< " " << y1 << " " << z1 << endl;

        if(!isCube[x+500][y + 500][z + 500] && boxLeft[currentBoxType]>0) {

            isCube[x + 500][y + 500][z + 500] = true;
            isCubeType[x + 500][y + 500][z + 500] = currentBoxType;

            SpawnPos[currentBoxType].push_back(glm::vec3((float)x, (float)y, (float)z));

           // cout <<"From Draw" << SpawnPos[0].size() << " " << SpawnPos[1].size() << " " << SpawnPos[2].size() << endl;
            boxLeft[currentBoxType]--;
        }
        
        //For  Shooting
        //SpawnPos.push_back(player.camera.ProcessMouseClick());
        //SpawnDir.push_back(player.camera.Front);
        
    }
    
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {       
        //trigger = false;
        //std::cout << "Relaease" << endl;
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        
    }

    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    {
        int x = (int)floor(player.camera.ProcessMouseClick().x + .5), y = (int)floor(player.camera.ProcessMouseClick().y + .5), z = (int)floor(player.camera.ProcessMouseClick().z + .5);
        
        //cout << "Dlt" << x << " " << y << " " << z << endl;

        cout << isCube[x + 500][y + 500][z + 500] << " " << isCubeType[x + 500][y + 500][z + 500]<<endl;

        if (isCube[x + 500][y + 500][z + 500] && isCubeType[x + 500][y + 500][z + 500] != -1) {

            

            isCube[x + 500][y + 500][z + 500] = false;

            int i = isCubeType[x + 500][y + 500][z + 500];
            isCubeType[x + 500][y + 500][z + 500] = -1;

            SpawnPos[i].erase(std::remove(SpawnPos[i].begin(), SpawnPos[i].end(), glm::vec3((float)x, (float)y, (float)z)), SpawnPos[i].end());
            //remove(SpawnPos[i].begin(), SpawnPos[i].end(), glm::vec3((float)x, (float)y, (float)z));

            //cout <<"From dlt" << SpawnPos[0].size() << " " << SpawnPos[1].size() << " " << SpawnPos[2].size() << endl;
            boxLeft[i]++;
        }
    }

}


bool checkFront() {
    int x = (int)floor(player.camera.ProcessMouseClick1().x + .5), y = (int)floor(player.foot+1), z = (int)floor(player.camera.ProcessMouseClick1().z + .5);
    //cout << x << y << z<<endl;

    return isCube[x + 500][y + 500][z + 500];
}


float jump(int tm ) {

    //float u = 3.0;
    //float a =  .2;
    //float s = u - (a / 2) - a * tm;  //(u-a/2)+at
    //cout << tm << endl;
    float s= 0.4;

    if (tm == 10) {
        onAir = false;
        JumpCalTime = 0.0;
    }

    return s;
}

unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void addTree(glm::vec2 pos, int height) {

    int i;
    for (i = 1; i <= height; i++) {

        isCube[(int)pos.x + 500][i + 500][(int)pos.y + 500] = true;
        isCubeType[(int)pos.x + 500][i + 500][(int)pos.y + 500] = 0;
        SpawnPos[0].push_back(glm::vec3(pos.x, (float)i, pos.y));



    }



    for (int ii = -3; ii <= 3; ii++) {
        for (int jj = -3; jj <= 3; jj++) {
            if (ii | jj) {

                isCube[(int)ii + (int)pos.x + 500][height + 500][(int)pos.y + jj + 500] = true;
                isCubeType[(int)pos.x + ii + 500][height + 500][(int)pos.y + jj + 500] = 3;

                SpawnPos[3].push_back(glm::vec3(pos.x + (float)ii, (float)height, (float)pos.y + (float)jj));
            }
        }
    }

    for (int ii = -2; ii <= 2; ii++) {
        for (int jj = -2; jj <= 2; jj++) {


            isCube[(int)ii + (int)pos.x + 500][height + 1 + 500][(int)pos.y + jj + 500] = true;
            isCubeType[(int)pos.x + ii + 500][height + 1 + 500][(int)pos.y + jj + 500] = 3;

            SpawnPos[3].push_back(glm::vec3(pos.x + (float)ii, (float)height + 1.0, (float)pos.y + (float)jj));

        }
    }

}
