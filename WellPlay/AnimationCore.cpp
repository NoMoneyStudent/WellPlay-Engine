#include "stdafx.h"
#include "AnimationCore.h"
#include "SystemTime.h"
#include "Spline\spline.h"

using namespace Render;
using namespace std;
using namespace DirectX;

struct AniTable
{
	vector<double> T[3][2];
	vector<double> R[3][2];
	vector<double> S[3][2];
};
vector<AniTable> samples;
vector<tk::spline> splines;
float sincetime = 0;
float finaltime = 0;

//void Ani(Render::RenderObject& model)
//{
//	float currentT[3], currentR[3], currentS[3];
//	sincetime += (float)SystemTime::deltaTime();
//	if (sincetime > finaltime)
//		sincetime -= finaltime;
//	int count = 0;
//	vector<Bone>& BoneList = *(model.boneList);
//	for (int i = 0; i < samples.size(); i++)
//	{
//		for (int j = 0; j < 3; j++)
//		{
//			currentT[j] = splines[count++](sincetime);
//			currentR[j] = splines[count++](sincetime);
//			currentS[j] = splines[count++](sincetime);
//		}
//		XMMATRIX parent;
//		if (BoneList[i].parentindex == -1)
//			parent = XMMatrixIdentity();
//		else
//			parent = XMLoadFloat4x4(&BoneList[BoneList[i].parentindex].SRT);
//		XMMATRIX me;
//		me = XMMatrixRotationX(BoneList[i].preRotation[0]);
//		me *= XMMatrixRotationY(BoneList[i].preRotation[1]);
//		me *= XMMatrixRotationZ(BoneList[i].preRotation[2]);
//		me *= XMMatrixScaling(currentS[0], currentS[1], currentS[2]);
//		me *= XMMatrixRotationX(currentR[0])*XMMatrixRotationY(currentR[1])*XMMatrixRotationZ(currentR[2]);
//		me *= XMMatrixTranslation(currentT[0], currentT[1], currentT[2]);
//
//		XMStoreFloat4x4(&BoneList[i].SRT, me*parent);
//	}
//}
//
//void InitAni(Render::RenderObject& model)
//{
//	vector<Bone>& bonelist = *model.boneList;
//	samples.reserve(bonelist.size());
//	splines.reserve(bonelist.size() * 9);
//	
//	for (int i = 0; i < bonelist.size(); i++)
//	{
//		AniTable tempSamples;
//		for (int j = 0; j < 3; j++)
//		{
//			tempSamples.T[j][0].reserve(bonelist[i].Animaions[0].T[j].size());
//			tempSamples.T[j][1].reserve(bonelist[i].Animaions[0].T[j].size());
//			for (int k = 0; k < bonelist[i].Animaions[0].T[j].size(); k++)
//			{
//				tempSamples.T[j][0].push_back(bonelist[i].Animaions[0].T[j][k][1]);
//				tempSamples.T[j][1].push_back(bonelist[i].Animaions[0].T[j][k][0]);
//				if (k == bonelist[i].Animaions[0].T[j].size() - 1)
//					finaltime = bonelist[i].Animaions[0].T[j][k][1];
//			}
//			{tk::spline tempsline;
//			tempsline.set_points(tempSamples.T[j][0], tempSamples.T[j][1]);
//			splines.push_back(tempsline);
//			}
//			tempSamples.R[j][0].reserve(bonelist[i].Animaions[0].R[j].size());
//			tempSamples.R[j][1].reserve(bonelist[i].Animaions[0].R[j].size());
//			for (int k = 0; k < bonelist[i].Animaions[0].R[j].size(); k++)
//			{
//				tempSamples.R[j][0].push_back(bonelist[i].Animaions[0].R[j][k][1]);
//				tempSamples.R[j][1].push_back(bonelist[i].Animaions[0].R[j][k][0]);
//			}
//			{tk::spline tempsline;
//			tempsline.set_points(tempSamples.R[j][0], tempSamples.R[j][1]);
//			splines.push_back(tempsline);
//			}
//			tempSamples.S[j][0].reserve(bonelist[i].Animaions[0].S[j].size());
//			tempSamples.S[j][1].reserve(bonelist[i].Animaions[0].S[j].size());
//			for (int k = 0; k < bonelist[i].Animaions[0].S[j].size(); k++)
//			{
//				tempSamples.S[j][0].push_back(bonelist[i].Animaions[0].S[j][k][1]);
//				tempSamples.S[j][1].push_back(bonelist[i].Animaions[0].S[j][k][0]);
//			}
//			{tk::spline tempsline;
//			tempsline.set_points(tempSamples.S[j][0], tempSamples.S[j][1]);
//			splines.push_back(tempsline);
//			}
//		}
//		samples.push_back(std::move(tempSamples));
//	}
//}