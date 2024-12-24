#pragma once
#include "DrawDebugHelpers.h"
#define DRAW_SPHERE(Location) if(GetWorld()) DrawDebugSphere(GetWorld(),Location,25.f,12,FColor::Red,true)
#define DRAW_SPHERE_COLOR(Location,Color) if(GetWorld()) DrawDebugSphere(GetWorld(),Location,8.f,12,Color,false,5.0f)
#define DRAW_LINE(StartLocaiton, EndLocation) if(GetWorld()) 
#define DRAW_POINT(Location) if(GetWorld()) 
//false�� �����ϰ� lifetime�� -1.f�� �����Ͽ� �׸��Ⱑ ���� �׸��� ������ ȭ�鿡 ��������.
#define DRAW_SPHERE_SingleFrame(Loc) if(GetWorld()) DrawDebugSphere(GetWorld(),Loc,25.f,12,FColor::Red, false,-1.f)
#define DRAW_LINE_SingleFrame(StartLoc, EndLoc) if(GetWorld()) DrawDebugLine(GetWorld(),StartLoc, EndLoc, FColor::Red ,false,-1.f ,0,1.f)
#define DRAW_POINT_SingleFrame(Loc) if(GetWorld()) DrawDebugPoint(GetWorld(),EndLoc , 15.f, FColor::Red, false,-1.f)
#define DRAW_VECTOR(StartLoc,EndLoc) if(GetWorld()) \
	{\
		DrawDebugLine(GetWorld(),StartLoc, EndLoc, FColor::Red ,true,-1.f ,0,1.f);\
		DrawDebugPoint(GetWorld(),EndLoc , 15.f, FColor::Red, true);\
	}

#define DRAW_VECTOR_SingleFrame(StartLoc,EndLoc) if(GetWorld()) \
	{\
		DrawDebugLine(GetWorld(),StartLoc, EndLoc, FColor::Red ,false,-1.f ,0,1.f);\
		DrawDebugPoint(GetWorld(),EndLoc , 15.f, FColor::Red, false,-1.f);\
	}