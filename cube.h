
#ifndef CUBE_H
#define CUBE_H

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

class Cube
{
private:
    unsigned int cubeVAO;
    unsigned int lightCubeVAO;
    unsigned int lightTexCubeVAO;
    unsigned int cubeVBO;
    unsigned int cubeEBO;
    glm::vec3 rotation_;

public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float shininess;
    float speed = 2.0f;

    glm::vec3 currentPos;

    unsigned int diffuseMap;
    unsigned int specularMap;



    Cube(glm::vec3 amb = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 diff = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 spec = glm::vec3(0.0f, 0.0f, 0.0f), float shiny = 32.0f)
    {
        set(amb, diff, spec, shiny);
        buildVertices();

    }

    ~Cube() {}

    void set(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {

        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;
    }


    void setTextureProperty(unsigned int dMap, unsigned int sMap, float shiny = 32.0f)
    {
        this->diffuseMap = dMap;
        this->specularMap = sMap;
        this->shininess = shiny;
    }


    void drawCubee(Shader& lightingShader, glm::mat4 model) const
    {   



        lightingShader.use();

        lightingShader.setVec3("material.ambient", ambient);
        lightingShader.setVec3("material.diffuse", diffuse);
        lightingShader.setVec3("material.specular", specular);
        lightingShader.setFloat("material.shininess", shininess);

        lightingShader.setMat4("model", model);

        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

    }

    void drawCubeWithTexture(Shader& lightingShaderWithTexture, glm::mat4 model = glm::mat4(1.0f))
    {
        lightingShaderWithTexture.use();

        lightingShaderWithTexture.setInt("material.diffuse", 0);
        lightingShaderWithTexture.setInt("material.specular", 1);
        lightingShaderWithTexture.setFloat("material.shininess", this->shininess);


        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->diffuseMap);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, this->specularMap);

        lightingShaderWithTexture.setMat4("model", model);

        glBindVertexArray(lightTexCubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }


    void drawCubeeWithPos(Shader& lightingShader, glm::vec3 CurrentPos) 
    {
        this->currentPos = CurrentPos;

        glm::mat4 identityMatrix = glm::mat4(1.0f);
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;

        translateMatrix = glm::translate(identityMatrix, currentPos);
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0, 1.0, 1.0));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;

        lightingShader.use();

        lightingShader.setVec3("material.ambient", ambient);
        lightingShader.setVec3("material.diffuse", diffuse);
        lightingShader.setVec3("material.specular", specular);
        lightingShader.setFloat("material.shininess", shininess);

        lightingShader.setMat4("model", model);

        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);

    }



private:


    void buildVertices()
    {

        float cube_vertices1[] = {
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

        float cube_vertices[] = {
            // positions      // normals
            -.5f, -.5f, -.5f, 0.0f, 0.0f, -1.0f,
            .5f, -.5f, -.5f, 0.0f, 0.0f, -1.0f,
            .5f, .5f, -.5f, 0.0f, 0.0f, -1.0f,
            -.5f, .5f, -.5f, 0.0f, 0.0f, -1.0f,

            .5f, -.5f, -.5f,   1.0f, 0.0f, 0.0f,
            .5f, .5f, -.5f,    1.0f, 0.0f, 0.0f,
            .5f, -.5f, .5f,   1.0f, 0.0f, 0.0f,
            .5f, .5f, .5f,     1.0f, 0.0f, 0.0f,

            -.5f, -.5f, .5f,  0.0f, 0.0f, 1.0f,
            .5f, -.5f, .5f,  0.0f, 0.0f, 1.0f,
            .5f, .5f, .5f,  0.0f, 0.0f, 1.0f,
            -.5f, .5f, .5f,  0.0f, 0.0f, 1.0f,

            -.5f, -.5f, .5f, -1.0f, 0.0f, 0.0f,
            -.5f, .5f, .5f, -1.0f, 0.0f, 0.0f,
            -.5f, .5f, -.5f, -1.0f, 0.0f, 0.0f,
            -.5f, -.5f, -.5f, -1.0f, 0.0f, 0.0f,

            .5f, .5f, .5f,    0.0f, 1.0f, 0.0f,
            .5f, .5f, -.5f,   0.0f, 1.0f, 0.0f,
            -.5f, .5f, -.5f,  0.0f, 1.0f, 0.0f,
            -.5f, .5f, .5f,   0.0f, 1.0f, 0.0f,

            -.5f, -.5f, -.5f,  0.0f, -1.0f, 0.0f,
            .5f, -.5f, -.5f,   0.0f, -1.0f, 0.0f,
            .5f, -.5f, .5f,    0.0f, -1.0f, 0.0f,
            -.5f, -.5f, .5f,   0.0f, -1.0f, 0.0f
        };

        float cube_vertices2[] = {
            // positions      // normals         // texture
            0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0, 0.5,
            1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0, 0.5,
            1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0, 1.0,
            0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0, 1.0,

            1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0, 0.5,
            1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0, 1.0,
            1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0, 1.0,
            1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0, 0.5,

            0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0, 0.5,
            1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0, 0.5,
            1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0, 1.0,
            0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0, 1.0,

            0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0, 0.5,
            0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0, 1.0,
            0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0, 1.0,
            0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0, 0.5,

            1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0, 0.0,
            1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0, 0.0,
            0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0, 0.5,
            0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0, 0.5,

            0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0, 0.0,
            1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0, 0.0,
            1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0, 0.5,
            0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0, 0.5,
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


        glGenVertexArrays(1, &cubeVAO);
        glGenVertexArrays(1, &lightCubeVAO);
        glGenVertexArrays(1, &lightTexCubeVAO);
        glGenBuffers(1, &cubeVBO);
        glGenBuffers(1, &cubeEBO);


        glBindVertexArray(lightTexCubeVAO);

        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices2), cube_vertices2, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // vertex normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)12);
        glEnableVertexAttribArray(1);

        // texture coordinate attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)24);
        glEnableVertexAttribArray(2);


        glBindVertexArray(lightCubeVAO);

        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)12);
        glEnableVertexAttribArray(1);


        glBindVertexArray(cubeVAO);

        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }


};

#endif // CUBE_H
