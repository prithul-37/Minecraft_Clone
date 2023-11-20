
#ifndef tree_h
#define tree_h

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "cube.h"

class Tree
{
public:
    glm::vec3 position;
    Cube wood = Cube(glm::vec3(0.58, 0.094, 0.094), glm::vec3(0.58, 0.094, 0.094));
    Cube leaf = Cube(glm::vec3(0.11, 0.678, 0.247), glm::vec3(0.11, 0.678, 0.247));
        float height = 4.0f;

        float leafRed = 3.0f;
        float leafHeight = 2.0f;

    

    Tree(glm ::vec3 positionn = glm :: vec3 (0.0, 0.0 ,0.0)) {
        this->position = positionn;
    }



    void drawTree(Shader& lightingShader) {

        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0, 0.0, 0.0));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0, 1.0, 1.0));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;

        glm::mat4 modelForCube = glm::mat4(1.0f);
 
        glm :: vec3 woodPos = this->position;


        for (float i = 0; i < this->height; i++)
        {   
            
            woodPos.y += 1;

            woodPos.x -= .5;
            woodPos.y -= .5;
            woodPos.z -= .5;
            
            modelForCube = glm::translate(model, woodPos);
            wood.drawCubee(lightingShader, modelForCube);
        }

        //leaf

        glm::vec3 leafPos = this->position;
        leafPos.y += height;
        float lfht = leafHeight;
        float gridSize = this->leafRed;

        while (lfht) {


            for (float i = -gridSize; i < gridSize; i = i + 1.0f)
            {
                for (float j = -gridSize; j < gridSize; j = j + 1.0f)
                {   

                   // std::cout << leafPos.x << " " << leafPos.z<<endl;

                    leafPos.x = this->position.x + i;
                    leafPos.z = this->position.z + j;

                    leafPos.x -= .5;
                    leafPos.y -= .5;
                    leafPos.z -= .5;

;                   modelForCube = glm::translate(model, leafPos);

                    leaf.drawCubee(lightingShader, modelForCube);
                }
            }
            gridSize--;
            lfht--;
            leafPos.y++;
        }

        

    }

 

private:


    


};

#endif // CUBE_H
