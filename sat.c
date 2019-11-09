#include "sat.h"

// ------------- Macros -------------

#define EPSILON 0.0000001

// ------------- Functions declaration -------------

#if FRAME_NB_DIM == 3
// Check the intersection constraint along one axis
bool CheckAxis(
  const Frame* const that,
  const Frame* const tho,
  const double* const axis);
#endif

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

  // Declare two variables to memorize the opposite edges in case
  // of tetrahedron
  double oppEdgesThat[3][3];
  double oppEdgesTho[3][3];

  // Declare a variable to memorize the number of edges, by default 3
  int nbEdgesThat = 3;
  int nbEdgesTho = 3;

  // If the first Frame is a tetrahedron
  if (that->type == FrameTetrahedron) {

    // Shortcuts
    const double* frameCompA = that->comp[0];
    const double* frameCompB = that->comp[1];
    const double* frameCompC = that->comp[2];

    // Initialise the opposite edges
    oppEdgesThat[0][0] = frameCompB[0] - frameCompA[0];
    oppEdgesThat[0][1] = frameCompB[1] - frameCompA[1];
    oppEdgesThat[0][2] = frameCompB[2] - frameCompA[2];

    oppEdgesThat[1][0] = frameCompB[0] - frameCompC[0];
    oppEdgesThat[1][1] = frameCompB[1] - frameCompC[1];
    oppEdgesThat[1][2] = frameCompB[2] - frameCompC[2];

    oppEdgesThat[2][0] = frameCompC[0] - frameCompA[0];
    oppEdgesThat[2][1] = frameCompC[1] - frameCompA[1];
    oppEdgesThat[2][2] = frameCompC[2] - frameCompA[2];

    // Correct the number of edges
    nbEdgesThat = 6;

  }

  // If the second Frame is a tetrahedron
  if (tho->type == FrameTetrahedron) {

    // Shortcuts
    const double* frameCompA = tho->comp[0];
    const double* frameCompB = tho->comp[1];
    const double* frameCompC = tho->comp[2];

    // Initialise the opposite edges
    oppEdgesTho[0][0] = frameCompB[0] - frameCompA[0];
    oppEdgesTho[0][1] = frameCompB[1] - frameCompA[1];
    oppEdgesTho[0][2] = frameCompB[2] - frameCompA[2];

    oppEdgesTho[1][0] = frameCompB[0] - frameCompC[0];
    oppEdgesTho[1][1] = frameCompB[1] - frameCompC[1];
    oppEdgesTho[1][2] = frameCompB[2] - frameCompC[2];

    oppEdgesTho[2][0] = frameCompC[0] - frameCompA[0];
    oppEdgesTho[2][1] = frameCompC[1] - frameCompA[1];
    oppEdgesTho[2][2] = frameCompC[2] - frameCompA[2];

    // Correct the number of edges
    nbEdgesTho = 6;

  }
  
  // Declare variables to loop on Frames and commonalize code
  const Frame* frame = that;
  const double (*oppEdgesA)[3] = oppEdgesThat;

  // Loop to commonalize code when checking SAT based on that's edges
  // and then tho's edges
  for (int iTest = 2;
       iTest--;) {

    // Shortcuts
    FrameType frameType = frame->type;
    const double* frameCompA = frame->comp[0];
    const double* frameCompB = frame->comp[1];
    const double* frameCompC = frame->comp[2];
    
    // Declare a variable to memorize the number of faces, by default 3
    int nbFaces = 3;

    // Declare a variable to memorize the normal to faces
    // Arrangement is normFaces[iFace][iAxis]
    double normFaces[4][3];

    // Initialise the normal to faces
    normFaces[0][0] = 
      frameCompA[1] * frameCompB[2] -
      frameCompA[2] * frameCompB[1];
    normFaces[0][1] = 
      frameCompA[2] * frameCompB[0] -
      frameCompA[0] * frameCompB[2];
    normFaces[0][2] = 
      frameCompA[0] * frameCompB[1] -
      frameCompA[1] * frameCompB[0];

    normFaces[1][0] = 
      frameCompA[1] * frameCompC[2] -
      frameCompA[2] * frameCompC[1];
    normFaces[1][1] = 
      frameCompA[2] * frameCompC[0] -
      frameCompA[0] * frameCompC[2];
    normFaces[1][2] = 
      frameCompA[0] * frameCompC[1] -
      frameCompA[1] * frameCompC[0];

    normFaces[2][0] = 
      frameCompC[1] * frameCompB[2] -
      frameCompC[2] * frameCompB[1];
    normFaces[2][1] = 
      frameCompC[2] * frameCompB[0] -
      frameCompC[0] * frameCompB[2];
    normFaces[2][2] = 
      frameCompC[0] * frameCompB[1] -
      frameCompC[1] * frameCompB[0];

    // If the frame is a tetrahedron
    if (frameType == FrameTetrahedron) {

      // Shortcuts
      const double* oppEdgeA = oppEdgesA[0];
      const double* oppEdgeB = oppEdgesA[1];

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

      // Check against the current face's normal
      bool isIntersection = 
        CheckAxis(
          that, 
          tho, 
          normFaces[iFace]);
      if (isIntersection == false) {

        return false;

      }

    }

    // Switch the frame to test against the second Frame
    frame = tho;
    oppEdgesA = oppEdgesTho;

  }

  // Loop on the pair of edges between the two frames
  for (int iEdgeThat = nbEdgesThat;
       iEdgeThat--;) {

    // Get the first edge
    const double* edgeThat = 
      (iEdgeThat < 3 ? 
        that->comp[iEdgeThat] : 
        oppEdgesThat[iEdgeThat - 3]);

    for (int iEdgeTho = nbEdgesTho;
         iEdgeTho--;) {

      // Get the second edge
      const double* edgeTho = 
        (iEdgeTho < 3 ? 
          tho->comp[iEdgeTho] : 
          oppEdgesTho[iEdgeTho - 3]);

      // Get the cross product of the two edges
      double axis[3];
      axis[0] = edgeThat[1] * edgeTho[2] - edgeThat[2] * edgeTho[1];
      axis[1] = edgeThat[2] * edgeTho[0] - edgeThat[0] * edgeTho[2];
      axis[2] = edgeThat[0] * edgeTho[1] - edgeThat[1] * edgeTho[0];
      
      // Check against the cross product of the two edges
      bool isIntersection = 
        CheckAxis(
          that, 
          tho, 
          axis);
      if (isIntersection == false) {

        return false;

      }
      
    }

  }

  return true;

}

// Check the intersection constraint along one axis
bool CheckAxis(
  const Frame* const that,
  const Frame* const tho,
  const double* const axis) {
  
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

      // Get the projection of the vertex on the axis
      double proj = 
        vertex[0] * axis[0] + 
        vertex[1] * axis[1] + 
        vertex[2] * axis[2];

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

  return true;
}

#endif
