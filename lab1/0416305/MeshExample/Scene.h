#pragma once
#include "vec3.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

class MatSteps
{
public:
	Vec3 scale;
	double angle;
	Vec3 rotation;
	Vec3 translation;
	void translate(Vec3 t)
	{
		translation += t;
	}
};

class Scene
{
public:
	int sct;
	vector<string> objFileNames;
	vector<MatSteps> mats;
	int objNum;
	void select(int s)
	{
		if (s < objNum)
		{
			sct = s;
		}
	}
	void translteSelect(Vec3 t)
	{
		
		mats[sct].translate(t);
	}
	void readFile(string file)
	{
		fstream fp(file, ios::in);
		string s;
		while (fp >> s)
		{
			if (s == "model")
			{
				fp >> s;
				++objNum;
				objFileNames.push_back(s);
				MatSteps mat;
				fp >> mat.scale[0] >> mat.scale[1] >> mat.scale[2];
				fp >> mat.angle;
				fp >> mat.rotation[0] >> mat.rotation[1] >> mat.rotation[2];
				fp >> mat.translation[0] >> mat.translation[1] >> mat.translation[2];
				mats.push_back(mat);
			}
			else
			{
				cout << "error" << endl << "do not have:" << s << endl;
				return;
			}
		}
		fp.close();
	}

	Scene(string file)
	{
		sct = 0;
		objNum = 0;
		readFile(file);
		print();
	}

	void print()
	{
		for (size_t i = 0; i < objFileNames.size(); ++i)
		{
			cout << objFileNames[i] << endl;
			cout << mats[i].scale[0] << endl;
			cout << mats[i].angle << endl;
			cout << mats[i].rotation[0] << endl;
			cout << mats[i].translation[0] << endl;
		}
	}
};

/*model obj_file_name Sx Sy Sz Angle Rx Ry Rz Tx Ty Tz*/