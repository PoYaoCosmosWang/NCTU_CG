#ifndef VIEW
#define VIEW
#include<fstream>
#include<sstream>
#include"vec3.h"
#include <string>
#include <iostream>
using namespace std;
class View
{
public:
	const double zoomScale=1.0;
	const double angleScale = 1.0;
	double Rangle;
	Vec3 eye,vAt,vUp;
	double fovy;
	double dNear;
	double dFar;
	double viewport[4];
	void readfile(string file)
	{
		fstream fp(file, ios::in);
		string s;
		while (fp >> s)
		{
			if (s == "eye")
			{
				fp >> eye[0] >> eye[1] >> eye[2];
			}
			else if (s == "vat")
			{
				fp >> vAt[0] >> vAt[1] >> vAt[2];
			}
			else if (s == "vup")
			{
				fp >> vUp[0] >> vUp[1] >> vUp[2];
			}
			else if (s == "fovy")
			{
				fp >> fovy;
			}
			else if (s == "dnear")
			{
				fp >> dNear;
			}
			else if (s == "dfar")
			{
				fp >> dFar;
			}
			else if (s == "viewport")
			{
				fp >> viewport[0] >> viewport[1] >> viewport[2]>> viewport[3];
			}
			else
			{
				cout << "error" << endl<<"do not have:"<<s<<endl;
				return;
			}
		}
		fp.close();
	}
	void zoom(int dir)
	{
		fovy += dir*zoomScale;
	}
	void rotate(int dir)
	{
		Rangle += dir*angleScale;
	}
	View(string s)
	{
		Rangle = 0;
		readfile(s);
		print();
	}
	void print()
	{
		cout << eye[0] << endl;
		cout << vAt[0] << endl;
		cout << vUp[0] << endl;
		cout << fovy << endl;
		cout << dFar << endl;
		cout << dNear << endl;
		cout << viewport[0] << endl;
	}
};
/*
eye x y z : define the position of the camera.
vat x y z : define the position where the camera looks at.
vup x y z : define the up vector of the camera.
fovy theta : define the fovy angle of the view frustum.
dnear d : define the distance of the near plane from the camera.
dfar d : define the distance of the far plane from the camera.
viewport x y w h : define the position and the size of the view port.
*/
#endif // !VIEW

