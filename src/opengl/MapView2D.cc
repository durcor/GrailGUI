#include "opengl/MapView2D.hh"
#include "opengl/Style.hh"
#include "opengl/BlockMapLoader.hh"  //TODO: move to util or new data directory
#include <iostream>
using namespace std;

void MapView2D::init() {
  numPoints = bml->getNumPoints();
  Shader* shader = Shader::useShader(GLWin::COMMON_SHADER);
  shader->setMat4("projection", *parentCanvas->getProjection() * transform);
	shader->setVec4("solidColor",style->getFgColor());

  glGenVertexArrays(1, &vao);  // Create the container for all vbo objects
  glBindVertexArray(vao);

  // push points up to graphics card as two separate vbo for x and y
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, numPoints * (2*sizeof(float)), bml->getXPoints(),
               GL_STATIC_DRAW);
  // Describe how information is received in shaders
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);

  // Create a buffer object for indices of lines
  uint32_t numSegments = bml->getNumSegments();
  constexpr uint32_t endIndex = 0xFFFFFFFF;
  const uint32_t numIndices = numPoints + numSegments;
  uint32_t* lineIndices = new uint32_t[numIndices];
  for (uint32_t i = 0, j = 0, c = 0; i < numSegments; i++) {
    for (uint32_t k = 0; k < bml->getSegment(i).numPoints; k++)
      lineIndices[c++] = j++;
    lineIndices[c++] = endIndex;
  }
  glGenBuffers(1, &lbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * numIndices,
               lineIndices, GL_STATIC_DRAW);

  delete[] lineIndices;
}

void debug(const glm::mat4& m, float x, float y, float z) {
  glm::vec4 v(x, y, z, 0);
  v = m * v;
  cout << v.x << "," << v.y << "," << v.z << '\n';  
}
void MapView2D::render() {


  Shader* shader = Shader::useShader(GLWin::COMMON_SHADER);
  shader->setMat4("projection", *parentCanvas->getProjection() * transform);
  glm::mat4 t = *parentCanvas->getProjection() * transform;
  debug(transform, 0,0,0);
  debug(t, 100,0,0);
  debug(t, 0,70,0);

// quick debugging rectangle in old immediate mode
  glBegin(GL_QUADS);
  glColor3f(1,0,0); // red
  glVertex2f(0,0);
  glVertex2f(200,0);
  glVertex2f(200,200);
  glVertex2f(0,200);
  glEnd();

  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(0xFFFFFFFFU);

  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glLineWidth(style->getLineWidth());

  // Draw Lines
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lbo);
  glDrawElements(GL_LINE_LOOP, numPoints, GL_UNSIGNED_INT, 0);

  // Unbind
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);
  glBindVertexArray(0);
  glDisable(GL_PRIMITIVE_RESTART);
}

void MapView2D::update() {}
void MapView2D::dump() {}