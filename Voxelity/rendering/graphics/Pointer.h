//
// Created by adrian on 20/05/25.
//

#ifndef POINTER_H
#define POINTER_H

#include "GLT.h"
#include "rendering/shader/Shader.h"

class Pointer {
    glm::ivec3 blockPos;   // position du bloc ciblé (int)
    glm::vec4 color;       // couleur RGBA du cube
    Shader shader;

    // Mesh simple : un cube unité centré sur l'origine, à dessiner avec model transform
    GLuint vao = 0, vbo = 0;

    void initMesh() {
        // Cube vertices (positions seulement) - cube 1x1x1, origine en (0,0,0)
        float vertices[] = {
            // positions des 8 sommets du cube
            0,0,0,
            1,0,0,
            1,1,0,
            0,1,0,
            0,0,1,
            1,0,1,
            1,1,1,
            0,1,1,
        };

        // Indices pour dessiner 12 triangles (6 faces)
        unsigned int indices[] = {
            0,1,2, 2,3,0, // face avant
            1,5,6, 6,2,1, // face droite
            5,4,7, 7,6,5, // face arrière
            4,0,3, 3,7,4, // face gauche
            3,2,6, 6,7,3, // face haut
            4,5,1, 1,0,4  // face bas
        };

        // Création VAO, VBO, EBO (simplifié, vérifier erreur)
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        GLuint ebo;
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Attribut position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), static_cast<void *>(nullptr));

        glBindVertexArray(0);
    }

public:
    explicit Pointer(const glm::vec4& color = glm::vec4(1,1,0,1)) // jaune par défaut
    : blockPos(0), color(color), shader("../resources/shaders/pointer/vertex.glsl",
                                        "../resources/shaders/pointer/fragment.glsl") {
        initMesh();
    }

    ~Pointer() {
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }

    void setBlockPosition(const glm::ivec3& pos) {
        blockPos = pos;
    }

    void render(const glm::mat4& view, const glm::mat4& proj) const {
        shader.use();

        // model matrix pour positionner le cube à blockPos
        const glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(blockPos));

        shader.setUniform("u_model", model);
        shader.setUniform("u_view", view);
        shader.setUniform("u_proj", proj);
        shader.setUniform("u_color", color);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }
};

#endif //POINTER_H
