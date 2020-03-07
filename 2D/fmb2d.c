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

#include "fmb2d.h"

// ------------- Macros -------------

// Return 1.0 if v is positive, -1.0 if v is negative, 0.0 else
#define sgn(v) (((0.0 < (v)) ? 1 : 0) - (((v) < 0.0) ? 1 : 0))

// Return x if x is negative, 0.0 else
#define neg(x) (x < 0.0 ? x : 0.0)

#define FST_VAR 0
#define SND_VAR 1

#define EPSILON 0.0000001

// ------------- Functions declaration -------------

// Eliminate the first variable in the system M.X<=Y
// using the Fourier-Motzkin method and return
// the resulting system in Mp and Yp, and the number of rows of
// the resulting system in nbRemainRows
// Return false if the system becomes inconsistent during elimination,
// else return true
bool ElimVar2D(
  const double (*M)[2],
  const double* Y,
  const int nbRows,
  const int nbCols,
  double (*Mp)[2],
  double* Yp,
  int* const nbRemainRows);

// Get the bounds of the iVar-th variable in the nbRows rows
// system M.X<=Y which has been reduced to only one variable
// and store them in the iVar-th axis of the
// AABB bdgBox
// (M arrangement is [iRow][iCol])
// May return inconsistent values (max < min), which would
// mean the system has no solution
void GetBoundLastVar2D(
  const int iVar,
  const double (*M)[2],
  const double* Y,
  const int nbRows,
  AABB2D* const bdgBox);

// Get the bounds of the iVar-th variable in the nbRows rows
// system M.X<=Y where the iVar-th variable is on the first column
// and store them in the iVar-th axis of the AABB bdgBox
// (M arrangement is [iRow][iCol])
void GetBoundVar2D(
  const int iVar,
  const double (*M)[2],
  const double* Y,
  const int nbRows,
  const int nbCols,
  AABB2D* const bdgBox);

// ------------- Functions implementation -------------

// Eliminate the first variable in the system M.X<=Y
// using the Fourier-Motzkin method and return
// the resulting system in Mp and Yp, and the number of rows of
// the resulting system in nbRemainRows
// (M arrangement is [iRow][iCol])
// Return true if the system becomes inconsistent during elimination,
// else return false
bool ElimVar2D(
  const double (*M)[2],
  const double* Y,
  const int nbRows,
  const int nbCols,
  double (*Mp)[2],
  double* Yp,
  int* const nbRemainRows) {

  // Initialize the number of rows in the result system
  int nbResRows = 0;

  // First we process the rows where the eliminated variable is not null
  // For each row except the last one
  for (
    int iRow = 0;
    iRow < nbRows - 1;
    ++iRow) {

    // Shortcuts
    const double fabsMIRowIVar = fabs(M[iRow][0]);

    // If the coefficient for the eliminated variable is not null
    // in this row
    if (fabsMIRowIVar > EPSILON) {

      // Shortcuts
      const double* MiRow = M[iRow];
      const int sgnMIRowIVar = sgn(MiRow[0]);
      const double YIRowDivideByFabsMIRowIVar = Y[iRow] / fabsMIRowIVar;

      // For each following rows
      for (
        int jRow = iRow + 1;
        jRow < nbRows;
        ++jRow) {

        // If coefficients of the eliminated variable in the two rows have
        // different signs and are not null
        if (
          sgnMIRowIVar != sgn(M[jRow][0]) &&
          fabs(M[jRow][0]) > EPSILON) {

          // Shortcuts
          const double* MjRow = M[jRow];
          const double fabsMjRow = fabs(MjRow[0]);

          // Declare a variable to memorize the sum of the negative
          // coefficients in the row
          double sumNegCoeff = 0.0;

          // Add the sum of the two normed (relative to the eliminated
          // variable) rows into the result system. This actually
          // eliminate the variable while keeping the constraints on
          // others variables
          for (
            int iCol = 1;
            iCol < nbCols;
            ++iCol ) {

            Mp[nbResRows][iCol - 1] =
              MiRow[iCol] / fabsMIRowIVar +
              MjRow[iCol] / fabsMjRow;

            // Update the sum of the negative coefficient
            sumNegCoeff += neg(Mp[nbResRows][iCol - 1]);

          }

          // Update the right side of the inequality
          Yp[nbResRows] =
            YIRowDivideByFabsMIRowIVar +
            Y[jRow] / fabsMjRow;

          // If the right side of the inequality is lower than the sum of
          // negative coefficients in the row
          // (Add epsilon for numerical imprecision)
          if (Yp[nbResRows] < sumNegCoeff - EPSILON) {

            // Given that X is in [0,1], the system is inconsistent
            return true;

          }

          // Increment the nb of rows into the result system
          ++nbResRows;

        }

      }

    }

  }

  // Then we copy and compress the rows where the eliminated
  // variable is null
  // Loop on rows of the input system
  for (
    int iRow = 0;
    iRow < nbRows;
    ++iRow) {

    // Shortcut
    const double* MiRow = M[iRow];

    // If the coefficient of the eliminated variable is null on
    // this row
    if (fabs(MiRow[0]) < EPSILON) {

      // Shortcut
      double* MpnbResRows = Mp[nbResRows];

      // Copy this row into the result system excluding the eliminated
      // variable
      for (
        int iCol = 1;
        iCol < nbCols;
        ++iCol) {

        MpnbResRows[iCol - 1] = MiRow[iCol];

      }

      Yp[nbResRows] = Y[iRow];

      // Increment the nb of rows into the result system
      ++nbResRows;

    }

  }

  // Memorize the number of rows in the result system
  *nbRemainRows = nbResRows;

  // If we reach here the system is not inconsistent
  return false;

}

// Get the bounds of the iVar-th variable in the nbRows rows
// system M.X<=Y which has been reduced to only one variable
// and store them in the iVar-th axis of the
// AABB bdgBox
// (M arrangement is [iRow][iCol])
// May return inconsistent values (max < min), which would
// mean the system has no solution
void GetBoundLastVar2D(
  const int iVar,
  const double (*M)[2],
  const double* Y,
  const int nbRows,
  AABB2D* const bdgBox) {

  // Shortcuts
  double* min = bdgBox->min + iVar;
  double* max = bdgBox->max + iVar;

  // Initialize the bounds to their maximum maximum and minimum minimum
  *min = 0.0;
  *max = 1.0;

  // Loop on rows
  for (
    int jRow = 0;
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
    } else if (MjRowiVar < -EPSILON) {

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

// Get the bounds of the iVar-th variable in the nbRows rows
// system M.X<=Y where the iVar-th variable is on the first column
// and store them in the iVar-th axis of the AABB bdgBox
// (M arrangement is [iRow][iCol])
void GetBoundVar2D(
  const int iVar,
  const double (*M)[2],
  const double* Y,
  const int nbRows,
  const int nbCols,
  AABB2D* const bdgBox) {

  // Shortcuts
  double* bdgBoxMin = bdgBox->min;
  double* bdgBoxMax = bdgBox->max;

  // Initialize the bounds
  bdgBoxMin[iVar] = 0.0;
  bdgBoxMax[iVar] = 1.0;

  // Loop on the rows
  for (
    int iRow = 0;
    iRow < nbRows;
    ++iRow) {

    // Shortcuts
    const double* MIRow = M[iRow];
    double fabsMIRowIVar = fabs(MIRow[0]);

    // If the coefficient of the first variable on this row is not null
    if (fabsMIRowIVar > EPSILON) {

      // Declare two variables to memorize the min and max of the
      // requested variable in this row
      double min = -1.0 * Y[iRow];
      double max = Y[iRow];

      // Loop on columns except the first one which is the one of the
      // requested variable
      for (
        int iCol = 1;
        iCol < nbCols;
        ++iCol) {

        if (MIRow[iCol] > EPSILON) {

          min += MIRow[iCol] * bdgBoxMin[iCol + iVar];
          max -= MIRow[iCol] * bdgBoxMin[iCol + iVar];

        } else if (MIRow[iCol] < EPSILON) {

          min += MIRow[iCol] * bdgBoxMax[iCol + iVar];
          max -= MIRow[iCol] * bdgBoxMax[iCol + iVar];

        }

      }

      min /= -1.0 * MIRow[0];
      max /= MIRow[0];
      if (bdgBoxMin[iVar] > min) {

        bdgBoxMin[iVar] = min;

      }

      if (bdgBoxMax[iVar] < max) {

        bdgBoxMax[iVar] = max;

      }

    }

  }

}

// Test for intersection between Frame that and Frame tho
// Return true if the two Frames are intersecting, else false
// If the Frame are intersecting the AABB of the intersection
// is stored into bdgBox, else bdgBox is not modified
// If bdgBox is null, the result AABB is not memorized (to use if
// unnecessary and want to speed up the algorithm)
// The resulting AABB may be larger than the smallest possible AABB
// The resulting AABB of FMBTestIntersection(A,B) may be different
// of the resulting AABB of FMBTestIntersection(B,A)
// The resulting AABB is given in tho's local coordinates system
bool FMBTestIntersection2D(
  const Frame2D* const that,
  const Frame2D* const tho,
  AABB2D* const bdgBox) {

  // Get the projection of the Frame tho in Frame that coordinates
  // system
  Frame2D thoProj;
  Frame2DImportFrame(that, tho, &thoProj);

  // Declare two variables to memorize the system to be solved M.X <= Y
  // (M arrangement is [iRow][iCol])
  double M[8][2];
  double Y[8];

  // Create the inequality system
  // -sum_iC_j,iX_i<=O_j
  M[0][0] = -thoProj.comp[0][0];
  M[0][1] = -thoProj.comp[1][0];
  Y[0] = thoProj.orig[0];
  if (Y[0] < neg(M[0][0]) + neg(M[0][1])) {

    return false;

  }

  M[1][0] = -thoProj.comp[0][1];
  M[1][1] = -thoProj.comp[1][1];
  Y[1] = thoProj.orig[1];
  if (Y[1] < neg(M[1][0]) + neg(M[1][1])) {

    return false;

  }

  // Variable to memorize the nb of rows in the system
  int nbRows = 2;

  if (that->type == FrameCuboid) {

    // sum_iC_j,iX_i<=1.0-O_j
    M[nbRows][0] = thoProj.comp[0][0];
    M[nbRows][1] = thoProj.comp[1][0];
    Y[nbRows] = 1.0 - thoProj.orig[0];
    if (Y[nbRows] < neg(M[nbRows][0]) + neg(M[nbRows][1])) {

      return false;

    }

    ++nbRows;

    M[nbRows][0] = thoProj.comp[0][1];
    M[nbRows][1] = thoProj.comp[1][1];
    Y[nbRows] = 1.0 - thoProj.orig[1];
    if (Y[nbRows] < neg(M[nbRows][0]) + neg(M[nbRows][1])) {

      return false;

    }

    ++nbRows;

  } else {

    // sum_j(sum_iC_j,iX_i)<=1.0-sum_iO_i
    M[nbRows][0] = thoProj.comp[0][0] + thoProj.comp[0][1];
    M[nbRows][1] = thoProj.comp[1][0] + thoProj.comp[1][1];
    Y[nbRows] = 1.0 - thoProj.orig[0] - thoProj.orig[1];
    if (Y[nbRows] < neg(M[nbRows][0]) + neg(M[nbRows][1])) {

      return false;

    }

    ++nbRows;

  }

  if (tho->type == FrameCuboid) {

    // X_i <= 1.0
    M[nbRows][0] = 1.0;
    M[nbRows][1] = 0.0;
    Y[nbRows] = 1.0;
    ++nbRows;

    M[nbRows][0] = 0.0;
    M[nbRows][1] = 1.0;
    Y[nbRows] = 1.0;
    ++nbRows;

  } else {

    // sum_iX_i<=1.0
    M[nbRows][0] = 1.0;
    M[nbRows][1] = 1.0;
    Y[nbRows] = 1.0;
    ++nbRows;

  }

  // -X_i <= 0.0
  M[nbRows][0] = -1.0;
  M[nbRows][1] = 0.0;
  Y[nbRows] = 0.0;
  ++nbRows;

  M[nbRows][0] = 0.0;
  M[nbRows][1] = -1.0;
  Y[nbRows] = 0.0;
  ++nbRows;

  // Solve the system
  // Declare a AABB to memorize the bounding box of the intersection
  // in the coordinates system of tho
  AABB2D bdgBoxLocal = {

    .min = {0.0, 0.0},
    .max = {0.0, 0.0}

  };

  // Declare variables to eliminate the first variable
  // The size of the array given in the doc is a majoring value.
  // Instead I use a smaller value which has proven to be sufficient
  // during tests, validation and qualification, to avoid running
  // into the heap limit and to optimize slightly the performance
  //double Mp[24][2];
  //double Yp[24];
  double Mp[11][2];
  double Yp[11];
  int nbRowsP;

  // Eliminate the first variable
  bool inconsistency =
    ElimVar2D(
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
  GetBoundLastVar2D(
    SND_VAR,
    Mp,
    Yp,
    nbRowsP,
    &bdgBoxLocal);

  // If the bounds are inconsistent
  if (bdgBoxLocal.min[SND_VAR] >= bdgBoxLocal.max[SND_VAR]) {

    // The two Frames are not in intersection
    return false;

  // Else, if the bounds are consistent here it means
  // the two Frames are in intersection.
  // If the user has requested for the resulting bounding box
  } else if (bdgBox != NULL) {

    // Get the bounds of the first variable from the bounds of the
    // second one
    GetBoundVar2D(
      FST_VAR,
      M,
      Y,
      nbRows,
      2,
      &bdgBoxLocal);

    // Memorize the result
    *bdgBox = bdgBoxLocal;

  }

  // If we've reached here the two Frames are intersecting
  return true;

}
