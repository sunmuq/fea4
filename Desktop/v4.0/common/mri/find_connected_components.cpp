#include "find_connected_components.h"
#include <stdlib.h>
#include <stdio.h>
#include <QDebug>
#include <QTime>

void find_connected_components(int N1,int N2,int N3,int *output,unsigned char *input) {
	QTime timer;
	timer.start();
	for (long j=0; j<N1*N2*N3; j++)
		output[j]=0;
		
	long voxel_ct=0;
	for (int i1=0; i1<N1; 	i1++)
	for (int i2=0; i2<N2; 	i2++)
	for (int i3=0; i3<N3; 	i3++)
		if (input[i1+N1*i2+N1*N2*i3]) voxel_ct++;
	if (voxel_ct==0) return;

	short *Vx=(short *)malloc(sizeof(short)*voxel_ct);
	short *Vy=(short *)malloc(sizeof(short)*voxel_ct);
	short *Vz=(short *)malloc(sizeof(short)*voxel_ct);
	bool *Vactive=(bool *)malloc(sizeof(bool)*voxel_ct);
	int *Vcomponent=(int *)malloc(sizeof(int)*voxel_ct);
	int *grid=(int *)malloc(sizeof(int)*N1*N2*N3);
	
	for (long j=0; j<N1*N2*N3; j++)
		grid[j]=-1;
	long num_voxels=0;
	for (int i1=0; i1<N1; 	i1++)
	for (int i2=0; i2<N2; 	i2++)
	for (int i3=0; i3<N3; 	i3++) {
		if (input[i1+N1*i2+N1*N2*i3]) {
			grid[i1+i2*N1+i3*N1*N2]=num_voxels; //voxel number
			Vx[num_voxels]=i1;
			Vy[num_voxels]=i2;
			Vz[num_voxels]=i3;
			Vcomponent[num_voxels]=num_voxels+1; //initial label (label=component)
			Vactive[num_voxels]=true;
			num_voxels++;
		}
	}

	long ct=0;
	bool done=false;
	while (!done) {
		bool changed=false;
		for (long j=0; j<num_voxels; j++) {
			if (Vactive[j]) {
				int x=Vx[j];
				int y=Vy[j];
				int z=Vz[j];
				for (int dx=-1; dx<=1; dx++)
				if ((0<=x+dx)&&(x+dx<N1))
				for (int dy=-1; dy<=1; dy++)
				if ((0<=y+dy)&&(y+dy<N2))
				for (int dz=-1; dz<=1; dz++) 
				if ((0<=z+dz)&&(z+dz<N3)) {
					if ((dx!=0)||(dy!=0)||(dz!=0)) {
						if (grid[(x+dx)+(y+dy)*N1+(z+dz)*N1*N2]>=0) {
							if (Vcomponent[grid[(x+dx)+(y+dy)*N1+(z+dz)*N1*N2]]>Vcomponent[j]) {
								Vcomponent[grid[(x+dx)+(y+dy)*N1+(z+dz)*N1*N2]]=Vcomponent[j];
								Vactive[grid[(x+dx)+(y+dy)*N1+(z+dz)*N1*N2]]=true;
								changed=true;
							}
						}
					}
				}
				Vactive[j]=false;
			}
		}
		ct++;
		if (!changed) done=true;
	}
	
	int *component_maps=(int *)malloc(sizeof(int)*(num_voxels+1));
	for (long j=0; j<num_voxels+1; j++)
		component_maps[j]=0;
	int component_number=1;
	for (long j=0; j<num_voxels; j++) {
		if (component_maps[Vcomponent[j]]==0) {
			component_maps[Vcomponent[j]]=component_number;
			component_number++;
		}
	}
	
	for (long j=0; j<num_voxels; j++) {
		int x=Vx[j];
		int y=Vy[j];
		int z=Vz[j];
		output[x+N1*y+N1*N2*z]=component_maps[Vcomponent[j]];
	}
	
	free(component_maps);
	free(Vx);
	free(Vy);
	free(Vz);
	free(Vactive);
	free(Vcomponent);
	free(grid);
}

