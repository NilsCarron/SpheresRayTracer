#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <algorithm>

using namespace cv;
using namespace std;




struct Ray {
	Vec3f origin;
	Vec3f direction;

};
struct Light {
	Vec3f originOfLight;

	float brightness;



};
struct Sphere {
	Vec3f originOfSphere;
	Vec3f vecteurColor;

	float rayon;
	float albedo;
	bool obstruction;



	float intersection(Ray& r) {

		Vec3f op = originOfSphere - r.origin;        // Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0
		float t, eps = 1e-4, b = op.dot(r.direction), det =
			b * b - op.dot(op) + rayon * rayon;
		if (det < 0)
			return 0;
		else
			det = sqrt(det);
		return (t = b - det) > eps ? t : ((t = b + det) > eps ? t : 0);



	}
};


int main()
{
	float screenWidth = 1000.f;
	float screenHeight = 1000.f;
	Sphere orb1{ Vec3f{350.f, 365.0f,  350.0f}, { 1, 1 , 1 }, 5.0f, 1.0f };
	Sphere orb2{ Vec3f{350.f, 350.0f, 350.0f}, { 1, 0 , 1 }, 50.0f, 0.50f };
	Sphere orb3{ Vec3f{900.f, 0.0f,  300}, { 0, 1 , 1 }, 75.0, 0.25f };

	//Sphere wallBa{ Vec3f{screenWidth/2, screenHeight /2, screenWidth*10}, { 1, 1 , 1 }, screenHeight*8, 1.0f };
	//Sphere wallL{ Vec3f{-screenWidth, screenHeight / 2,  0}, { 0, 1 , 1 }, screenHeight * 8, 0.25f };
	//Sphere wallB{ Vec3i{900, 0,  300}, { 0, 1 , 1 }, 75.0, 0.25f };
	//Sphere wallT{ Vec3i{900, 0,  300}, { 0, 1 , 1 }, 75.0, 0.25f };
	//Sphere wallBa{ Vec3i{900, 0,  300}, { 0, 1 , 1 }, 75.0, 0.25f };


	Sphere sphereList[] = { orb1, orb2, orb3 };



	
	Light sun{ {610.f, 350.0f, 350.0f}, 750000000.0f };
	Mat image = Mat::zeros( screenWidth, screenHeight, CV_8UC3 );


	for (int indexX = 0; indexX < screenWidth; ++indexX)
	{
		for (int indexY = 0; indexY < screenHeight; ++indexY)
		{
			Ray movingRay{ Vec3i{indexX, indexY, 0 }, Vec3f{0, 0, 1} };
			

			for (Sphere &orb : sphereList) {
				float result1 = orb.intersection(movingRay);
				

				if (result1 > 0.0f) {
					Vec3f x = movingRay.origin + movingRay.direction * result1;
					Vec3f directionLampe = sun.originOfLight - x;
					float d2 = (directionLampe.dot(directionLampe));
					Vec3f w0 = directionLampe / sqrt(d2);

					Ray lightRay{x, d2};



					bool obstruction = false;


					for (Sphere& orb2 : sphereList) {


						Ray lightRay{ sun.originOfLight, -w0 };
						float intersectionDis = pow(orb2.intersection(lightRay), 2);
						if (intersectionDis > 0.f)
							if (0.99999*d2 > intersectionDis) {
								obstruction = true;
							}
					}

					
				if (!obstruction) {




					Vec3f normal = x - orb.originOfSphere;
					double normOfNormal = norm(normal, NORM_L2);
					normal = normal / normOfNormal;
					float f = (w0.dot(normal) / 3.14159265358979323846);





					float colorValue = (sun.brightness * f * orb.albedo) / d2;



					if (colorValue > 255)
						colorValue = 255;
					if (colorValue < 0)
						colorValue = 0;
					uchar color = colorValue;
					image.at<Vec3b>(Point(indexX, indexY)) = color * orb.vecteurColor;
				}
		
				}

				

			}
		};
	};
	imshow("sphere.png", image);
	waitKey(0);
	return 0;


};