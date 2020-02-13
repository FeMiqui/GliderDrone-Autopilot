#include <math.h>
#include <stdio.h>
#include <time.h>

static double[3] current_location;

// takes in two double arrays with three elements and finds the distance betwen the two
double getDistance_total(double p0[], double p1[])
{	
	// calculates the 
	double x_dist = p1[0] - p0[0];
	double y_dist = p1[1] - p0[1];
	double z_dist = p1[2] - p0[2];
	
	// squares the cartesian distances
	double x_dist_squared = x_dist * x_dist;
	double y_dist_squared = y_dist * y_dist;
	double z_dist_squared = z_dist * z_dist;
	
	// takes the square root of the sum of the distances
	double total_dist = pow( x_dist_squared + y_dist_squared + z_dist_squared, 0.5);	

	return total_dist;
}

// takes in two double arrays with three elements and returns a double array[3] with the respective velocities
double[] getDistance(double p0[], double p1[])
{	
	// puts the current velocities into an array and returns it
	double calculated_velocity[3];
	calculated_velocity[0] = p1[0] - p0[0];
	calculated_velocity[1] = p1[1] - p0[1];	
	calculated_velocity[2] = p1[2] - p0[2];	

	return calculated_velocity;
}

double[] getLocation()
{
	// creates and returns a random location
	double new_location[] = {rand()%6, rand()%6, rand()%6};
	
	return new_location;
}

double getVelocity()
{
	clock_t timer_start_time, timer_end_time;
	double location_0[3], location_1[3];
	
	// grabs our current location
	location_0 = getLocation();
	
	// starts a timer and grabs our new current location (since we are moving)
	timer_start_time = clock();
	location_1 = getLocation();
	timer_end_time = clock();

	
	
	// debug printing
	printf("location_0[3] == { %f, %f, %f }\n", location_0[0], location_0[1], location_0[2]);
	printf("location_1[3] == { %f, %f, %f }\n", location_1[0], location_1[1], location_1[2]);
	
	// calculates the values for found_velocity
	double found_velocity[] = getDistance(location_0, location_1);
	printf("found_velocity[3] in m/s == { %f, %f, %f }\n", found_velocity[0], found_velocity[1], found_velocity[2]);

	return getDistance_total(found_velocity, {0,0,0});
	
	
}

// main here just prints out test statements and then returns 0
int main(int argc, char *argv[])
{

	// tests out getVelocity
//	double calculated_distance = getDistance_total(t0, t1);
	// prints the result
	//printf("The Distance: %f\n", calculated_distance);
	
	// tests out getting velocity
	printf("Our Velocity: %f\n", getVelocity());
	
	// done testing without error, return 0
	return 0;
}