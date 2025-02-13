#include "OpenGL.Quad.h"

void Quad::Init() {
    float vertices[] = {

        -1.0f,  1.0f, 0.f,  0.0f, 1.0f,  // Верхний левый угол
        -1.0f, -1.0f, 0.f,  0.0f, 0.0f,  // Нижний левый угол
         1.0f, -1.0f, 0.f,  1.0f, 0.0f,  // Нижний правый угол

        -1.0f,  1.0f, 0.f,   0.0f, 1.0f,  // Верхний левый угол
         1.0f, -1.0f, 0.f,   1.0f, 0.0f,  // Нижний правый угол
         1.0f,  1.0f, 0.f,   1.0f, 1.0f   // Верхний правый угол
    };


    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);


    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texture coord
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Quad::Draw() {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}


Quad::Quad() {
    Init();
}

Quad::~Quad()
{
}