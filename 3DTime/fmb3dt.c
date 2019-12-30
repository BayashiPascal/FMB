#include "fmb3dt.h"

// ------------- Macros -------------

// Return 1.0 if v is positive, -1.0 if v is negative, 0.0 else
#define sgn(v) (((0.0 < (v)) ? 1 : 0) - (((v) < 0.0) ? 1 : 0))

// Return x if x is negative, 0.0 else
#define neg(x) (x < 0.0 ? x : 0.0)

#define FST_VAR 0
#define SND_VAR 1
#define THD_VAR 2
#define FOR_VAR 3

#define EPSILON 0.0000001

// ------------- Functions declaration -------------

// Eliminate the 'iVar'-th variable in the system 'M'.X<='Y'
// using the Fourier-Motzkin method and return
// the resulting system in 'Mp' and 'Yp', and the number of rows of 
// the resulting system in 'nbRemainRows'
// Return false if the system becomes inconsistent during elimination,
// else return true
bool ElimVar3DTime(
     const int iVar, 
  const double (*M)[4], 
  const double* Y, 
     const int nbRows, 
     const int nbCols, 
        double (*Mp)[4], 
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
void GetBound3DTime(
     const int iVar,
  const double (*M)[4], 
  const double* Y, 
     const int nbRows, 
   AABB3DTime* const bdgBox);

// ------------- Functions implementation -------------

// Eliminate the 'iVar'-th variable in the system 'M'.X<='Y'
// using the Fourier-Motzkin method and return
// the resulting system in 'Mp' and 'Yp', and the number of rows of 
// the resulting system in 'nbRemainRows'
// ('M' arrangement is [iRow][iCol])
// Return true if the system becomes inconsistent during elimination,
// else return false
bool ElimVar3DTime(
     const int iVar, 
  const double (*M)[4], 
  const double* Y, 
     const int nbRows, 
     const int nbCols, 
        double (*Mp)[4], 
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

            // Update the sum of the negative coefficient
            sumNegCoeff += neg(Mp[*nbRemainRows][jCol]);

            // Increment the number of columns in the new inequality
            ++jCol;

          }

        }

        // Update the right side of the inequality
        Yp[*nbRemainRows] = 
          YIRowDivideByFabsMIRowIVar +
          Y[jRow] / fabs(M[jRow][iVar]);

        // If the right side of the inequality if lower than the sum of 
        // negative coefficients in the row
        // (Add epsilon for numerical imprecision)
        if (Yp[*nbRemainRows] < sumNegCoeff - EPSILON) {

          // Given that X is in [0,1], the system is inconsistent
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

  // If we reach here the system is not inconsistent
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
void GetBound3DTime(
     const int iVar,
  const double (*M)[4], 
  const double* Y, 
     const int nbRows, 
   AABB3DTime* const bdgBox) {

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
// The resulting AABB is given in 'tho' 's local coordinates system
bool FMBTestIntersection3DTime(
  const Frame3DTime* const that, 
  const Frame3DTime* const tho, 
         AABB3DTime* const bdgBox) {

  // Get the projection of the Frame 'tho' in Frame 'that' coordinates
  // system
  Frame3DTime thoProj;
  Frame3DTimeImportFrame(that, tho, &thoProj);

  // Declare two variables to memorize the system to be solved M.X <= Y
  // (M arrangement is [iRow][iCol])
  double M[14][4];
  double Y[14];

  // Create the inequality system

  // -V_jT-sum_iC_j,iX_i<=O_j
  M[0][0] = -thoProj.comp[0][0];
  M[0][1] = -thoProj.comp[1][0];
  M[0][2] = -thoProj.comp[2][0];
  M[0][3] = -thoProj.speed[0];
  Y[0] = thoProj.orig[0];
  if (Y[0] < neg(M[0][0]) + neg(M[0][1]) + neg(M[0][2]) + neg(M[0][3]))
    return false;

  M[1][0] = -thoProj.comp[0][1];
  M[1][1] = -thoProj.comp[1][1];
  M[1][2] = -thoProj.comp[2][1];
  M[1][3] = -thoProj.speed[1];
  Y[1] = thoProj.orig[1];
  if (Y[1] < neg(M[1][0]) + neg(M[1][1]) + neg(M[1][2]) + neg(M[1][3]))
    return false;

  M[2][0] = -thoProj.comp[0][2];
  M[2][1] = -thoProj.comp[1][2];
  M[2][2] = -thoProj.comp[2][2];
  M[2][3] = -thoProj.speed[2];
  Y[2] = thoProj.orig[2];
  if (Y[2] < neg(M[2][0]) + neg(M[2][1]) + neg(M[2][2]) + neg(M[2][3]))
    return false;

  // Variable to memorise the nb of rows in the system
  int nbRows = 3;

  if (that->type == FrameCuboid) {

    // V_jT+sum_iC_j,iX_i<=1.0-O_j
    M[nbRows][0] = thoProj.comp[0][0];
    M[nbRows][1] = thoProj.comp[1][0];
    M[nbRows][2] = thoProj.comp[2][0];
    M[nbRows][3] = thoProj.speed[0];
    Y[nbRows] = 1.0 - thoProj.orig[0];
    if (Y[nbRows] < neg(M[nbRows][0]) + neg(M[nbRows][1]) + 
                    neg(M[nbRows][2]) + neg(M[nbRows][3]))
      return false;
    ++nbRows;

    M[nbRows][0] = thoProj.comp[0][1];
    M[nbRows][1] = thoProj.comp[1][1];
    M[nbRows][2] = thoProj.comp[2][1];
    M[nbRows][3] = thoProj.speed[1];
    Y[nbRows] = 1.0 - thoProj.orig[1];
    if (Y[nbRows] < neg(M[nbRows][0]) + neg(M[nbRows][1]) + 
                    neg(M[nbRows][2]) + neg(M[nbRows][3]))
      return false;
    ++nbRows;

    M[nbRows][0] = thoProj.comp[0][2];
    M[nbRows][1] = thoProj.comp[1][2];
    M[nbRows][2] = thoProj.comp[2][2];
    M[nbRows][3] = thoProj.speed[2];
    Y[nbRows] = 1.0 - thoProj.orig[2];
    if (Y[nbRows] < neg(M[nbRows][0]) + neg(M[nbRows][1]) + 
                    neg(M[nbRows][2]) + neg(M[nbRows][3]))
      return false;
    ++nbRows;

  } else {

    // sum_j(V_jT+sum_iC_j,iX_i)<=1.0-sum_iO_i
    M[nbRows][0] = 
      thoProj.comp[0][0] + thoProj.comp[0][1] + thoProj.comp[0][2];
    M[nbRows][1] = 
      thoProj.comp[1][0] + thoProj.comp[1][1] + thoProj.comp[1][2];
    M[nbRows][2] = 
      thoProj.comp[2][0] + thoProj.comp[2][1] + thoProj.comp[2][2];
    M[nbRows][3] = thoProj.speed[0] + thoProj.speed[1] + thoProj.speed[2];
    Y[nbRows] = 1.0 - thoProj.orig[0] - thoProj.orig[1] - thoProj.orig[2];
    if (Y[nbRows] < neg(M[nbRows][0]) + neg(M[nbRows][1]) + 
                    neg(M[nbRows][2]) + neg(M[nbRows][3]))
      return false;
    ++nbRows;

  }

  if (tho->type == FrameCuboid) {

    // X_i <= 1.0
    M[nbRows][0] = 1.0;
    M[nbRows][1] = 0.0;
    M[nbRows][2] = 0.0;
    M[nbRows][3] = 0.0;
    Y[nbRows] = 1.0;
    ++nbRows;

    M[nbRows][0] = 0.0;
    M[nbRows][1] = 1.0;
    M[nbRows][2] = 0.0;
    M[nbRows][3] = 0.0;
    Y[nbRows] = 1.0;
    ++nbRows;

    M[nbRows][0] = 0.0;
    M[nbRows][1] = 0.0;
    M[nbRows][2] = 1.0;
    M[nbRows][3] = 0.0;
    Y[nbRows] = 1.0;
    ++nbRows;

  } else {
    
    // sum_iX_i<=1.0
    M[nbRows][0] = 1.0;
    M[nbRows][1] = 1.0;
    M[nbRows][2] = 1.0;
    M[nbRows][3] = 0.0;
    Y[nbRows] = 1.0;
    ++nbRows;

  }

  // -X_i <= 0.0
  M[nbRows][0] = -1.0;
  M[nbRows][1] = 0.0;
  M[nbRows][2] = 0.0;
  M[nbRows][3] = 0.0;
  Y[nbRows] = 0.0;
  ++nbRows;

  M[nbRows][0] = 0.0;
  M[nbRows][1] = -1.0;
  M[nbRows][2] = 0.0;
  M[nbRows][3] = 0.0;
  Y[nbRows] = 0.0;
  ++nbRows;

  M[nbRows][0] = 0.0;
  M[nbRows][1] = 0.0;
  M[nbRows][2] = -1.0;
  M[nbRows][3] = 0.0;
  Y[nbRows] = 0.0;
  ++nbRows;

  // 0.0 <= t <= 1.0
  M[nbRows][0] = 0.0;
  M[nbRows][1] = 0.0;
  M[nbRows][2] = 0.0;
  M[nbRows][3] = 1.0;
  Y[nbRows] = 1.0;
  ++nbRows;

  M[nbRows][0] = 0.0;
  M[nbRows][1] = 0.0;
  M[nbRows][2] = 0.0;
  M[nbRows][3] = -1.0;
  Y[nbRows] = 0.0;
  ++nbRows;

  // Solve the system

  // Declare a AABB to memorize the bounding box of the intersection
  // in the coordinates system of that
  AABB3DTime bdgBoxLocal;
  
  // Declare variables to eliminate the first variable
  double Mp[49][4];
  double Yp[49];
  int nbRowsP;

  // Eliminate the first variable in the original system
  bool inconsistency = 
    ElimVar3DTime(
      FST_VAR,
      M, 
      Y, 
      nbRows, 
      4,
      Mp, 
      Yp, 
      &nbRowsP);

  // If the system is inconsistent
  if (inconsistency == true) {

    // The two Frames are not in intersection
    return false;

  }

  // Declare variables to eliminate the second variable
  double Mpp[625][4];
  double Ypp[625];
  int nbRowsPP;

  // Eliminate the second variable (which is the first in the new system)
  inconsistency = 
    ElimVar3DTime(
      FST_VAR,
      Mp, 
      Yp, 
      nbRowsP, 
      3,
      Mpp, 
      Ypp, 
      &nbRowsPP);

  // If the system is inconsistent
  if (inconsistency == true) {

    // The two Frames are not in intersection
    return false;

  }

  // Declare variables to eliminate the third variable
  double Mppp[97969][4];
  double Yppp[97969];
  int nbRowsPPP;

  // Eliminate the third variable (which is the first in the new system)
  inconsistency = 
    ElimVar3DTime(
      FST_VAR,
      Mpp, 
      Ypp, 
      nbRowsPP, 
      2,
      Mppp, 
      Yppp, 
      &nbRowsPPP);

  // If the system is inconsistent
  if (inconsistency == true) {

    // The two Frames are not in intersection
    return false;

  }

  // Get the bounds for the remaining fourth variable
  GetBound3DTime(
    FOR_VAR,
    Mppp,
    Yppp,
    nbRowsPPP,
    &bdgBoxLocal);

  // If the bounds are inconstent
  if (bdgBoxLocal.min[FOR_VAR] >= bdgBoxLocal.max[FOR_VAR]) {

    // The two Frames are not in intersection
    return false;

  // Else, if the bounds are consistent here it means
  // the two Frames are in intersection.
  // If the user hasn't requested for the resulting bounding box
  } else if (bdgBox == NULL) {

    // Immediately return true
    return true;

  }

  // Eliminate the fourth variable (which is the second in the new
  // system)
  inconsistency = 
    ElimVar3DTime(
      SND_VAR,
      Mpp, 
      Ypp, 
      nbRowsPP, 
      2,
      Mppp, 
      Yppp, 
      &nbRowsPPP);

  // Get the bounds for the remaining third variable
  GetBound3DTime(
    THD_VAR,
    Mppp,
    Yppp,
    nbRowsPPP,
    &bdgBoxLocal);

  // Now starts again from the initial systems and eliminate the 
  // third and fourth variables to get the bounds of the first and
  // second variables.
  // No need to check for consistency because we already know here
  // that the Frames are intersecting and the system is consistent
  inconsistency = 
    ElimVar3DTime(
      FOR_VAR,
      M, 
      Y, 
      nbRows, 
      4,
      Mp, 
      Yp, 
      &nbRowsP);

  inconsistency = 
    ElimVar3DTime(
      THD_VAR,
      Mp, 
      Yp, 
      nbRowsP, 
      3,
      Mpp, 
      Ypp, 
      &nbRowsPP);

  inconsistency = 
    ElimVar3DTime(
      SND_VAR,
      Mpp, 
      Ypp, 
      nbRowsPP, 
      2,
      Mppp, 
      Yppp, 
      &nbRowsPPP);

  GetBound3DTime(
    FST_VAR,
    Mppp,
    Yppp,
    nbRowsPPP,
    &bdgBoxLocal);

  inconsistency = 
    ElimVar3DTime(
      FST_VAR,
      Mpp, 
      Ypp, 
      nbRowsPP, 
      2,
      Mppp, 
      Yppp, 
      &nbRowsPPP);

  GetBound3DTime(
    SND_VAR,
    Mppp,
    Yppp,
    nbRowsPPP,
    &bdgBoxLocal);

  // If the user requested the resulting bounding box
  if (bdgBox != NULL) {

    // Memorize the result
    *bdgBox = bdgBoxLocal;
    
  }

  // If we've reached here the two Frames are intersecting
  return true;

}
