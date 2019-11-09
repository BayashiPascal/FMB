#include "sat.h"

// ------------- Macros -------------

#define EPSILON 0.0000001

// ------------- Functions declaration -------------

// ------------- Functions implementation -------------

#if FRAME_NB_DIM == 2
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
          switch (iVertex) {
            case 3:
              vertex[0] += frameCompA[0] + frameCompB[0];
              vertex[1] += frameCompA[1] + frameCompB[1];
              break;
            case 2:
              vertex[0] += frameCompA[0];
              vertex[1] += frameCompA[1];
              break;
            case 1:
              vertex[0] += frameCompB[0];
              vertex[1] += frameCompB[1];
              break;
            default:
              break;
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
#endif

#if FRAME_NB_DIM == 3
// Test for intersection between 3D Frame 'that' and 3D Frame 'tho'
// Return true if the two Frames are intersecting, else false
bool SATTestIntersection3D(
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
    const double* frameEdgeCompC = frameEdge->comp[2];
    
    // Declare a variable to memorize the number of edges, by default 3
    int nbEdges = 3;

    // Declare a variable to memorize the three opposite edges in case
    // of tetrahedron
    // Arrangement is normFaces[iEdge][iAxis]
    double oppEdges[3][3];

    // If the frame is a tetrahedron
    if (frameEdgeType == FrameTetrahedron) {

      // Initialise the opposite edges
      oppEdges[0][0] = frameEdgeCompB[0] - frameEdgeCompA[0];
      oppEdges[0][1] = frameEdgeCompB[1] - frameEdgeCompA[1];
      oppEdges[0][2] = frameEdgeCompB[2] - frameEdgeCompA[2];

      oppEdges[1][0] = frameEdgeCompB[0] - frameEdgeCompC[0];
      oppEdges[1][1] = frameEdgeCompB[1] - frameEdgeCompC[1];
      oppEdges[1][2] = frameEdgeCompB[2] - frameEdgeCompC[2];

      oppEdges[2][0] = frameEdgeCompC[0] - frameEdgeCompA[0];
      oppEdges[2][1] = frameEdgeCompC[1] - frameEdgeCompA[1];
      oppEdges[2][2] = frameEdgeCompC[2] - frameEdgeCompA[2];

      // Correct the number of edges
      nbEdges = 6;

    }

    // Loop on the frame's edges
    for (int iEdge = nbEdges;
         iEdge--;) {

      // Get the current edge
      const double* edge = 
        (iEdge < 3 ? frameEdge->comp[iEdge] : oppEdges[iEdge - 3]);

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
        const double* frameCompC = frame->comp[2];
        FrameType frameType = frame->type;

        // Get the number of vertices of frame
        int nbVertices = (frameType == FrameTetrahedron ? 4 : 8);

        // Declare a variable to memorize if the current vertex is
        // the first in the loop, used to initialize the boundaries
        bool firstVertex = true;

        // Loop on vertices of the frame
        for (int iVertex = nbVertices;
             iVertex--;) {
          
          // Get the vertex
          double vertex[3];
          memcpy(vertex, frameOrig, 3 * sizeof(double));
          switch (iVertex) {
            case 7:
              vertex[0] += 
                frameCompA[0] +  frameCompB[0] + frameCompC[0];
              vertex[1] += 
                frameCompA[1] +  frameCompB[1] + frameCompC[1];
              vertex[2] += 
                frameCompA[2] +  frameCompB[2] + frameCompC[2];
              break;
            case 6:
              vertex[0] += frameCompB[0] + frameCompC[0];
              vertex[1] += frameCompB[1] + frameCompC[1];
              vertex[2] += frameCompB[2] + frameCompC[2];
              break;
            case 5:
              vertex[0] += frameCompA[0] + frameCompC[0];
              vertex[1] += frameCompA[1] + frameCompC[1];
              vertex[2] += frameCompA[2] + frameCompC[2];
              break;
            case 4:
              vertex[0] += frameCompA[0] + frameCompB[0];
              vertex[1] += frameCompA[1] + frameCompB[1];
              vertex[2] += frameCompA[2] + frameCompB[2];
              break;
            case 3:
              vertex[0] += frameCompC[0];
              vertex[1] += frameCompC[1];
              vertex[2] += frameCompC[2];
              break;
            case 2:
              vertex[0] += frameCompB[0];
              vertex[1] += frameCompB[1];
              vertex[2] += frameCompB[2];
              break;
            case 1:
              vertex[0] += frameCompA[0];
              vertex[1] += frameCompA[1];
              vertex[2] += frameCompA[2];
              break;
            default:
              break;
          }

          // Get the projection of the vertex on the edge
          double proj = 
            vertex[0] * edge[0] + 
            vertex[1] * edge[1] + 
            vertex[2] * edge[2];

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

    // Declare a variable to memorize the number of faces, by default 3
    int nbFaces = 3;

    // Declare a variable to memorize the normal to faces
    // Arrangement is normFaces[iFace][iAxis]
    double normFaces[4][3];

    // Initialise the normal to faces
    normFaces[0][0] = 
      frameEdgeCompA[1] * frameEdgeCompB[2] -
      frameEdgeCompA[2] * frameEdgeCompB[1];
    normFaces[0][1] = 
      frameEdgeCompA[2] * frameEdgeCompB[0] -
      frameEdgeCompA[0] * frameEdgeCompB[2];
    normFaces[0][2] = 
      frameEdgeCompA[0] * frameEdgeCompB[1] -
      frameEdgeCompA[1] * frameEdgeCompB[0];

    normFaces[1][0] = 
      frameEdgeCompA[1] * frameEdgeCompC[2] -
      frameEdgeCompA[2] * frameEdgeCompC[1];
    normFaces[1][1] = 
      frameEdgeCompA[2] * frameEdgeCompC[0] -
      frameEdgeCompA[0] * frameEdgeCompC[2];
    normFaces[1][2] = 
      frameEdgeCompA[0] * frameEdgeCompC[1] -
      frameEdgeCompA[1] * frameEdgeCompC[0];

    normFaces[2][0] = 
      frameEdgeCompC[1] * frameEdgeCompB[2] -
      frameEdgeCompC[2] * frameEdgeCompB[1];
    normFaces[2][1] = 
      frameEdgeCompC[2] * frameEdgeCompB[0] -
      frameEdgeCompC[0] * frameEdgeCompB[2];
    normFaces[2][2] = 
      frameEdgeCompC[0] * frameEdgeCompB[1] -
      frameEdgeCompC[1] * frameEdgeCompB[0];

    // If the frame is a tetrahedron
    if (frameEdgeType == FrameTetrahedron) {

      // Shortcuts
      double* oppEdgeA = oppEdges[0];
      double* oppEdgeB = oppEdges[1];

      // Initialise the normal to the opposite face
      normFaces[3][0] = 
        oppEdgeA[1] * oppEdgeB[2] -
        oppEdgeA[2] * oppEdgeB[1];
      normFaces[3][1] = 
        oppEdgeA[2] * oppEdgeB[0] -
        oppEdgeA[0] * oppEdgeB[2];
      normFaces[3][2] = 
        oppEdgeA[0] * oppEdgeB[1] -
        oppEdgeA[1] * oppEdgeB[0];

      // Correct the number of faces
      nbFaces = 4;

    }

    // Loop on the frame's faces
    for (int iFace = nbFaces;
         iFace--;) {

      // Get the current face's normal
      const double* normal = normFaces[iFace];

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
        const double* frameCompC = frame->comp[2];
        FrameType frameType = frame->type;

        // Get the number of vertices of frame
        int nbVertices = (frameType == FrameTetrahedron ? 4 : 8);

        // Declare a variable to memorize if the current vertex is
        // the first in the loop, used to initialize the boundaries
        bool firstVertex = true;

        // Loop on vertices of the frame
        for (int iVertex = nbVertices;
             iVertex--;) {
          
          // Get the vertex
          double vertex[3];
          memcpy(vertex, frameOrig, 3 * sizeof(double));
          switch (iVertex) {
            case 7:
              vertex[0] += 
                frameCompA[0] +  frameCompB[0] + frameCompC[0];
              vertex[1] += 
                frameCompA[1] +  frameCompB[1] + frameCompC[1];
              vertex[2] += 
                frameCompA[2] +  frameCompB[2] + frameCompC[2];
              break;
            case 6:
              vertex[0] += frameCompB[0] + frameCompC[0];
              vertex[1] += frameCompB[1] + frameCompC[1];
              vertex[2] += frameCompB[2] + frameCompC[2];
              break;
            case 5:
              vertex[0] += frameCompA[0] + frameCompC[0];
              vertex[1] += frameCompA[1] + frameCompC[1];
              vertex[2] += frameCompA[2] + frameCompC[2];
              break;
            case 4:
              vertex[0] += frameCompA[0] + frameCompB[0];
              vertex[1] += frameCompA[1] + frameCompB[1];
              vertex[2] += frameCompA[2] + frameCompB[2];
              break;
            case 3:
              vertex[0] += frameCompC[0];
              vertex[1] += frameCompC[1];
              vertex[2] += frameCompC[2];
              break;
            case 2:
              vertex[0] += frameCompB[0];
              vertex[1] += frameCompB[1];
              vertex[2] += frameCompB[2];
              break;
            case 1:
              vertex[0] += frameCompA[0];
              vertex[1] += frameCompA[1];
              vertex[2] += frameCompA[2];
              break;
            default:
              break;
          }

          // Get the projection of the vertex on the face's normal
          double proj = 
            vertex[0] * normal[0] + 
            vertex[1] * normal[1] + 
            vertex[2] * normal[2];

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
#endif
