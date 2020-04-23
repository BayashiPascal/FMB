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

#ifndef __FRAME_H_
#define __FRAME_H_

// ------------- Includes -------------

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// ------------- Macros -------------

// ------------- Enumerations -------------

typedef enum {

  FrameCuboid,
  FrameTetrahedron

} FrameType;

// ------------- Data structures -------------

// Axis aligned bounding box structure
typedef struct {

  // x, y
  double min[2];
  double max[2];

} AABB2D;

typedef struct {

  // x, y, z
  double min[3];
  double max[3];

} AABB3D;

typedef struct {

  // x, y, t
  double min[3];
  double max[3];

} AABB2DTime;

typedef struct {

  // x, y, z, t
  double min[4];
  double max[4];

} AABB3DTime;

// Axis unaligned cuboid and tetrahedron structure
typedef struct {

  FrameType type;
  double orig[2];
  double comp[2][2];

  // AABB of the frame
  AABB2D bdgBox;

  // Inverted components used during computation
  double invComp[2][2];

} Frame2D;

typedef struct {

  FrameType type;
  double orig[3];
  double comp[3][3];

  // AABB of the frame
  AABB3D bdgBox;

  // Inverted components used during computation
  double invComp[3][3];

} Frame3D;

typedef struct {

  FrameType type;
  double orig[2];
  double comp[2][2];

  // AABB of the frame
  AABB2DTime bdgBox;

  // Inverted components used during computation
  double invComp[2][2];
  double speed[2];

} Frame2DTime;

typedef struct {

  FrameType type;
  double orig[3];
  double comp[3][3];

  // AABB of the frame
  AABB3DTime bdgBox;

  // Inverted components used during computation
  double invComp[3][3];
  double speed[3];

} Frame3DTime;

// ------------- Functions declaration -------------

// Print the AABB that on stdout
// Output format is
// (min[0], min[1], min[2], min[3])-(max[0], max[1], max[2], max[3])
void AABB2DPrint(const AABB2D* const that);
void AABB3DPrint(const AABB3D* const that);
void AABB2DTimePrint(const AABB2DTime* const that);
void AABB3DTimePrint(const AABB3DTime* const that);

// Print the Frame that on stdout
// Output format is
// T/C  <- type of Frame
// o(orig[0], orig[1], orig[2])
// s(speed[0], speed[1], speed[2])
// x(comp[0][0], comp[0][1], comp[0][2])
// y(comp[1][0], comp[1][1], comp[1][2])
// z(comp[2][0], comp[2][1], comp[2][2])
void Frame2DPrint(const Frame2D* const that);
void Frame3DPrint(const Frame3D* const that);
void Frame2DTimePrint(const Frame2DTime* const that);
void Frame3DTimePrint(const Frame3DTime* const that);

// Create a static Frame structure of FrameType type,
// at position orig with components comp ([iComp][iAxis])
// and speed
Frame2D Frame2DCreateStatic(
  const FrameType type,
  const double orig[2],
  const double comp[2][2]);
Frame3D Frame3DCreateStatic(
  const FrameType type,
  const double orig[3],
  const double comp[3][3]);
Frame2DTime Frame2DTimeCreateStatic(
  const FrameType type,
  const double orig[2],
  const double speed[2],
  const double comp[2][2]);
Frame3DTime Frame3DTimeCreateStatic(
  const FrameType type,
  const double orig[3],
  const double speed[3],
  const double comp[3][3]);

// Project the Frame Q in the Frame P's coordinates system and
// memorize the result in the Frame Qp
void Frame2DImportFrame(
  Frame2D* const P,
  const Frame2D* const Q,
  Frame2D* const Qp);
void Frame3DImportFrame(
  Frame3D* const P,
  const Frame3D* const Q,
  Frame3D* const Qp);
void Frame2DTimeImportFrame(
  Frame2DTime* const P,
  const Frame2DTime* const Q,
  Frame2DTime* const Qp);
void Frame3DTimeImportFrame(
  Frame3DTime* const P,
  const Frame3DTime* const Q,
  Frame3DTime* const Qp);

// Export the AABB bdgBox from that's coordinates system to
// the real coordinates system and update bdgBoxProj with the resulting
// AABB
void Frame2DExportBdgBox(
  const Frame2D* const that,
  const AABB2D* const bdgBox,
  AABB2D* const bdgBoxProj);
void Frame3DExportBdgBox(
  const Frame3D* const that,
  const AABB3D* const bdgBox,
  AABB3D* const bdgBoxProj);
void Frame2DTimeExportBdgBox(
  const Frame2DTime* const that,
  const AABB2DTime* const bdgBox,
  AABB2DTime* const bdgBoxProj);
void Frame3DTimeExportBdgBox(
  const Frame3DTime* const that,
  const AABB3DTime* const bdgBox,
  AABB3DTime* const bdgBoxProj);

// Check the intersection between two static AABB that and tho
bool AABBTestIntersection2D(
  const AABB2D* const that,
  const AABB2D* const tho);
bool AABBTestIntersection3D(
  const AABB3D* const that,
  const AABB3D* const tho);

// Power function for integer base and exponent
// Return base^exp
int powi(
  int base,
  unsigned int exp);

#endif
