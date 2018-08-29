#ifndef fbblocksolver_H
#define fbblocksolver_H

#include "fbglobal.h"
#include "arrays.h"
#include "fberrorestimator.h"
#include "fbblock.h"

class FBBlockSolverPrivate;
class FBBlockSolver {
public:
	friend class FBBlockSolverPrivate;
	FBBlockSolver();
	virtual ~FBBlockSolver();
	void setEpsilon(fbreal epsilon);
	void setMaxIterations(int val);
	void setNumThreads(int val);
	void setUsePreconditioner(bool val);
	void setStiffnessMatrix(const FBArray2D<float> &stiffness_matrix);
	void setBVFMap(const FBArray3D<unsigned char> &bvf_map); // N1 x N2 x N3
	void setInitialDisplacements(const FBSparseArray4D &displacements); //(N1+1)x(N2+1)x(N3+1)x3
	void setInitialDisplacements(FBMacroscopicStrain &strain);
	void setFixedVariables(const FBSparseArray4D &fixed_variables); //(N1+1)x(N2+1)x(N3+1)x3
	long setFixedVariables(FBMacroscopicStrain &macroscopic_strain);
	void setResolution(QList<fbreal> &res);
	void solve(); 
	
	int getNumIterations();
	QList<double> getStress();	
	void getDisplacements(FBSparseArray4D &displacements); //(N1+1)x(N2+1)x(N3+1)x3
	void getForces(FBSparseArray4D &forces); //(N1+1)x(N2+1)x(N3+1)x3
	void getEnergy(FBSparseArray4D &energy); //N1 x N2 x N3 x 1
	FBErrorEstimator *errorEstimator();
private:
	FBBlockSolverPrivate *d;
};

#endif
