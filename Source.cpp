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
	float rayon;
	float albedo;



	float intersection(Ray& r) {

		Vec3i op = originOfSphere - r.origin;        // Solve t^2*d.d + 2*t*(o-p).d + (o-p).(o-p)-R^2 = 0
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




	Sphere orb{ Vec3i{750, 750,  500}, 100.0, 0.50f };

	int screenWidth = 1000;
	int screenHeight = 1000;
	Light sun{ {600.f, 350.0f, 350.0f}, 70500000.0f };
	Mat image = Mat::zeros( screenWidth, screenHeight, CV_8UC3 );


	for (int indexX = 0; indexX < screenWidth; ++indexX)
	{
		for (int indexY = 0; indexY < screenHeight; ++indexY)
		{
			Ray movingRay{ Vec3i{indexX, indexY, 0 }, Vec3i{0, 0, 1} };
			


			float result1 = orb.intersection(movingRay);
			if (result1 > 0.0f) {

			 Vec3f x = movingRay.origin + movingRay.direction * result1;
			 Vec3f directionLampe = sun.originOfLight - x;

			 float d2 = (directionLampe.dot(directionLampe));
			 Vec3f w0 = directionLampe / sqrt(d2);
			 
			 
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
			image.at<Vec3b>(Point(indexX, indexY)) = { color, 0 , color };
				 
			 }
	

			

		};
	};
	imshow("sphere.png", image);
	waitKey(0);
	return 0;


};