#include "fmb.h"

// ------------- Macros -------------

// Return 1.0 if v is positive, -1.0 if v is negative, 0.0 else
#define sgn(v) (((0.0 < (v)) ? 1 : 0) - (((v) < 0.0) ? 1 : 0))

#define FST_VAR 0
#define SND_VAR 1
#define THD_VAR 2

#define EPSILON 0.0000001 //0.001

// ------------- Functions declaration -------------

// Eliminate the 'iVar'-th variable in the system 'M'.X<='Y'
// using the Fourier-Motzkin method and return
// the resulting system in 'Mp' and 'Yp', and the number of rows of 
// the resulting system in 'nbRemainRows'
// Return false if the system becomes inconsistent during elimination,
// else return true
bool ElimVar(
     const int iVar, 
  const double (*M)[FRAME_NB_DIM], 
  const double* Y, 
     const int nbRows, 
     const int nbCols, 
        double (*Mp)[FRAME_NB_DIM], 
        double* Yp, 
    int* const nbRemainRows);

// Get the bounds of the 'iVar'-th variable in the 'nbRows' rows
// system 'M'.X<='Y' and store them in the 'iVar'-th axis of the
// AABB 'bdgBox'
// ('M' arrangement is [iRow][iCol])
// The system is supposed to have been reduced to only one variable
// per row, the one in argument, which can be located in a different
// column than 'iVar'
// May return inconsistent values (max < min), which would
// mean the system has no solution
void GetBound(
     const int iVar,
  const double (*M)[FRAME_NB_DIM], 
  const double* Y, 
     const int nbRows, 
   AABB* const bdgBox);

// ------------- Functions implementation -------------

void PrintMY(
  const double (*M)[FRAME_NB_DIM], 
  const double* Y, 
     const int nbRows,
     const int nbVar) {
  for (int iRow = 0; iRow < nbRows; ++iRow) {
    for (int iCol = 0; iCol < nbVar; ++iCol) {
      printf("%f ", M[iRow][iCol]);
    }
    printf("| %f\n", Y[iRow]);
  }
}

void PrintM(
  const double (*M)[FRAME_NB_DIM], 
     const int nbRows) {
  for (int iRow = 0; iRow < nbRows; ++iRow) {
    for (int iCol = 0; iCol < FRAME_NB_DIM; ++iCol) {
      printf("%f ", M[iRow][iCol]);
    }
    printf("\n");
  }
}

// Eliminate the 'iVar'-th variable in the system 'M'.X<='Y'
// using the Fourier-Motzkin method and return
// the resulting system in 'Mp' and 'Yp', and the number of rows of 
// the resulting system in 'nbRemainRows'
// ('M' arrangement is [iRow][iCol])
// Return true if the system becomes inconsistent during elimination,
// else return false
bool ElimVar(
     const int iVar, 
  const double (*M)[FRAME_NB_DIM], 
  const double* Y, 
     const int nbRows, 
     const int nbCols, 
        double (*Mp)[FRAME_NB_DIM], 
        double* Yp, 
    int* const nbRemainRows) {

  // Initialize the number of rows in the result system
  *nbRemainRows = 0;
  
  // First we copy and compress the rows where the eliminated
  // variable is null
  
  // Loop on rows of the input system
  for (int iRow = 0; 
       iRow < nbRows; 
       ++iRow) {

    // Shortcut
    const double* MiRow = M[iRow];

    // If the coefficient of the eliminated variable is null on
    // this row
    if (fabs(M[iRow][iVar]) < EPSILON) {

      // Shortcut
      double* MpnbRemainRows = Mp[*nbRemainRows];

      // Copy this row into the result system excluding the eliminated
      // variable
      for (int iCol = 0, jCol = 0; 
           iCol < nbCols; 
           ++iCol) {

        if (iCol != iVar) {

          MpnbRemainRows[jCol] = MiRow[iCol];

          ++jCol;

        }

      }

      Yp[*nbRemainRows] = Y[iRow];

      // Increment the nb of rows into the result system
      ++(*nbRemainRows);

    }

  }

  // Then we process the rows where the eliminated variable is not null
  
  // For each row except the last one
  for (int iRow = 0; 
       iRow < nbRows - 1; 
       ++iRow) {

    // Shortcuts
    int sgnMIRowIVar = sgn(M[iRow][iVar]);
    double fabsMIRowIVar = fabs(M[iRow][iVar]);
    double YIRowDivideByFabsMIRowIVar = Y[iRow] / fabsMIRowIVar;

    // For each following rows
    for (int jRow = iRow + 1; 
         jRow < nbRows; 
         ++jRow) {

      // If coefficients of the eliminated variable in the two rows have
      // different signs and are not null
      if (sgnMIRowIVar != sgn(M[jRow][iVar]) && 
          fabsMIRowIVar > EPSILON && 
          fabs(M[jRow][iVar]) > EPSILON) {

        // Declare a variable to memorize the sum of the negative 
        // coefficients in the row
        double sumNegCoeff = 0.0;
        
        // Declare a variable to memorize if all the coefficients
        // are >= 0.0
        bool allPositive = true;

        // Declare a variable to memorize if all the coefficients
        // are null
        bool allNull = true;

        // Add the sum of the two normed (relative to the eliminated
        // variable) rows into the result system. This actually
        // eliminate the variable while keeping the constraints on
        // others variables
        for (int iCol = 0, jCol = 0; 
             iCol < nbCols; 
             ++iCol ) {

          if (iCol != iVar) {

            Mp[*nbRemainRows][jCol] = 
              M[iRow][iCol] / fabsMIRowIVar + 
              M[jRow][iCol] / fabs(M[jRow][iVar]);

            // If the coefficient is negative
            if (Mp[*nbRemainRows][jCol] < -1.0 * EPSILON) {

              // Memorize that at least one coefficient is not positive
              allPositive = false;

              // Memorize that at least one coefficient is not null
              allNull = false;

              // Update the sum of the negative coefficient
              sumNegCoeff += Mp[*nbRemainRows][jCol];

            // Else, if the coefficient is positive
            } else if (Mp[*nbRemainRows][jCol] > EPSILON) {

              // Memorize that at least one coefficient is not null
              allNull = false;

            }

            ++jCol;

          }

        }

        Yp[*nbRemainRows] = 
          YIRowDivideByFabsMIRowIVar +
          Y[jRow] / fabs(M[jRow][iVar]);

        // If at least one coefficient is not null
        if (allNull == false) {

          // If all the coefficients are positive and the right side of the 
          // inequality is negative
          if (allPositive == true && 
              Yp[*nbRemainRows] < 0.0) {

            // As X is in [0,1], the system is inconsistent
            return true;

          }

          // If the right side of the inequality if lower than the sum of 
          // negative coefficients in the row
          if (Yp[*nbRemainRows] < sumNegCoeff) {

            // As X is in [0,1], the system is inconsistent
            return true;

          }

        // Else all coefficients are null, if the right side is null
        // or negative
        } else if (Yp[*nbRemainRows] <= 0.0) {

          // The system is inconsistent
          return true;

        }

        // Increment the nb of rows into the result system
        ++(*nbRemainRows);

      }

    }

  }

  return false;

}

// Get the bounds of the 'iVar'-th variable in the 'nbRows' rows
// system 'M'.X<='Y' and store them in the 'iVar'-th axis of the
// AABB 'bdgBox'
// ('M' arrangement is [iRow][iCol])
// The system is supposed to have been reduced to only one variable
// per row, the one in argument
// May return inconsistent values (max < min), which would
// mean the system has no solution
void GetBound(
     const int iVar,
  const double (*M)[FRAME_NB_DIM], 
  const double* Y, 
     const int nbRows, 
   AABB* const bdgBox) {

  // Shortcuts
  double* min = bdgBox->min + iVar;
  double* max = bdgBox->max + iVar;

  // Initialize the bounds to there maximum maximum and minimum minimum
  *min = 0.0;
  *max = 1.0;

  // Loop on rows
  for (int jRow = 0; 
       jRow < nbRows; 
       ++jRow) {

    // Shortcut
    double MjRowiVar = M[jRow][0];

    // If this row has been reduced to the variable in argument
    // and it has a strictly positive coefficient
    if (MjRowiVar > EPSILON) {

      // Get the scaled value of Y for this row
      double y = Y[jRow] / MjRowiVar;

      // If the value is lower than the current maximum bound
      if (*max > y) {

        // Update the maximum bound
        *max = y;

      }

    // Else, if this row has been reduced to the variable in argument
    // and it has a strictly negative coefficient
    } else if (MjRowiVar < -1.0 * EPSILON) {

      // Get the scaled value of Y for this row
      double y = Y[jRow] / MjRowiVar;

      // If the value is greater than the current minimum bound
      if (*min < y) {

        // Update the minimum bound
        *min = y;

      }

    }

  }

}

// Test for intersection between Frame 'that' and Frame 'tho'
// Return true if the two Frames are intersecting, else false
// If the Frame are intersecting the AABB of the intersection
// is stored into 'bdgBox', else 'bdgBox' is not modified
// If 'bdgBox' is null, the result AABB is not memorized (to use if
// unnecessary and want to speed up the algorithm)
// The resulting AABB may be larger than the smallest possible AABB
// The resulting AABB of FMBTestIntersection(A,B) may be different
// of the resulting AABB of FMBTestIntersection(B,A)
bool FMBTestIntersection(
  const Frame* const that, 
  const Frame* const tho, 
         AABB* const bdgBox) {

  // Get the projection of the Frame 'tho' in Frame 'that' coordinates
  // system
  Frame thoProj;
  FrameImportFrame(that, tho, &thoProj);

//printf("thoProj:\n");
//FramePrint(&thoProj);
//printf("\n--\n");

  // Declare two variables to memorize the system to be solved M.X <= Y
  // (M arrangement is [iRow][iCol])
  double M[4 * FRAME_NB_DIM + 2][FRAME_NB_DIM];
  double Y[4 * FRAME_NB_DIM + 2];

  // Shortcuts
  double (*thoProjComp)[FRAME_NB_DIM] = thoProj.comp;
  double *thoProjOrig = thoProj.orig;

  // For the first 2 * FRAME_NB_DIM rows of the system
  for (int iStepRow = FRAME_NB_DIM;
       iStepRow--;) {

    // Shortcuts
    int iRow = 2 * iStepRow;
    int iNextRow = iRow + 1;
    double* MIRow = M[iRow];
    double* MINextRow = M[iNextRow];
    int jRow = 2 * (iStepRow + FRAME_NB_DIM);
    int jNextRow = jRow + 1;
    double* MJRow = M[jRow];
    double* MJNextRow = M[jNextRow];

    // Declare a variable to memorize the sum of the negative 
    // coefficients in the row
    double sumNegCoeff[2] = {0.0, 0.0};
    
    // Declare a variable to memorize if all the coefficients
    // are >= 0.0
    bool allPositive[2] = {true, true};

    // Declare a variable to memorize if all the coefficients
    // are null
    bool allNull = true;

    // For each column of the system
    for (int iCol = FRAME_NB_DIM;
         iCol--;) {

      MIRow[iCol] = thoProjComp[iCol][iStepRow];
      MINextRow[iCol] = -1.0 * MIRow[iCol];

      // If it's on the diagonal 
      if (iStepRow == iCol) {

        MJRow[iCol] = 1.0;
        MJNextRow[iCol] = -1.0;

      // Else it's not on the diagonal
      } else {

        MJRow[iCol] = 0.0;
        MJNextRow[iCol] = 0.0;

      }

      // If the coefficient is negative
      if (MIRow[iCol] < -1.0 * EPSILON) {

        // Memorize that at least one coefficient is not positive
        allPositive[0] = false;

        // Memorize that at least one coefficient is not null
        allNull = false;

        // Update the sum of the negative coefficient
        sumNegCoeff[0] += MIRow[iCol];

      // Else, if the coefficient is positive
      } else if (MIRow[iCol] > EPSILON) {

        // Memorize that at least one coefficient is not null
        allNull = false;

        // Memorize that at least one coefficient is not positive
        allPositive[1] = false;

        // Update the sum of the negative coefficient
        sumNegCoeff[1] -= MIRow[iCol];

      }

    }

    Y[iRow] = 1.0 - thoProjOrig[iStepRow];
    Y[iNextRow] = thoProjOrig[iStepRow];

    // If at least one coefficient is not null
    if (allNull == false) {

      // If all the coefficients are positive and the right side of the 
      // inequality is negative
      if ((allPositive[0] == true && 
           Y[iRow] < 0.0) ||
          (allPositive[1] == true && 
           Y[iNextRow] < 0.0)) {

        // As X is in [0,1], the system is inconsistent,
        // there is no intersection
        return false;

      }

      // If the right side of the inequality is lower than the sum of 
      // negative coefficients in the row
      if (Y[iRow] < sumNegCoeff[0] ||
          Y[iNextRow] < sumNegCoeff[1]) {

        // As X is in [0,1], the system is inconsistent
        // there is no intersection
        return false;

      }

    // Else all coefficients are null, if the right side is null
    // or negative
    } else if (Y[iRow] <= 0.0 ||
               Y[iNextRow] <= 0.0) {

      // The system is inconsistent, there is no intersection
      return false;

    }

    Y[jRow] = 1.0;
    Y[jNextRow] = 0.0;

  }

  // Declare a variable to memorize the total number of rows in the
  // system. It may vary depending on the type of Frames
  int nbRows = 4 * FRAME_NB_DIM;

  // If the first frame is a Tetrahedron
  if (that->type == FrameTetrahedron) {

    // Declare a variable to memorize the sum of the negative 
    // coefficients in the row
    double sumNegCoeff = 0.0;
    
    // Declare a variable to memorize if all the coefficients
    // are >= 0.0
    bool allPositive = true;

    // Declare a variable to memorize if all the coefficients
    // are null
    bool allNull = true;

    // Shortcut
    double* MRow = M[nbRows];

    Y[nbRows] = 1.0;

    // For each column of the system
    for (int iCol = FRAME_NB_DIM;
         iCol--;) {

      MRow[iCol] = 0.0;

      // For each component
      for (int iAxis = FRAME_NB_DIM;
           iAxis--;) {

        MRow[iCol] += thoProjComp[iCol][iAxis];

      }

      Y[nbRows] -= thoProjOrig[iCol];

      // If the coefficient is negative
      if (MRow[iCol] < -1.0 * EPSILON) {

        // Memorize that at least one coefficient is not positive
        allPositive = false;

        // Memorize that at least one coefficient is not null
        allNull = false;

        // Update the sum of the negative coefficient
        sumNegCoeff += MRow[iCol];

      // Else, if the coefficient is positive
      } else if (MRow[iCol] > EPSILON) {

        // Memorize that at least one coefficient is not null
        allNull = false;

      }

    }

    // If at least one coefficient is not null
    if (allNull == false) {

      // If all the coefficients are positive and the right side of the 
      // inequality is negative
      if (allPositive == true && 
           Y[nbRows] < 0.0) {

        // As X is in [0,1], the system is inconsistent,
        // there is no intersection
        return false;

      }

      // If the right side of the inequality is lower than the sum of 
      // negative coefficients in the row
      if (Y[nbRows] < sumNegCoeff) {

        // As X is in [0,1], the system is inconsistent
        // there is no intersection
        return false;

      }

    // Else all coefficients are null, if the right side is null
    // or negative
    } else if (Y[nbRows] <= 0.0) {

      // The system is inconsistent, there is no intersection
      return false;

    }

    // Update the number of rows in the system
    ++nbRows;

  }

  // If the second frame is a Tetrahedron
  if (tho->type == FrameTetrahedron) {

    // Declare a variable to memorize the sum of the negative 
    // coefficients in the row
    double sumNegCoeff = 0.0;
    
    // Declare a variable to memorize if all the coefficients
    // are >= 0.0
    bool allPositive = true;

    // Declare a variable to memorize if all the coefficients
    // are null
    bool allNull = true;

    // Shortcut
    double* MRow = M[nbRows];

    // For each column of the system
    for (int iCol = FRAME_NB_DIM;
         iCol--;) {

      MRow[iCol] =  1.0;

      // If the coefficient is negative
      if (MRow[iCol] < -1.0 * EPSILON) {

        // Memorize that at least one coefficient is not positive
        allPositive = false;

        // Memorize that at least one coefficient is not null
        allNull = false;

        // Update the sum of the negative coefficient
        sumNegCoeff += MRow[iCol];

      // Else, if the coefficient is positive
      } else if (MRow[iCol] > EPSILON) {

        // Memorize that at least one coefficient is not null
        allNull = false;

      }

    }

    Y[nbRows] = 1.0;

    // If at least one coefficient is not null
    if (allNull == false) {

      // If all the coefficients are positive and the right side of the 
      // inequality is negative
      if (allPositive == true && 
           Y[nbRows] < 0.0) {

        // As X is in [0,1], the system is inconsistent,
        // there is no intersection
        return false;

      }

      // If the right side of the inequality is lower than the sum of 
      // negative coefficients in the row
      if (Y[nbRows] < sumNegCoeff) {

        // As X is in [0,1], the system is inconsistent
        // there is no intersection
        return false;

      }

    // Else all coefficients are null, if the right side is null
    // or negative
    } else if (Y[nbRows] <= 0.0) {

      // The system is inconsistent, there is no intersection
      return false;

    }

    // Update the number of rows in the system
    ++nbRows;

  }

  // Solve the system

  // Declare a AABB to memorize the bounding box of the intersection
  // in the coordinates system of that
  AABB bdgBoxLocal;
  
  // Declare variables to eliminate the first variable
  // The number of rows is set conservatively, one may try to reduce
  // them if needed  
  double Mp[4 * FRAME_NB_DIM + 2 + 50][FRAME_NB_DIM];
  double Yp[4 * FRAME_NB_DIM + 2 + 50];
  int nbRowsP;

  // Eliminate the first variable
  bool inconsistency = 
    ElimVar(
      FST_VAR,
      M, 
      Y, 
      nbRows, 
      FRAME_NB_DIM,
      Mp, 
      Yp, 
      &nbRowsP);

  // If the system is inconsistent
  if (inconsistency == true) {

    // The two Frames are not in intersection
    return false;

  }

  #if FRAME_NB_DIM == 3

    // Declare variables to eliminate the second variable
    // The number of rows is set conservatively, one may try to reduce
    // them if needed  
    double Mpp[4 * FRAME_NB_DIM + 2 + 500][FRAME_NB_DIM];
    double Ypp[4 * FRAME_NB_DIM + 2 + 500];
    int nbRowsPP;

    // Eliminate the second variable (which is the first in the new system)
    inconsistency = 
      ElimVar(
        FST_VAR,
        Mp, 
        Yp, 
        nbRowsP, 
        FRAME_NB_DIM - 1,
        Mpp, 
        Ypp, 
        &nbRowsPP);

    // If the system is inconsistent
    if (inconsistency == true) {

      // The two Frames are not in intersection
      return false;

    }

    // Get the bounds for the remaining third variable
    GetBound(
      THD_VAR,
      Mpp,
      Ypp,
      nbRowsPP,
      &bdgBoxLocal);

    // If the bounds are inconstent
    if (bdgBoxLocal.min[THD_VAR] >= bdgBoxLocal.max[THD_VAR]) {

      // The two Frames are not in intersection
      return false;

    }

    // Eliminate the third variable (which is second first in the new
    // system)
    inconsistency = 
      ElimVar(
        SND_VAR,
        Mp, 
        Yp, 
        nbRowsP, 
        FRAME_NB_DIM - 1,
        Mpp, 
        Ypp, 
        &nbRowsPP);

    // If the resulting system is inconsistent
    if (inconsistency == true) {

      // The two Frames are not in intersection
      return false;

    // Else, if the bounds are consistent here it means
    // the two Frames are in intersection.
    // If the user hasn't requested for the resulting bounding box
    } else if (bdgBox == NULL) {

      // Immediately return true
      return true;

    }

    // Get the bounds for the remaining second variable
    GetBound(
      SND_VAR,
      Mpp,
      Ypp,
      nbRowsPP,
      &bdgBoxLocal);

    // If the bounds are inconstent
    if (bdgBoxLocal.min[SND_VAR] >= bdgBoxLocal.max[SND_VAR]) {

      // The two Frames are not in intersection
      return false;

    }

    // Now starts again from the initial systems and eliminate the 
    // second and third variables to get the bounds of the first variable
    inconsistency = 
      ElimVar(
        SND_VAR,
        M, 
        Y, 
        nbRows, 
        FRAME_NB_DIM,
        Mp, 
        Yp, 
        &nbRowsP);

    if (inconsistency == true) {
      return false;
    }

    inconsistency = 
      ElimVar(
        SND_VAR,
        Mp, 
        Yp, 
        nbRowsP, 
        FRAME_NB_DIM - 1,
        Mpp, 
        Ypp, 
        &nbRowsPP);

    if (inconsistency == true) {
      return false;
    }

    GetBound(
      FST_VAR,
      Mpp,
      Ypp,
      nbRowsPP,
      &bdgBoxLocal);

    if (bdgBoxLocal.min[FST_VAR] >= bdgBoxLocal.max[FST_VAR]) {
      return false;
    }

  #elif FRAME_NB_DIM == 2

//PrintMY(M, Y, nbRows, 2);
//printf("--\n");
//PrintMY(Mp, Yp, nbRowsP, 1);

    // Get the bounds for the remaining second variable
    GetBound(
      SND_VAR,
      Mp,
      Yp,
      nbRowsP,
      &bdgBoxLocal);

    // If the bounds are inconstent
    if (bdgBoxLocal.min[SND_VAR] >= bdgBoxLocal.max[SND_VAR]) {

      // The two Frames are not in intersection
      return false;

    // Else, if the bounds are consistent here it means
    // the two Frames are in intersection.
    // If the user hasn't requested for the resulting bounding box
    } else if (bdgBox == NULL) {

      // Immediately return true
      return true;

    }

    // Now starts again from the initial systems and eliminate the 
    // second variable to get the bounds of the first variable
    inconsistency = 
      ElimVar(
        SND_VAR,
        M, 
        Y, 
        nbRows, 
        FRAME_NB_DIM,
        Mp, 
        Yp, 
        &nbRowsP);
    if (inconsistency == true) {
      return false;
    }

//printf("--\n");
//PrintMY(Mp, Yp, nbRowsP, 1);

    GetBound(
      FST_VAR,
      Mp,
      Yp,
      nbRowsP,
      &bdgBoxLocal);

//printf("--\nbdgBoxLocal:\n");
//AABBPrint(&bdgBoxLocal);printf("\n");

    if (bdgBoxLocal.min[FST_VAR] >= bdgBoxLocal.max[FST_VAR]) {
      return false;
    }

  #else

    printf("Not implemented for dimension %d\n", FRAME_NB_DIM);
    exit(0);

  #endif
  
  // If the user requested the resulting bounding box
  if (bdgBox != NULL) {

    // Export the local bounding box toward the real coordinates
    // system
    FrameExportBdgBox(tho, &bdgBoxLocal, bdgBox);

//printf("bdgBox:\n");AABBPrint(bdgBox);printf("\n");
//printf("that->bdgBox:\n");AABBPrint(&(that->bdgBox));printf("\n");

    // Clip with the AABB of 'that'
    double* const min = bdgBox->min;
    double* const max = bdgBox->max;
    const double* const thatBdgBoxMin = that->bdgBox.min;
    const double* const thatBdgBoxMax = that->bdgBox.max;
    for (int iAxis = FRAME_NB_DIM; 
         iAxis--;) {

      if (min[iAxis] < thatBdgBoxMin[iAxis]) {

        min[iAxis] = thatBdgBoxMin[iAxis];

      }
      if (max[iAxis] > thatBdgBoxMax[iAxis]) {

        max[iAxis] = thatBdgBoxMax[iAxis];

      }

    }
    
  }

  // If we've reached here the two Frames are intersecting
  return true;

}
