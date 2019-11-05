#include "sat.h"

// ------------- Macros -------------

#define EPSILON 0.0000001

// ------------- Functions declaration -------------

// ------------- Functions implementation -------------

// Test for intersection between 2D Frame 'that' and 2D Frame 'tho'
// Return true if the two Frames are intersecting, else false
bool SATTestIntersection2D(
  const Frame* const that, 
  const Frame* const tho) {

  // Declare a variable to loop on Frames and commonalize code
  const Frame* frameEdge = that;

  // Loop to commonalize code when checking SAT based on that's edges
  // and then tho's edges
  for (int iTest = 2;
       iTest--;) {

    // Shortcuts
    FrameType frameEdgeType = frameEdge->type;
    const double* frameEdgeCompA = frameEdge->comp[0];
    const double* frameEdgeCompB = frameEdge->comp[1];
    
    // Declare a variable to memorize the number of edges, by default 2
    int nbEdges = 2;

    // Declare a variable to memorize the third edge in case of
    // tetrahedron
    double thirdEdge[2];

    // If the frame is a tetrahedron
    if (frameEdgeType == FrameTetrahedron) {

      // Initialise the third edge
      thirdEdge[0] = frameEdgeCompB[0] - frameEdgeCompA[0];
      thirdEdge[1] = frameEdgeCompB[1] - frameEdgeCompA[1];

      // Correct the number of edges
      nbEdges = 3;

    }

    // Loop on the frame's edges
    for (int iEdge = nbEdges;
         iEdge--;) {

      // Get the current edge
      const double* edge = 
        (iEdge == 2 ? thirdEdge : frameEdge->comp[iEdge]);

      // Declare variables to memorize the boundaries of projection
      // of the two frames on the current edge
      double bdgBoxA[2];
      double bdgBoxB[2];

      // Declare two variables to loop on Frames and commonalize code
      const Frame* frame = that;
      double* bdgBox = bdgBoxA;

      // Loop on Frames
      for (int iFrame = 2;
          iFrame--;) {
      
        // Shortcuts
        const double* frameOrig = frame->orig;
        const double* frameCompA = frame->comp[0];
        const double* frameCompB = frame->comp[1];
        FrameType frameType = frame->type;

        // Get the number of vertices of frame
        int nbVertices = (frameType == FrameTetrahedron ? 3 : 4);

        // Declare a variable to memorize if the current vertex is
        // the first in the loop, used to initialize the boundaries
        bool firstVertex = true;

        // Loop on vertices of the frame
        for (int iVertex = nbVertices;
             iVertex--;) {
          
          // Get the vertex
          double vertex[2];
          memcpy(vertex, frameOrig, 2 * sizeof(double));
          if (iVertex == 3) {

            vertex[0] += frameCompA[0] + frameCompB[0];
            vertex[1] += frameCompA[1] + frameCompB[1];

          } else if (iVertex == 2) {

            vertex[0] += frameCompA[0];
            vertex[1] += frameCompA[1];

          } else if (iVertex == 1) {

            vertex[0] += frameCompB[0];
            vertex[1] += frameCompB[1];

          }

          // Get the projection of the vertex on the normal of the edge
          // Orientation of the normal doesn't matter, so we
          // consider here the normal (edge[1], -edge[0])
          double proj = vertex[0] * edge[1] - vertex[1] * edge[0];

          // Update the boundaries of the projection of the Frame on
          // the edge
          if (firstVertex == true) {

              bdgBox[0] = proj;
              bdgBox[1] = proj;
              firstVertex = false;
            
          } else {

            if (bdgBox[0] > proj) {
              bdgBox[0] = proj;
            }
            if (bdgBox[1] < proj) {
              bdgBox[1] = proj;
            }

          }

        }

        // Switch the frame to check the vertices of the second Frame
        frame = tho;
        bdgBox = bdgBoxB;

      }

      // If the projections of the two frames on the edge are
      // not intersecting
      if (bdgBoxB[1] < bdgBoxA[0] || 
          bdgBoxA[1] < bdgBoxB[0]) {

         // There exists an axis which separates the Frames,
         // thus they are not in intersection
         return false;

      }

    }

    // Switch the frames to test against the second Frame's edges
    frameEdge = tho;

  }

  return true;

}

#if FRAME_NB_DIM == 3
// Test for intersection between 3D Frame 'that' and 3D Frame 'tho'
// Return true if the two Frames are intersecting, else false
bool SATTestIntersection3D(
  const Frame* const that, 
  const Frame* const tho) {

  return false;

}
#endif
