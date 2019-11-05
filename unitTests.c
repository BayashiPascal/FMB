// Include standard libraries
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// Include FMB algorithm library
#include "fmb.h"

#define EPSILON 0.0001

typedef struct {
  FrameType type;
  double orig[FRAME_NB_DIM];
  double comp[FRAME_NB_DIM][FRAME_NB_DIM];
} Param;

void UnitTest(
        const Param paramP,
        const Param paramQ,
         const bool answer,
  const AABB* const correctBdgBox) {

  Frame P = 
    FrameCreateStatic(
      paramP.type,
      paramP.orig,
      paramP.comp);
  
  Frame Q = 
    FrameCreateStatic(
      paramQ.type,
      paramQ.orig,
      paramQ.comp);

  AABB bdgBox;
  Frame* that = &P;
  Frame* tho = &Q;
  
  for (int iTest = 0;
       iTest < 2;
       ++iTest) {

    FramePrint(that);
    printf(" against ");
    FramePrint(tho);

    bool isIntersecting = 
      FMBTestIntersection(
        that, 
        tho, 
        &bdgBox);

    if (isIntersecting != answer) {

      printf(" Failed\n");
      printf("Expected : ");
      if (answer == false) {
        printf("no ");
      }
      printf("intersection\n");
      printf("Got : ");
      if (isIntersecting == false) {
        printf("no ");
      }
      printf("intersection\n");
      exit(0);

    } else {

      if (isIntersecting == true) {

        bool flag = true;
        for (int i = FRAME_NB_DIM;
             i--;) {
          if (bdgBox.min[i] > correctBdgBox->min[i] + EPSILON ||
              bdgBox.max[i] < correctBdgBox->max[i] - EPSILON) {

            flag = false;

          }

        }

        if (flag == true) {

          printf(" Succeed\n");

        } else {

          printf(" Failed\n");
          printf("Expected : ");
          AABBPrint(correctBdgBox);
          printf("\n");
          printf("     Got : ");
          AABBPrint(&bdgBox);
          printf("\n");
          exit(0);

        }

      } else {

        printf(" Succeed\n");

      }

    }

    that = &Q;
    tho = &P;

  }
  
}

// Main function
int main(int argc, char** argv) {

  Param paramP;
  Param paramQ;
  AABB correctBdgBox;

#if FRAME_NB_DIM == 3

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0, 0.0},
     .comp = 
       {{1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0, 0.0},
     .comp = 
       {{1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.0, 0.0, 0.0},
     .max = {1.0, 1.0, 1.0}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0, 0.0},
     .comp = 
       {{1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {0.5, 0.5, 0.5},
     .comp = 
       {{1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.5, 0.5, 0.5},
     .max = {1.0, 1.0, 1.0}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0, 0.0},
     .comp = 
       {{1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {-0.5, -0.5, -0.5},
     .comp = 
       {{1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.0, 0.0, 0.0},
     .max = {0.5, 0.5, 0.5}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0, 0.0},
     .comp = 
       {{1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {1.5, 1.5, 1.5},
     .comp = 
       {{-1.0, 0.0, 0.0},
        {0.0, -1.0, 0.0},
        {0.0, 0.0, -1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.5, 0.5, 0.5},
     .max = {1.0, 1.0, 1.0}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0, 0.0},
     .comp = 
       {{1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {0.5, 1.5, -1.5},
     .comp = 
       {{1.0, 0.0, 0.0},
        {0.0, -1.0, 0.0},
        {0.0, 0.0, 1.0}}
    };
  UnitTest(
    paramP,
    paramQ,
    false,
    NULL);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0, 0.0},
     .comp = 
       {{1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, -1.0}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {0.5, 1.5, -1.5},
     .comp = 
       {{1.0, 0.0, 0.0},
        {0.0, -1.0, 0.0},
        {0.0, 0.0, 1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.5, 0.5, -1.0},
     .max = {1.0, 1.0, -0.5}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {-1.0, -1.0, -1.0},
     .comp = 
       {{1.0, 0.0, 0.0},
        {1.0, 1.0, 1.0},
        {0.0, 0.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0, 0.0},
     .comp = 
       {{1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}}
    };
  UnitTest(
    paramP,
    paramQ,
    false,
    NULL);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {-1.0, -1.0, -1.0},
     .comp = 
       {{1.0, 0.0, 0.0},
        {1.0, 1.0, 1.0},
        {0.0, 0.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, -0.5, 0.0},
     .comp = 
       {{1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.0, -0.5, 0.0},
     .max = {1.0, 0.0, 1.0}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameTetrahedron,
     .orig = {-1.0, -1.0, -1.0},
     .comp = 
       {{1.0, 0.0, 0.0},
        {1.0, 1.0, 1.0},
        {0.0, 0.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, -0.5, 0.0},
     .comp = 
       {{1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}}
    };
  UnitTest(
    paramP,
    paramQ,
    false,
    NULL);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {-1.0, -1.0, -1.0},
     .comp = 
       {{1.0, 0.0, 0.0},
        {1.0, 1.0, 1.0},
        {0.0, 0.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameTetrahedron,
     .orig = {0.0, -0.5, 0.0},
     .comp = 
       {{1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.0, -0.5, 0.0},
     .max = {0.75, 0.0, 0.75}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameTetrahedron,
     .orig = {-1.0, -1.0, -1.0},
     .comp = 
       {{1.0, 0.0, 0.0},
        {1.0, 1.0, 1.0},
        {0.0, 0.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameTetrahedron,
     .orig = {0.0, -0.5, 0.0},
     .comp = 
       {{1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}}
    };
  UnitTest(
    paramP,
    paramQ,
    false,
    NULL);

  // ----------------------------
  paramP = (Param)
    {.type = FrameTetrahedron,
     .orig = {-0.5, -1.0, -0.5},
     .comp = 
       {{1.0, 0.0, 0.0},
        {1.0, 1.0, 1.0},
        {0.0, 0.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameTetrahedron,
     .orig = {0.0, -0.5, 0.0},
     .comp = 
       {{1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.0, -0.5, 0.0},
     .max = {0.5, -0.5 + 1.0 / 3.0, 0.5}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

#elif FRAME_NB_DIM == 2

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.0, 0.0},
     .max = {1.0, 1.0}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {0.5, 0.5},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.5, 0.5},
     .max = {1.0, 1.0}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {-0.5, -0.5},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {0.5, 0.5},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}}
    };
  UnitTest(
    paramP,
    paramQ,
    false,
    NULL);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {0.25, -0.25},
     .comp = 
       {{0.5, 0.0},
        {0.0, 2.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.25, 0.0},
     .max = {0.75, 1.0}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {-0.25, 0.25},
     .comp = 
       {{2.0, 0.0},
        {0.0, 0.5}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.0, 0.25},
     .max = {1.0, 0.75}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 1.0},
        {-1.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.0, 0.0},
     .max = {1.0, 1.0}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {-0.5, -0.5},
     .comp = 
       {{1.0, 1.0},
        {-1.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.0, 0.0},
     .max = {0.5, 1.0}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {1.5, 1.5},
     .comp = 
       {{1.0, -1.0},
        {-1.0, -1.0}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {1.0, 0.0},
     .comp = 
       {{-1.0, 0.0},
        {0.0, 1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.5, 0.0},
     .max = {1.0, 1.0}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {1.0, 0.5},
     .comp = 
       {{-0.5, 0.5},
        {-0.5, -0.5}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 1.0},
     .comp = 
       {{1.0, 0.0},
        {0.0, -1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.0, 0.0},
     .max = {1.0, 1.0}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.0},
        {1.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {2.0, -1.0},
     .comp = 
       {{0.0, 1.0},
        {-0.5, 1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {1.5, 0.5},
     .max = {1.5 + 0.5 / 3.0, 1.0}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.5},
        {0.5, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {1.0, 1.0},
     .comp = 
       {{-0.5, -0.5},
        {0.0, -1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.5, 0.25},
     .max = {1.0, 1.0}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.5},
        {0.5, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {1.0, 2.0},
     .comp = 
       {{-0.5, -0.5},
        {0.0, -1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.5, 0.75},
     .max = {1.0, 1.25}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameTetrahedron,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.5},
        {0.5, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameCuboid,
     .orig = {1.0, 2.0},
     .comp = 
       {{-0.5, -0.5},
        {0.0, -1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.5, 0.5},
     .max = {0.75, 1.0}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.5},
        {0.5, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameTetrahedron,
     .orig = {1.0, 2.0},
     .comp = 
       {{-0.5, -0.5},
        {0.0, -1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.5 + 1.0 / 3.0, 1.0},
     .max = {1.0, 1.0 + 1.0 / 3.0}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameTetrahedron,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.0, 0.0},
     .max = {1.0, 1.0}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameTetrahedron,
     .orig = {0.0, -0.5},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.0, 0.0},
     .max = {0.5, 0.5}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {0.5, 0.5},
     .comp = 
       {{-0.5, 0.0},
        {0.0, -0.5}}
    };
  paramQ = (Param)
    {.type = FrameTetrahedron,
     .orig = {0.0, -0.5},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.0, 0.0},
     .max = {0.5, 0.5}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {0.5, 0.5},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameTetrahedron,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}}
    };
  UnitTest(
    paramP,
    paramQ,
    false,
    NULL);

  // ----------------------------
  paramP = (Param)
    {.type = FrameCuboid,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameTetrahedron,
     .orig = {1.5, 1.5},
     .comp = 
       {{-1.5, 0.0},
        {0.0, -1.5}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.5, 0.5},
     .max = {1.0, 1.0}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameTetrahedron,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.0},
        {0.0, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameTetrahedron,
     .orig = {1.0, 1.0},
     .comp = 
       {{-1.0, 0.0},
        {0.0, -1.0}}
    };
  UnitTest(
    paramP,
    paramQ,
    false,
    NULL);

  // ----------------------------
  paramP = (Param)
    {.type = FrameTetrahedron,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.5},
        {0.5, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameTetrahedron,
     .orig = {1.0, 1.0},
     .comp = 
       {{-0.5, -0.5},
        {0.0, -1.0}}
    };
  correctBdgBox = (AABB) 
    {.min = {0.5, 0.5 - 1.0 / 6.0},
     .max = {1.0, 0.75}
    };
  UnitTest(
    paramP,
    paramQ,
    true,
    &correctBdgBox);

  // ----------------------------
  paramP = (Param)
    {.type = FrameTetrahedron,
     .orig = {0.0, 0.0},
     .comp = 
       {{1.0, 0.5},
        {0.5, 1.0}}
    };
  paramQ = (Param)
    {.type = FrameTetrahedron,
     .orig = {1.0, 1.5},
     .comp = 
       {{-0.5, -0.5},
        {0.0, -1.0}}
    };
  UnitTest(
    paramP,
    paramQ,
    false,
    NULL);

#else

  printf("Not implemented for dimension %d\n", FRAME_NB_DIM);
  exit(0);

#endif
  
  printf("All unit tests have succeed.\n");
  
  return 0;
}
