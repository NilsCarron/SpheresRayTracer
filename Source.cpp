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


class Box3
{
	Vec3d bounds[2];
	std::vector<Sphere>sphereList;


public: Box3(Vec3d& vmin, const Vec3d& vmax, std::vector<Sphere>sphereListTotal)
{
	bounds[0] = vmin;
	bounds[1] = vmax;

	double Xmin = vmin[0];
	double Ymin = vmin[1];
	double Zmin = vmin[2];

	double Xmax = vmax[0];
	double Ymax = vmax[1];
	double Zmax = vmax[2];






	for (Sphere& orb : sphereListTotal) {
		if (orb.originOfSphere[0] < vmax[0] & orb.originOfSphere[1] < vmax[1] & orb.originOfSphere[2] < vmax[2]) &(orb.originOfSphere[0] > vmin[0] & orb.originOfSphere[1] > vmin[1] & orb.originOfSphere[2] > vmin[2]) {
			sphereList.push_back(orb);
		}
	}

	for (Sphere& orb : sphereList) {
		if (Xmin > orb.originOfSphere[0] - orb.rayon) {
			Xmin = (orb.originOfSphere[0] - orb.rayon);
		}
		if (Ymin > orb.originOfSphere[1] - orb.rayon) {
			Ymin = (orb.originOfSphere[1] - orb.rayon);
		}
		if (Zmin > orb.originOfSphere[2] - orb.rayon) {
			Zmin = (orb.originOfSphere[2] - orb.rayon);
		}

		if (Xmax < orb.originOfSphere[0] + orb.rayon) {
			Xmax = (orb.originOfSphere[0] + orb.rayon);
		}
		if (Ymax < orb.originOfSphere[1] + orb.rayon) {
			Ymax = (orb.originOfSphere[1] + orb.rayon);
		}
		if (Zmax < orb.originOfSphere[2] + orb.rayon) {
			Zmax = (orb.originOfSphere[2] + orb.rayon);
		}


	}


	bounds[0] = { Xmin, Ymin, Zmin };
	bounds[0] = { Xmax, Ymax, Zmax };



};
		





	bool intersect(const Ray& r)
	{
		Vec3d& min = bounds[0];
		Vec3d& max = bounds[1];

		float tmin = (min[0] - r.origin[0]) / r.direction[0];
		float tmax = (max[0] - r.origin[0]) / r.direction[0];

		if (tmin > tmax) {
			Vec3f invdir;
			invdir[0] = 1 / r.direction[0];
			invdir[1] = 1 / r.direction[1];

			invdir[2] = 1 / r.direction[2];

			if (r.direction[0] >= 0) {
				tmin = (min[0] - r.origin[0]) / r.direction[0];
				tmax = (max[0] - r.origin[0]) / r.direction[0];
			}
			else {
				tmin = (max[0] - r.origin[0]) / r.direction[0];
				tmax = (min[0] - r.origin[0]) / r.direction[0];
			}

		}
		float tymin = (min[1] - r.origin[1]) / r.direction[1];
		float tymax = (max[1] - r.origin[1]) / r.direction[1];

		if (tymin > tymax) {
			Vec3f invdir;
			invdir[0] = 1 / r.direction[0];
			invdir[1] = 1 / r.direction[1];

			invdir[2] = 1 / r.direction[2];

			if (r.direction[0] >= 0) {
				tmin = (min[0] - r.origin[0]) / invdir[0];
				tmax = (max[0] - r.origin[0]) / invdir[0];
			}
			else {
				tmin = (max[0] - r.origin[0]) / invdir[0];
				tmax = (min[0] - r.origin[0]) / invdir[0];
			}

		}

		if ((tmin > tymax) || (tymin > tmax))
			return false;

		if (tymin > tmin)
			tmin = tymin;

		if (tymax < tmax)
			tmax = tymax;

		float tzmin = (min[2] - r.origin[2]) / r.direction[2];
		float tzmax = (max[2] - r.origin[2]) / r.direction[2];

		if (tzmin > tzmax) {
			if (r.direction[0] >= 0) {
				Vec3f invdir;
				invdir[0] = 1 / r.direction[0];
				invdir[1] = 1 / r.direction[1];

				invdir[2] = 1 / r.direction[2];

				tmin = (min[0] - r.origin[0]) / r.direction[0];
				tmax = (max[0] - r.origin[0]) / r.direction[0];
			}
			else {
				tmin = (max[0] - r.origin[0]) / r.direction[0];
				tmax = (min[0] - r.origin[0]) / r.direction[0];
			}

		}

		if ((tmin > tzmax) || (tzmin > tmax))
			return false;

		if (tzmin > tmin)
			tmin = tzmin;

		if (tzmax < tmax)
			tmax = tzmax;

		return true;
	}

};



int main()
{
	float screenWidth = 1000.f;
	float screenHeight = 1000.f;
	Sphere orb1{ Vec3f{365.f, 380.0f,  250.0f}, { 1, 1 , 1 }, 15.0f, 1.0f };
	Sphere orb2{ Vec3f{350.f, 360.0f, 300.0f}, { 1, 0 , 1 }, 50.0f, 0.50f };
	Sphere orb3{ Vec3f{900.f, 0.0f,  100}, { 0, 1 , 1 }, 75.0, 0.25f };

	//Sphere wallBa{ Vec3f{screenWidth/2, screenHeight /2, screenWidth*10}, { 1, 1 , 1 }, screenHeight*8, 1.0f };
	//Sphere wallL{ Vec3f{-screenWidth, screenHeight / 2,  0}, { 0, 1 , 1 }, screenHeight * 8, 0.25f };
	//Sphere wallB{ Vec3i{900, 0,  300}, { 0, 1 , 1 }, 75.0, 0.25f };
	//Sphere wallT{ Vec3i{900, 0,  300}, { 0, 1 , 1 }, 75.0, 0.25f };
	//Sphere wallBa{ Vec3i{900, 0,  300}, { 0, 1 , 1 }, 75.0, 0.25f };


	std::vector<Sphere>sphereList  { orb1, orb2, orb3 };



	
	Light sun{ {610.f, 300.0f, 100.0f}, 750000000.0f };
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
							if (0.9999*d2 > intersectionDis) {
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