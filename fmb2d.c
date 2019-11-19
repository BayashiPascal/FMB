#include "fmb2d.h"

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
bool ElimVar2D(
     const int iVar, 
  const double (*M)[2], 
  const double* Y, 
     const int nbRows, 
     const int nbCols, 
        double (*Mp)[2], 
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
void GetBound2D(
     const int iVar,
  const double (*M)[2], 
  const double* Y, 
     const int nbRows, 
   AABB2D* const bdgBox);

// ------------- Functions implementation -------------

void PrintMY2D(
  const double (*M)[2], 
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

void PrintM2D(
  const double (*M)[2], 
     const int nbRows) {
  for (int iRow = 0; iRow < nbRows; ++iRow) {
    for (int iCol = 0; iCol < 2; ++iCol) {
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
bool ElimVar2D(
     const int iVar, 
  const double (*M)[2], 
  const double* Y, 
     const int nbRows, 
     const int nbCols, 
        double (*Mp)[2], 
        double* Yp, 
    int* const nbRemainRows) {

  // Initialize the number of rows in the result system
  *nbRemainRows = 0;
  
  // First we process the rows where the eliminated variable is not null
  
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

          // If all the coefficients are positive and the right side of
          // the inequality is negative
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

  // Then we copy and compress the rows where the eliminated
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
void GetBound2D(
     const int iVar,
  const double (*M)[2], 
  const double* Y, 
     const int nbRows, 
   AABB2D* const bdgBox) {

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
bool FMBTestIntersection2D(
  const Frame2D* const that, 
  const Frame2D* const tho, 
         AABB2D* const bdgBox) {

  // Get the projection of the Frame 'tho' in Frame 'that' coordinates
  // system
  Frame2D thoProj;
  Frame2DImportFrame(that, tho, &thoProj);

  // Declare two variables to memorize the system to be solved M.X <= Y
  // (M arrangement is [iRow][iCol])
  double M[10][2];
  double Y[10];

  // Shortcuts
  double (*thoProjComp)[2] = thoProj.comp;
  double *thoProjOrig = thoProj.orig;

  // Variable to memorise the current row in the system
  int iRow = 0;

  // Shortcuts
  double* MIRow;

  // Constraints O + C.X >= 0.0
  // and constraints x_i >= 0.0
  for (;
       iRow < 2;
       ++iRow) {

    // Shortcuts
    MIRow = M[iRow];
    double* MJRow = M[iRow + 2];

    // For each column of the system
    double sumNeg = 0.0;
    for (int iCol = 2;
         iCol--;) {

      MIRow[iCol] = -1.0 * thoProjComp[iCol][iRow];
      if (MIRow[iCol] < 0.0) {
        sumNeg += MIRow[iCol];
      }
      
      // If it's on the diagonal 
      if (iRow == iCol) {

        MJRow[iCol] = -1.0;

      // Else it's not on the diagonal
      } else {

        MJRow[iCol] = 0.0;

      }
    }
    if (thoProjOrig[iRow] < sumNeg)
      return false;
    Y[iRow] = thoProjOrig[iRow];
    Y[iRow + 2] = 0.0;
  }
  iRow = 4;

  if (that->type == FrameCuboid) {

    // Constraints O + C.X <= 1.0
    for (int jRow = 0;
         jRow < 2;
         ++jRow, ++iRow) {

      // Shortcuts
      MIRow = M[iRow];
      double* MJRow = M[jRow];

      // For each column of the system
      double sumNeg = 0.0;
      for (int iCol = 2;
           iCol--;) {

        MIRow[iCol] = -1.0 * MJRow[iCol];
        if (MIRow[iCol] < 0.0) {
          sumNeg += MIRow[iCol];
        }
        
      }
      Y[iRow] = 1.0 - thoProjOrig[jRow];
      if (Y[iRow] < sumNeg)
        return false;
    }

  // Else, the first frame is a Tetrahedron
  } else {

    // Declare a variable to memorize the sum of the negative 
    // coefficients in the row
    double sumNegCoeff = 0.0;
    
    // Declare a variable to memorize if all the coefficients
    // are null
    bool allNull = true;

    // Shortcut
    double* MRow = M[iRow];

    Y[iRow] = 1.0;

    // For each column of the system
    for (int iCol = 2;
         iCol--;) {

      MRow[iCol] = 0.0;

      // For each component
      for (int iAxis = 2;
           iAxis--;) {

        MRow[iCol] += thoProjComp[iCol][iAxis];

      }

      Y[iRow] -= thoProjOrig[iCol];

      // If the coefficient is negative
      if (MRow[iCol] < -1.0 * EPSILON) {

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

      // If the right side of the inequality is lower than the sum of 
      // negative coefficients in the row
      if (Y[iRow] < sumNegCoeff) {

        // As X is in [0,1], the system is inconsistent
        // there is no intersection
        return false;

      }

    // Else all coefficients are null, if the right side is null
    // or negative
    } else if (Y[iRow] <= 0.0) {

      // The system is inconsistent, there is no intersection
      return false;

    }

    // Update the number of rows in the system
    ++iRow;

  }

  if (tho->type == FrameCuboid) {

    // Constraints x_i <= 1.0
    for (int jRow = 0;
         jRow < 2;
         ++jRow, ++iRow) {

      // Shortcuts
      MIRow = M[iRow];

      // For each column of the system
      for (int iCol = 2;
           iCol--;) {
        // If it's on the diagonal 
        if (jRow == iCol) {

          MIRow[iCol] = 1.0;

        // Else it's not on the diagonal
        } else {

          MIRow[iCol] = 0.0;

        }

      }
      Y[iRow] = 1.0;

    }

  // Else, the second frame is a Tetrahedron
  } else {

    // Declare a variable to memorize the sum of the negative 
    // coefficients in the row
    double sumNegCoeff = 0.0;
    
    // Declare a variable to memorize if all the coefficients
    // are null
    bool allNull = true;

    // Shortcut
    double* MRow = M[iRow];

    // For each column of the system
    for (int iCol = 2;
         iCol--;) {

      MRow[iCol] =  1.0;

      // If the coefficient is negative
      if (MRow[iCol] < -1.0 * EPSILON) {

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

    Y[iRow] = 1.0;

    // If at least one coefficient is not null
    if (allNull == false) {

      // If the right side of the inequality is lower than the sum of 
      // negative coefficients in the row
      if (Y[iRow] < sumNegCoeff) {

        // As X is in [0,1], the system is inconsistent
        // there is no intersection
        return false;

      }

    // Else all coefficients are null, if the right side is null
    // or negative
    } else if (Y[iRow] <= 0.0) {

      // The system is inconsistent, there is no intersection
      return false;

    }

    // Update the number of rows in the system
    ++iRow;

  }

  // Declare a variable to memorize the total number of rows in the
  // system. It may vary depending on the type of Frames
  int nbRows = iRow;

  // Solve the system

  // Declare a AABB to memorize the bounding box of the intersection
  // in the coordinates system of that
  AABB2D bdgBoxLocal;
  
  // Declare variables to eliminate the first variable
  // The number of rows is set conservatively, one may try to reduce
  // them if needed  
  double Mp[60][2];
  double Yp[60];
  int nbRowsP;

  // Eliminate the first variable
  bool inconsistency = 
    ElimVar2D(
      FST_VAR,
      M, 
      Y, 
      nbRows, 
      2,
      Mp, 
      Yp, 
      &nbRowsP);

  // If the system is inconsistent
  if (inconsistency == true) {

    // The two Frames are not in intersection
    return false;

  }

  // Get the bounds for the remaining second variable
  GetBound2D(
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
    ElimVar2D(
      SND_VAR,
      M, 
      Y, 
      nbRows, 
      2,
      Mp, 
      Yp, 
      &nbRowsP);
  if (inconsistency == true) {
    return false;
  }

  GetBound2D(
    FST_VAR,
    Mp,
    Yp,
    nbRowsP,
    &bdgBoxLocal);

  if (bdgBoxLocal.min[FST_VAR] >= bdgBoxLocal.max[FST_VAR]) {
    return false;
  }

  // If the user requested the resulting bounding box
  if (bdgBox != NULL) {

    // Export the local bounding box toward the real coordinates
    // system
    Frame2DExportBdgBox(
      tho, 
      &bdgBoxLocal, 
      bdgBox);

    // Clip with the AABB of 'that'
    double* const min = bdgBox->min;
    double* const max = bdgBox->max;
    const double* const thatBdgBoxMin = that->bdgBox.min;
    const double* const thatBdgBoxMax = that->bdgBox.max;
    for (int iAxis = 2; 
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

