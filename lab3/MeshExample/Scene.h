#pragma once
#include "vec3.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Mesh.h"
#include "glew.h"
#include "glut.h"
#include "freeimage.h"

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

class Object
{
public:
	int textureType;
	unsigned int* picIDs;
	MatSteps mat;
	string objFileName;
	Mesh* msh;
	
	Object()
	{

	}
	Object(string s)
	{
		objFileName = s;
		msh = new Mesh(objFileName);
	}
	void setObj()
	{
		glPushMatrix();
		glTranslatef(mat.translation[0], mat.translation[1], mat.translation[2]);
		glRotatef(mat.angle,mat.rotation[0], mat.rotation[1], mat.rotation[2]);
		glScalef(mat.scale[0], mat.scale[1], mat.scale[2]);

		if (textureType == 1) {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, picIDs[0]);

			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.5f);
		}
		else if (textureType == 2) {
			glActiveTexture(GL_TEXTURE0);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, picIDs[0]);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
			glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);

			glActiveTexture(GL_TEXTURE1);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, picIDs[1]);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
			glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
		}
		else if (textureType == 6) {
			
			glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
			glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
			glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);

			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);
			glEnable(GL_TEXTURE_GEN_R);

			glEnable(GL_TEXTURE_CUBE_MAP);
			glBindTexture(GL_TEXTURE_CUBE_MAP, picIDs[0]);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		}

		int lastMaterial = -1;
		for (size_t j = 0; j < msh->fTotal; j++) {
			// set material property if this face used different material
			if (lastMaterial != msh->faceList[j].m) {
				lastMaterial = (int)msh->faceList[j].m;
				glMaterialfv(GL_FRONT, GL_AMBIENT, msh->mList[lastMaterial].Ka);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, msh->mList[lastMaterial].Kd);
				glMaterialfv(GL_FRONT, GL_SPECULAR, msh->mList[lastMaterial].Ks);
				glMaterialfv(GL_FRONT, GL_SHININESS, &msh->mList[lastMaterial].Ns);
			}

			glBegin(GL_TRIANGLES);
			for (size_t k = 0; k < 3; k++) {
				if (textureType == 1) {
					glTexCoord2fv(msh->tList[msh->faceList[j][k].t].ptr);
				}
				else if (textureType == 2) {
					glMultiTexCoord2fv(GL_TEXTURE0, msh->tList[msh->faceList[j][k].t].ptr);
					glMultiTexCoord2fv(GL_TEXTURE1, msh->tList[msh->faceList[j][k].t].ptr);
				}
				else if (textureType == 6) {
					glTexCoord2fv(msh->tList[msh->faceList[j][k].t].ptr);
				}

				glNormal3fv(msh->nList[msh->faceList[j][k].n].ptr);
				glVertex3fv(msh->vList[msh->faceList[j][k].v].ptr);
			}
			glEnd();
		}

		if (textureType == 1) {
			glDisable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else if (textureType == 2) {
			glActiveTexture(GL_TEXTURE1);
			glDisable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);

			glActiveTexture(GL_TEXTURE0);
			glDisable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else if (textureType == 6) {
			glDisable(GL_TEXTURE_GEN_S);
			glDisable(GL_TEXTURE_GEN_T);
			glDisable(GL_TEXTURE_GEN_R);

			glDisable(GL_TEXTURE_CUBE_MAP);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}

		glPopMatrix();
	
	}

	void reflection(Vec3 a)
	{
		glPushMatrix();
		glTranslatef(mat.translation[0] + (a[0]-mat.translation[0])*2, mat.translation[1], mat.translation[2]);
		glScalef(-1,1,1);
		glRotatef(mat.angle, mat.rotation[0], mat.rotation[1], mat.rotation[2]);
		glScalef(mat.scale[0], mat.scale[1], mat.scale[2]);

		if (textureType == 1) {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, picIDs[0]);

			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.5f);
		}
		else if (textureType == 2) {
			glActiveTexture(GL_TEXTURE0);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, picIDs[0]);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
			glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);

			glActiveTexture(GL_TEXTURE1);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, picIDs[1]);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
			glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
		}
		else if (textureType == 6) {

			glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
			glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
			glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);

			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);
			glEnable(GL_TEXTURE_GEN_R);

			glEnable(GL_TEXTURE_CUBE_MAP);
			glBindTexture(GL_TEXTURE_CUBE_MAP, picIDs[0]);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		}

		int lastMaterial = -1;
		for (size_t j = 0; j < msh->fTotal; j++) {
			// set material property if this face used different material
			if (lastMaterial != msh->faceList[j].m) {
				lastMaterial = (int)msh->faceList[j].m;
				glMaterialfv(GL_FRONT, GL_AMBIENT, msh->mList[lastMaterial].Ka);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, msh->mList[lastMaterial].Kd);
				glMaterialfv(GL_FRONT, GL_SPECULAR, msh->mList[lastMaterial].Ks);
				glMaterialfv(GL_FRONT, GL_SHININESS, &msh->mList[lastMaterial].Ns);
			}

			glBegin(GL_TRIANGLES);
			for (size_t k = 0; k < 3; k++) {
				if (textureType == 1) {
					glTexCoord2fv(msh->tList[msh->faceList[j][k].t].ptr);
				}
				else if (textureType == 2) {
					glMultiTexCoord2fv(GL_TEXTURE0, msh->tList[msh->faceList[j][k].t].ptr);
					glMultiTexCoord2fv(GL_TEXTURE1, msh->tList[msh->faceList[j][k].t].ptr);
				}
				else if (textureType == 6) {
					glTexCoord2fv(msh->tList[msh->faceList[j][k].t].ptr);
				}

				glNormal3fv(msh->nList[msh->faceList[j][k].n].ptr);
				glVertex3fv(msh->vList[msh->faceList[j][k].v].ptr);
			}
			glEnd();
		}

		if (textureType == 1) {
			glDisable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else if (textureType == 2) {
			glActiveTexture(GL_TEXTURE1);
			glDisable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);

			glActiveTexture(GL_TEXTURE0);
			glDisable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else if (textureType == 6) {
			glDisable(GL_TEXTURE_GEN_S);
			glDisable(GL_TEXTURE_GEN_T);
			glDisable(GL_TEXTURE_GEN_R);

			glDisable(GL_TEXTURE_CUBE_MAP);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}

		glPopMatrix();
	}
};

class Scene

{
public:
	int sct;
	vector<Object> objs;
	vector<vector<string> > picNames;
	int objNum;
	int textureNum;
	unsigned int* createIDs(int type)
	{
			unsigned int* textureID;
			textureID = new unsigned int[6];
			FreeImage_Initialise();
			
			if (type == 1)
			{
				glGenTextures(1, textureID);
				glBindTexture(GL_TEXTURE_2D, textureID[0]);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				
				loadTexture(picNames[textureNum][0].c_str(), GL_TEXTURE_2D);

				glGenerateMipmap(GL_TEXTURE_2D);
				glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

				
			}
			else if (type==2)
			{
				glGenTextures(2, textureID);
				glBindTexture(GL_TEXTURE_2D, textureID[0]);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

				loadTexture(picNames[textureNum][0].c_str(), GL_TEXTURE_2D);
				glGenerateMipmap(GL_TEXTURE_2D);
				glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

				glBindTexture(GL_TEXTURE_2D, textureID[1]);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				
				loadTexture(picNames[textureNum][1].c_str(), GL_TEXTURE_2D);
				glGenerateMipmap(GL_TEXTURE_2D);
				glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
			}
			else if (type == 6)
			{
				glGenTextures(1, textureID);

				glBindTexture(GL_TEXTURE_CUBE_MAP, textureID[0]);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				cout << "NAMMMMMMMMMMMMMMMMMMMM" << endl;
				cout << picNames[textureNum][0] << endl;

				loadTexture(picNames[textureNum][0].c_str(), GL_TEXTURE_CUBE_MAP_POSITIVE_X);
				loadTexture(picNames[textureNum][1].c_str(), GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
				loadTexture(picNames[textureNum][2].c_str(), GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
				loadTexture(picNames[textureNum][3].c_str(), GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
				loadTexture(picNames[textureNum][4].c_str(), GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
				loadTexture(picNames[textureNum][5].c_str(), GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
				glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
				glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
			}

			FreeImage_DeInitialise();
			return textureID;
	}

	void loadTexture(const char* fileName, GLenum target)
	{
		FIBITMAP *pImage = FreeImage_Load(FreeImage_GetFileType(fileName, 0), fileName);
		FIBITMAP *p32BitsImage = FreeImage_ConvertTo32Bits(pImage);

		int iWidth = FreeImage_GetWidth(p32BitsImage);
		int iHeight = FreeImage_GetHeight(p32BitsImage);

		glTexImage2D(target, 0, GL_RGBA8, iWidth, iHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(p32BitsImage));

		FreeImage_Unload(p32BitsImage);
		FreeImage_Unload(pImage);
	}

	void set()
	{
		glMatrixMode(GL_MODELVIEW);
		cout << "start rendering" << endl;
		for (int k = 0; k < objNum; ++k)
		{
			objs[k].setObj();
			
		}
	}

	void select(int s)
	{
		if (s < objNum)
		{
			sct = s;
		}
	}
	void translteSelect(Vec3 t)
	{
		objs[sct].mat.translate(t);
	}
	void readFile(string file)
	{
		fstream fp(file, ios::in);
		string s;
		int nowType=0;
		unsigned int* textID=0;
		while (fp >> s)
		{
			if (s == "no-texture")
			{
				nowType = 0;
			}
			if (s == "single-texture")
			{
				nowType = 1;
				fp >> s;
				vector<string> a;
				a.push_back(s);
				picNames.push_back(a);
				cout <<"fjeijfijfisejfis"<<endl<< picNames[textureNum][0]<<endl;
				textID = createIDs(nowType);
				cout << textID[0]<<endl;
				++textureNum;
			}
			else if (s == "multi-texture")
			{
				vector<string> a;
				picNames.push_back(a);
				fp >> s;
				picNames[textureNum ].push_back(s);
				fp >> s;
				picNames[textureNum ].push_back(s);
				nowType = 2;
				textID = createIDs(nowType);
				++textureNum;
			}
			else if (s == "cube-map")
			{
				nowType = 6;
				vector<string> a;
				picNames.push_back(a);
				fp >> s;
				picNames[textureNum ].push_back(s);
				fp >> s;
				picNames[textureNum ].push_back(s);
				fp >> s;
				picNames[textureNum ].push_back(s);
				fp >> s;
				picNames[textureNum ].push_back(s);
				fp >> s;
				picNames[textureNum ].push_back(s);
				fp >> s;
				picNames[textureNum ].push_back(s);
				cout << "cube"  << endl;
				cout << picNames[textureNum][5] << endl;
				textID = createIDs(nowType);
				cout << textID[0] << endl;
				++textureNum;
			}

			if (s == "model")
			{
				fp >> s;
				objs.push_back(s);
				fp >> objs[objNum].mat.scale[0] >> objs[objNum].mat.scale[1] >> objs[objNum].mat.scale[2];
				fp >> objs[objNum].mat.angle;
				fp >> objs[objNum].mat.rotation[0] >> objs[objNum].mat.rotation[1] >> objs[objNum].mat.rotation[2];
				fp >> objs[objNum].mat.translation[0] >> objs[objNum].mat.translation[1] >> objs[objNum].mat.translation[2];
				objs[objNum].textureType = nowType;
				objs[objNum].picIDs = textID;
				++objNum;
			}
			

		}
		fp.close();
	}

	Scene(string file)
	{
		sct = 0;
		objNum = 0;
		textureNum = 0;
		readFile(file);
		print();
	}

	void print()
	{
		for (size_t i = 0; i < objNum; ++i)
		{
			cout << objs[i].objFileName << endl;
			cout << objs[i].mat.scale[0] << endl;
			cout << objs[i].mat.angle << endl;
			cout << objs[i].mat.rotation[0] << endl;
			cout << objs[i].mat.translation[0] << endl;
		}
	}
};

/*model obj_file_name Sx Sy Sz Angle Rx Ry Rz Tx Ty Tz*/