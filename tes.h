#include "rtweekend.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

int main() {
	point3 center = vec3(0.0, 0.5, -1);
	double big_radius = 0.9;

	double gumball_radius = 0.2;

	
	for (double x = center.x() - big_radius; x <= center.x() + big_radius; x += gumball_radius) {
		for (double y = center.y() - big_radius; y <= center.y() + big_radius; y += gumball_radius) {
			for (double z = center.z() - big_radius; z <= center.z() + big_radius; z += gumball_radius) {
				if (big_radius >= (point3(x, y, z) - center).length() + gumball_radius) {
					std::cout << (std::cout, center) << std::endl;
				}
			}
		}
	}
}