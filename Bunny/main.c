/*
    FMB algorithm implementation to perform intersection detection of pairs of static/dynamic cuboid/tetrahedron in 2D/3D by using the Fourier-Motzkin elimination method
    Copyright (C) 2020  Pascal Baillehache bayashipascal@gmail.com
    https://github.com/BayashiPascal/FMB

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// Include standard libraries
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

// Include the FMB algorithm library
#include "fmb3d.h"
#include "sat.h"

// Intersection detection mode
// 0: FMB
// 1: SAT
#ifndef MODE
  #define MODE 0
#endif

// Main function
int main(int argc, char** argv) {

#if MODE == 0
      printf("Using FMB\n");
#else
      printf("Using SAT\n");
#endif

  // Allocate memory for the vertices
  double* vertices =
    malloc(sizeof(double) * 3L * 35947L);

  // Allocate memory for the faces
  unsigned long* faces =
    malloc(sizeof(unsigned long) * 3L * 69451L);

  // Open the file containing the bunny in PLY format
  FILE* fp =
    fopen(
      "./bun_zipper.ply",
      "r");

  // Open the file to save the bunny in Pov-Ray format
  FILE* fpPov =
    fopen(
      "./bunny.inc",
      "w");

  // Open the file to save the intersection in Pov-Ray format
  FILE* fpFmb =
    fopen(
      "./fmb.inc",
      "w");

  // Skip the header
  for (
    int i = 12;
    i--;) {

    char buffer[1000];
    char* ret =
      fgets(
        buffer,
        1000,
        fp);
    (void)ret;

  }

  // Declare a variable to scale the model to avoid null determinant
  const double scale = 1000.0;

  // Read the vertices
  for (
    unsigned long iVertex =0;
    iVertex < 35947L;
    ++iVertex) {

    double buff;
    int ret = 
      fscanf(
        fp,
        "%lf %lf %lf %lf %lf\n",
        vertices + 3L * iVertex,
        vertices + 3L * iVertex + 1L,
        vertices + 3L * iVertex + 2L,
        &buff,
        &buff);
    (void)ret;

    // Scale 
    vertices[3L * iVertex] *= scale;
    vertices[3L * iVertex + 1L] *= scale;
    vertices[3L * iVertex + 2L] *= scale;

  }

  // Write the header of the bunny file
  fprintf(
    fpPov,
    "#declare bunny = union {\n");

  // Read the faces
  for (
    unsigned long iFace =0;
    iFace < 69451L;
    ++iFace) {

    unsigned long buff;
    int ret =
      fscanf(
        fp,
        "%lu %lu %lu %lu\n",
        &buff,
        faces + 3L * iFace,
        faces + 3L * iFace + 1,
        faces + 3L * iFace + 2);
    (void)ret;

    fprintf(
      fpPov,
      "triangle {<%lf,%lf,%lf>, <%lf,%lf,%lf>, <%lf,%lf,%lf>}\n",
      vertices[faces[3L * iFace] * 3L],
      vertices[faces[3L * iFace] * 3L + 1],
      vertices[faces[3L * iFace] * 3L + 2],
      vertices[faces[3L * iFace + 1] * 3L],
      vertices[faces[3L * iFace + 1] * 3L + 1],
      vertices[faces[3L * iFace + 1] * 3L + 2],
      vertices[faces[3L * iFace + 2] * 3L],
      vertices[faces[3L * iFace + 2] * 3L + 1],
      vertices[faces[3L * iFace + 2] * 3L + 2]);

  }

  // Write the tailer of the bunny file
  fprintf(
    fpPov,
    "}\n");
  

  // Write the header of the intersection file
  fprintf(
    fpFmb,
    "#declare fmb = union {\n");

  // Declare variables to create the tetrahedrons
  double orig[3];
  double comp[3][3];
  double l;

  // Loop on the faces of one bunny
  for (
    unsigned long iFace =0;
    iFace < 69451L;
    ++iFace) {

    // printf(
    //   "%05ld/69451\r",
    //   iFace);

    // Create the Tetrahedron for this face
    orig[0] = vertices[faces[3L * iFace] * 3L];
    orig[1] = vertices[faces[3L * iFace] * 3L + 1];
    orig[2] = vertices[faces[3L * iFace] * 3L + 2];
    comp[0][0] =
      vertices[faces[3L * iFace + 1] * 3L] -
      vertices[faces[3L * iFace] * 3L];
    comp[0][1] =
      vertices[faces[3L * iFace + 1] * 3L + 1] -
      vertices[faces[3L * iFace] * 3L + 1];
    comp[0][2] =
      vertices[faces[3L * iFace + 1] * 3L + 2] -
      vertices[faces[3L * iFace] * 3L + 2];
    comp[1][0] =
      vertices[faces[3L * iFace + 2] * 3L] -
      vertices[faces[3L * iFace] * 3L];
    comp[1][1] =
      vertices[faces[3L * iFace + 2] * 3L + 1] -
      vertices[faces[3L * iFace] * 3L + 1];
    comp[1][2] =
      vertices[faces[3L * iFace + 2] * 3L + 2] -
      vertices[faces[3L * iFace] * 3L + 2];
    comp[2][0] = comp[1][1] * comp[0][2] - comp[1][2] * comp[0][1];
    comp[2][1] = comp[1][2] * comp[0][0] - comp[1][0] * comp[0][2];
    comp[2][2] = comp[1][0] * comp[0][1] - comp[1][1] * comp[0][0];
    l =
      sqrt(
        comp[2][0] * comp[2][0] +
        comp[2][1] * comp[2][1] +
        comp[2][2] * comp[2][2]);
    comp[2][0] /= l;
    comp[2][1] /= l;
    comp[2][2] /= l;

    Frame3D P =
      Frame3DCreateStatic(
        FrameTetrahedron,
        orig,
        comp);

    // Loop on the faces of the mirrored/translated bunny
    for (
      unsigned long jFace =0;
      jFace < 69451L;
      ++jFace) {

      // Create the second tetrahedron
      orig[0] = -1.0 * vertices[faces[3L * jFace] * 3L] + 50.0;
      orig[1] = vertices[faces[3L * jFace] * 3L + 1] - 10.0;
      orig[2] = vertices[faces[3L * jFace] * 3L + 2];
      comp[0][0] = -1.0 * (
        vertices[faces[3L * jFace + 1] * 3L] -
        vertices[faces[3L * jFace] * 3L]);
      comp[0][1] =
        vertices[faces[3L * jFace + 1] * 3L + 1] -
        vertices[faces[3L * jFace] * 3L + 1];
      comp[0][2] =
        vertices[faces[3L * jFace + 1] * 3L + 2] -
        vertices[faces[3L * jFace] * 3L + 2];
      comp[1][0] = -1.0 * (
        vertices[faces[3L * jFace + 2] * 3L] -
        vertices[faces[3L * jFace] * 3L]);
      comp[1][1] =
        vertices[faces[3L * jFace + 2] * 3L + 1] -
        vertices[faces[3L * jFace] * 3L + 1];
      comp[1][2] =
        vertices[faces[3L * jFace + 2] * 3L + 2] -
        vertices[faces[3L * jFace] * 3L + 2];
      comp[2][0] = comp[1][1] * comp[0][2] - comp[1][2] * comp[0][1];
      comp[2][1] = comp[1][2] * comp[0][0] - comp[1][0] * comp[0][2];
      comp[2][2] = comp[1][0] * comp[0][1] - comp[1][1] * comp[0][0];
      l =
        sqrt(
          comp[2][0] * comp[2][0] +
          comp[2][1] * comp[2][1] +
          comp[2][2] * comp[2][2]);
      comp[2][0] /= l;
      comp[2][1] /= l;
      comp[2][2] /= l;

      Frame3D Q =
        Frame3DCreateStatic(
          FrameTetrahedron,
          orig,
          comp);

      // Test the intersection
#if MODE == 0
      bool inter = 
        FMB(
          &P,
          &Q);
#else
      bool inter = 
        SATTestIntersection3D(
          &P,
          &Q);
#endif

      // If there is intersection
      if (inter) {

        // Add the intersection  to the file
        double pos[3];
        pos[0] =
          (vertices[faces[3L * iFace] * 3L] +
          vertices[faces[3L * iFace + 1] * 3L] +
          vertices[faces[3L * iFace + 2] * 3L]) / 3.0;
        pos[1] =
          (vertices[faces[3L * iFace] * 3L + 1L] +
          vertices[faces[3L * iFace + 1] * 3L + 1L] +
          vertices[faces[3L * iFace + 2] * 3L + 1L]) / 3.0;
        pos[2] =
          (vertices[faces[3L * iFace] * 3L + 2L] +
          vertices[faces[3L * iFace + 1] * 3L + 2L] +
          vertices[faces[3L * iFace + 2] * 3L + 2L]) / 3.0;
        fprintf(
          fpFmb,
          "sphere {<%lf,%lf,%lf>, 1.0}\n",
          pos[0],
          pos[1],
          pos[2]);

        // Skip the other faces
        jFace = 69451L;

      }

    }

  }

  // Write the tailer of the bunny file
  fprintf(
    fpFmb,
    "}\n");
  

  // Free memory
  fclose(fp);
  fclose(fpPov);
  fclose(fpFmb);
  free(vertices);
  free(faces);

  return 0;

}
