#include <iostream>
#include <cmath>
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include <deque>
#include <fstream>

/*
 * Author: Mikheil Kvizhinadze
 * Date: 03/11/2024
 *
 * Description:
 * This program interfaces with the Google Directions API to retrieve route information and subsequently processes
 * this data to perform calculations and checks as outlined in the exercise prompt.
 * Debugging statements and dummy inputs were extensively employed to evaluate the performance of the functions.
 * Although not explicitly required by the assignment, these debugging elements were retained as they provide
 * significant insights into the code and its functionality.
 * Further details are provided below.
 */

// The API key for the Google Directions API
const std::string API_KEY = "AIzaSyDexF3UcqhLCDW0dR-0_Z4Yyfg89ycIku0";

// Callback function for libcurl to write the response data into a string
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t newLength = size * nmemb;
    try {
        // Append the data to the string
        s->append(static_cast<char*>(contents), newLength);
    }
    catch(const std::bad_alloc& e) {
        // If memory allocation fails, print an error message and return 0 to signal an error to libcurl
        std::cerr << "Error: Unable to allocate memory in WriteCallback: " << e.what() << std::endl;
        return 0;
    }
    return newLength;
}

// Function to make an HTTP request to the Google Directions API and return the response data
std::string getRouteInfo() {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl) {
        // Set up the HTTP request
        std::string url = "https://maps.googleapis.com/maps/api/directions/json?origin=44.07698,-103.21505&destination=43.665134772619126,-116.73921890223438&key=" + API_KEY;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

        // Perform the HTTP request
        res = curl_easy_perform(curl);

        // Check for errors
        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        // Cleanup
        curl_easy_cleanup(curl);
    } else {
        std::cerr << "Error: Unable to initialize curl" << std::endl;
    }
    curl_global_cleanup();

    // Return the response data
    return readBuffer;
}





// Define the GPS coordinates
const double DEST_LAT = 43.665134772619126;
const double DEST_LNG = -116.73921890223438;


// Define the areas
const double AREA_C_LAT = 42.55029319411949;
const double AREA_C_LNG = -108.10396510030051;
const double AREA_C_RADIUS = 200;
const double AREA_A1_RADIUS = 400;
const double AREA_A2_RADIUS = 400;
const double EARTH_RADIUS_KM = 6371.0;
const double EARTH_CIRCUMFERENCE = 40075.0;
// Velocities
// Converting to seconds because below we will be working with seconds
const double EGO_VELOCITY = 50.0 / 3600.0; // km/second
const double A1_VELOCITY = 70.0 / 3600.0; // km/second
const double A2_VELOCITY = 5.0 / 3600.0;   // km/second
const double TIME_INTERVAL = 1.0; // Update every second

double degreesToRadians(double degrees) {
    return degrees * M_PI / 180.0;
}

double radiansToDegrees(double radians) {
    return radians * (180.0 / M_PI);
}

// Calculate distance function, to find distance between two points
double calculateDistance(double lat1, double lng1, double lat2, double lng2) {
    if (lat1 < -90 or lat1 > 90 or lat2 < -90 or lat2 > 90 or
        lng1 < -180 or lng1 > 180 or lng2 < -180 or lng2 > 180) {
        throw std::invalid_argument("Invalid latitude or longitude");
    }

    // distance between latitudes and longitudes
    double dLat = degreesToRadians(lat2 - lat1);
    double dLng = degreesToRadians(lng2 - lng1);

    // convert to radians
    lat1 = degreesToRadians(lat1);
    lat2 = degreesToRadians(lat2);

    // apply formulae
    double a = pow(sin(dLat / 2), 2) +
               pow(sin(dLng / 2), 2) *
               cos(lat1) * cos(lat2);
    double c = 2 * asin(sqrt(a));
    return EARTH_RADIUS_KM * c;
}



// Function to check if a point is within a certain radius of another point

bool isWithinRadius(double lat1, double lng1, double lat2, double lng2, double radius) {
    if (radius < 0) {
        throw std::invalid_argument("Radius cannot be negative");
    }

    double distance = calculateDistance(lat1, lng1, lat2, lng2);
    return distance <= radius;
}

/* Function to calculate heading of ego. It can be calculated as,

β = atan2(X,Y), where, X and Y are two quantities and can be calculated as:

X = cos θb * sin ∆L

Y = cos θa * sin θb – sin θa * cos θb * cos ∆L

 */
double calculateHeading(double lat1, double lng1, double lat2, double lng2) {
    if (lat1 < -90 or lat1 > 90 or lat2 < -90 or lat2 > 90 or
        lng1 < -180 or lng1 > 180 or lng2 < -180 or lng2 > 180) {
        throw std::invalid_argument("Invalid latitude or longitude");
    }

    double latRad1 = degreesToRadians(lat1);
    double latRad2 = degreesToRadians(lat2);
    double deltaLng = degreesToRadians(lng2 - lng1);

    double y = std::sin(deltaLng) * std::cos(latRad2);
    double x = std::cos(latRad1) * std::sin(latRad2) -
               std::sin(latRad1) * std::cos(latRad2) * std::cos(deltaLng);
    double heading = std::atan2(y, x);

    // Convert the heading to degrees
    heading = heading * 180.0 / M_PI;

    // Make sure the heading is between 0 and 360 degrees
    if (heading < 0) {
        heading += 360;
    }

    return heading;
}



// Function to evaluate the functions f_c, f_1, and f_2
double evaluateFunction_f1_f2(double distance, double constant) {
    return constant / pow(4 * M_PI * distance / 1000000, 2);
}
double evaluateFunction_fc(double distance){
    return 200 / pow(4 * M_PI * distance / 1000000, 3);
}

/*
 * Function to update the position of a point
 *
 */
void updatePosition(double& lat, double& lng, double distance, double heading) {
    double d = distance / EARTH_RADIUS_KM; // Convert distance to angular distance in radians
    double headingRadians = degreesToRadians(heading);

    lat = radiansToDegrees(asin(sin(degreesToRadians(lat)) * cos(d) + cos(degreesToRadians(lat)) * sin(d) * cos(headingRadians)));
    lng += radiansToDegrees(atan2(sin(headingRadians) * sin(d) * cos(degreesToRadians(lat)), cos(d) - sin(degreesToRadians(lat)) * sin(degreesToRadians(lat))));
}

    /*
     * Function to get comparison value to check whether it satisfies condition,
     * described in the exercise.
     *
    */
double calculateComparison_value (const double &a,const double &b){
    if (std::min(a,b) > 0)
        return 2 * std::min(a,b);


    // max of a and b might also be zero, so such cases will be handled below
    return 2 * std::max(a,b);
}




int main() {

/*
     Before diving into main implementation
     let’s perform some manual checks using dummy inputs, whose outputs are known and predictable.
     This is needed to verify the correct functionality of functions defined above.

      Lets us take an example to calculate heading between the two different points with the formula:

      Kansas City: 39.099912, -94.581213
      St Louis: 38.627089, -90.200203
      So X and Y can be calculated as,

      X =  cos(38.627089) * sin(4.38101)

      X  = 0.05967668696 And
      Y = cos(39.099912) * sin(38.627089) – sin(39.099912) * cos(38.627089) * cos(4.38101)

      Y = 0.77604737571 * 0.62424902378 – 0.6306746155 * 0.78122541965 * 0.99707812506

      Y = -0.00681261948

      So as, β = atan2(X,Y) = atan2(0.05967668696, -0.00681261948) = 1.684463062558 radians

      convert it into degrees ---->>> β = 96.51°
*/

    double lat1 = 39.099912;
    double lng1 = -94.58121;

    double lat2 = 38.627089;
    double lng2 = -90.200203;


    double heading_test = calculateHeading(lat1, lng1, lat2, lng2);

    std::cout<< "Calculations on dummy variables for testing purposes: "<<std::endl;
    std::cout<< "Calculated Heading: " << heading_test << std::endl;
    // After running this code, output shows 'Calculated Heading: 96.5126', as expected.


    double calculated_distance_manual = calculateDistance(39.099912, -94.58121,38.627089,  -90.200203);
    std::cout<< "Calculated distance: " << calculated_distance_manual << std::endl;
    // Distance between Kansas City and St Louis is ~383km
    // And output shows 382.9 as expected



    // Finally, let us check the functionality of updatePosition function
    // Since we already know start coordinate of Kansas city, and heading from Kansas city to
    // St Louis is ~96.51°, and distance is 383. After we use those values we should get
    // coordinates of St Louis, which is 38.627089, -90.200203

    updatePosition(lat1, lng1, 383, 96.51);

    std::cout << "Updated position: (" << lat1 << ", " << lng1 << ")" << std::endl;
    // And, the output is 'Updated position: (38.6271, -90.1989)', verifying that crucial functions are implemented correctly

    std::cout<< "End of testing calculations."<<std::endl;


    std::cout<<"\n\n\n\n";

    // Next, let us to the implementation of main body, where we follow the route, update coordinates,
    // calculate function values and store relevant values.

    // Get the route info from the Google Directions API
    std::string routeInfo = getRouteInfo();

    // Parse the route info
    nlohmann::json routeJson = nlohmann::json::parse(routeInfo);

    // Variable to track elapsed time
    double elapsedTime = 0.0;

    // Define the positions ego, A1 and A2
    double lat = 44.07698; // Ego lat
    double lng = -103.21505; // Ego lng
    double AREA_A1_LAT = 44.26279167885404;
    double AREA_A1_LNG = -105.52274156011913;
    double AREA_A2_LAT = 43.665134772619126;
    double AREA_A2_LNG = -116.73921890223438;


    // Define vectors to store continuous values
    std::vector<double> ego_latitudes, ego_longitudes;
    std::vector<double> function_values_c, function_values_1, function_values_2;
    std::vector<double> distances_to_destination, distances_to_c, distances_to_a1, distances_to_a2;
    std::vector<double> a1_latitudes, a1_longitudes, a2_latitudes, a2_longitudes;

    // Define vectors to store special values (which satisfy conditions described in the exercise)
    std::vector<double> special_ego_latitudes, special_ego_longitudes;
    std::vector<double> special_function_values_c, special_function_values_1, special_function_values_2;
    std::vector<double> special_distances_to_destination, special_distances_to_c, special_distances_to_a1, special_distances_to_a2;





    /* Before directly calculating function values and store relevant values, we first
     * divide each step into 1-second intervals, because each step is much greater than 1 second
     * and this division would allow get much more accurate results, that is
     * if we only checked conditions at the start and end of a route step (much greater than 3 sec)
     * we would most likely miss the 3-second window where the condition is fulfilled.
     * Furthermore, we need to take ego's constant velocity into account, but the Google directions API's primary
     * function is to provide optimized routes and turn-by-turn instructions.
     * It factors in real-world conditions like traffic, road types, and typical speed limits to estimate travel times
     * So, To adjust for this, we first converted the ego’s velocity from km/sec to degrees/sec by dividing
     * the velocity by the Earth’s circumference
     * and multiplying by 360. This gave us the speed of the ego in terms of degrees of latitude per second. We then
     * calculated the change in latitude (dy) and longitude (dx) per second based on the ego’s velocity and the heading of the current step.
     * The heading was calculated using a function that determined the direction from the current position to the end of the current step.
     * Finally, we updated the ego’s latitude and longitude by adding dy and dx, respectively, to the current latitude and longitude
     * for each second of the step’s duration. This effectively moved the ego along the step at the specified constant speed.
     * By making these modifications, we were able to adjust the route based on the pre-defined constant velocity of the ego.
     *
     *
     */


    // Define ego heading and conditionStartTime
    double EGO_HEADING;
    double conditionStartTime = -1;
    // Iterate through the route steps
    for (const auto &step: routeJson["routes"][0]["legs"][0]["steps"]) {
        double time_step = step["duration"]["value"].get<double>();
        double end_lat = step["end_location"]["lat"];
        double end_lng = step["end_location"]["lng"];

        // Calculate the heading of the current step
        EGO_HEADING = calculateHeading(lat, lng, end_lat, end_lng);



        // Convert the ego's velocity to degrees/sec
        double ego_velocity_deg_per_sec = EGO_VELOCITY / EARTH_CIRCUMFERENCE * 360;

        // Calculate the change in latitude and longitude per second based on the ego's velocity and the heading
        double dy = ego_velocity_deg_per_sec * cos(EGO_HEADING) * TIME_INTERVAL;
        double dx = ego_velocity_deg_per_sec * sin(EGO_HEADING) * TIME_INTERVAL;

        double step_distance = step["distance"]["value"].get<double>();

        // Convert the distance to kilometers
        step_distance /= 1000.0;

        // Calculate the time it would take to traverse the step at the ego's speed
        double step_duration = step_distance / EGO_VELOCITY;

        std::cout<<"Time step: "<<time_step<<std::endl;

        for (double t = 0; t < step_duration; t += TIME_INTERVAL) {

            // Now we repeat procedures below for the whole step_duration
            // We update variables, calculate functions and store respective values
            // each second

            lat += dy;  // Update latitude
            lng += dx;  // Update longitude

            double distanceTraveledByEgo = EGO_VELOCITY * TIME_INTERVAL; // in km

            // Calculate heading, update A1, A2 positions, calculate distances, evaluate functions f_c, f_1, f_2
            EGO_HEADING = calculateHeading(lat, lng, end_lat, end_lng);

            updatePosition(lat, lng, distanceTraveledByEgo, EGO_HEADING);
            updatePosition(AREA_A2_LAT, AREA_A2_LNG, A2_VELOCITY * TIME_INTERVAL, EGO_HEADING);
            updatePosition(AREA_A1_LAT, AREA_A1_LNG, A1_VELOCITY * TIME_INTERVAL, EGO_HEADING);




            bool withinAreaC = isWithinRadius(lat, lng, AREA_C_LAT, AREA_C_LNG, AREA_C_RADIUS);
            bool withinAreaA1 = isWithinRadius(AREA_A1_LAT, AREA_A1_LNG, lat, lng, AREA_A1_RADIUS);
            bool withinAreaA2 = isWithinRadius(AREA_A2_LAT, AREA_A2_LNG, lat, lng, AREA_A2_RADIUS);

            // Values of fc,f1 and f2 are assumed to be zero, if the condition is not met
            double f_c = withinAreaC ? evaluateFunction_fc(calculateDistance(lat, lng, AREA_C_LAT, AREA_C_LNG)) : 0;
            double f_1 = withinAreaA1 ? evaluateFunction_f1_f2(calculateDistance(AREA_A1_LAT, AREA_A1_LNG, lat, lng),
                                                               100) : 0;
            double f_2 = withinAreaA2 ? evaluateFunction_f1_f2(calculateDistance(AREA_A2_LAT, AREA_A2_LNG, lat, lng),
                                                               300) : 0;

            // Continuously store the ego location, function values, and distances
            // Since TIME_INTERVAL is 1 sec, we are effectively storing values each second
            ego_latitudes.push_back(lat);
            ego_longitudes.push_back(lng);
            function_values_c.push_back(f_c);
            function_values_1.push_back(f_1);
            function_values_2.push_back(f_2);
            a1_latitudes.push_back(AREA_A1_LAT);
            a1_longitudes.push_back(AREA_A1_LNG);
            a2_latitudes.push_back(AREA_A2_LAT);
            a2_longitudes.push_back(AREA_A2_LNG);
            distances_to_destination.push_back(calculateDistance(lat, lng, DEST_LAT, DEST_LNG));
            distances_to_c.push_back(calculateDistance(lat, lng, AREA_C_LAT, AREA_C_LNG));
            distances_to_a1.push_back(calculateDistance(lat, lng, AREA_A1_LAT, AREA_A1_LNG));
            distances_to_a2.push_back(calculateDistance(lat, lng, AREA_A2_LAT, AREA_A2_LNG));


            // Check the conditions
            double comparisonValue_for_fc = calculateComparison_value(f_1,f_2);
            double comparisonValue_for_f1 = calculateComparison_value(f_c,f_2);
            double comparisonValue_for_f2 = calculateComparison_value(f_c,f_1);

            /*
             * Booleans below will check if the value is greater than zero
             * if it is not, it is assumed that it should not be recorded,
             * because in such cases ego vehicle is outsize the respective circles
             * and value is no longer relevant. For example, if at some point fc > 0
             * and f1 and f2 are both zeros, it means that their values are not relevant
             * (ego vehicle is outsize those circles), and they will not be used for comparison
            */
            bool isFcValue_Relevant = comparisonValue_for_fc > 0;
            bool isF1Value_Relevant = comparisonValue_for_f1 > 0;
            bool isF2Value_Relevant = comparisonValue_for_f2 > 0;

            if ((f_c >= comparisonValue_for_fc and isFcValue_Relevant) or
                (f_1 >= comparisonValue_for_f1 and isF1Value_Relevant or
                (f_2 >= comparisonValue_for_f2) and isF2Value_Relevant)) {

                // If the condition is met, check if it's the first time it's met
                if (conditionStartTime < 0) {
                    conditionStartTime = elapsedTime;
                }
                // If the condition is met for at least 3 seconds
                if (elapsedTime - conditionStartTime >= 3) {
                    // Store the corresponding ego location, function values, and distances
                    special_ego_latitudes.push_back(lat);
                    special_ego_longitudes.push_back(lng);
                    special_function_values_c.push_back(f_c);
                    special_function_values_1.push_back(f_1);
                    special_function_values_2.push_back(f_2);
                    special_distances_to_destination.push_back(calculateDistance(lat, lng, DEST_LAT, DEST_LNG));
                    special_distances_to_c.push_back(calculateDistance(lat, lng, AREA_C_LAT, AREA_C_LNG));
                    special_distances_to_a1.push_back(calculateDistance(lat, lng, AREA_A1_LAT, AREA_A1_LNG));
                    special_distances_to_a2.push_back(calculateDistance(lat, lng, AREA_A2_LAT, AREA_A2_LNG));
                    // Reset conditionStartTime
                    conditionStartTime = elapsedTime;
                }
            }

            // If condition is not met anywhere in a 3 sec window, we reset time
            else{
                conditionStartTime = elapsedTime; // Update time if condition is not met
            }
            // Update the elapsed time
            elapsedTime += TIME_INTERVAL;


}

        // Printing start and end locations of ego
        std::cout << "Step start location: " << lat << ", " << lng << std::endl;
        std::cout << "Step end location: " << end_lat << ", " << end_lng << std::endl;


    }



        // Print the final positions of the ego, A1, and A2
        std::cout << "Final position of the ego: " << lat << ", " << lng << std::endl;
        std::cout << "Final position of A1: " << AREA_A1_LAT << ", " << AREA_A1_LNG << std::endl;
        std::cout << "Final position of A2: " << AREA_A2_LAT << ", " << AREA_A2_LNG << std::endl;

        // Print the total travel time
        std::cout << "Total travel time: " << elapsedTime / 3600 << " hours" << std::endl;

        std::cout << "Size of special vectors (how many times the condition was met):  " << special_distances_to_a1.size()<<std::endl;
        std::cout << "Size of normal vectors (whose values was stored values continuously): " << ego_latitudes.size()<<std::endl;


        // Creating csv file and writing relevant data.
        // This data will be used for visualizations, which will be done in Python (Jupyter notebook)
        std::ofstream file("output.csv");
        file
                << "ego_lat,ego_lng,a1_lat,a1_lng,a2_lat,a2_lng,f_c,f_1,f_2,dist_to_dest,dist_to_c,dist_to_a1,dist_to_a2\n";
        for (int i = 0; i < ego_latitudes.size(); i++) {
            file << ego_latitudes[i] << "," << ego_longitudes[i] << ","
                 << a1_latitudes[i] << "," << a1_longitudes[i] << ","
                 << a2_latitudes[i] << "," << a2_longitudes[i] << ","
                 << function_values_c[i] << "," << function_values_1[i] << "," << function_values_2[i] << ","
                 << distances_to_destination[i] << "," << distances_to_c[i] << ","
                 << distances_to_a1[i] << "," << distances_to_a2[i] << "\n";
        }
        file.close();



        // And, let us also create another csv file for special values:

        std::ofstream special_file("special_values.csv");
        special_file << "ego_lat,ego_lng,f_c,f_1,f_2,dist_to_dest,dist_to_c,dist_to_a1,dist_to_a2\n";
        for (int i =0; i <special_distances_to_a1.size();i++){
            special_file << special_ego_latitudes[i] << "," << special_ego_longitudes[i] << ","
                         << special_function_values_c[i] << "," << special_function_values_1[i] << ","
                         << special_function_values_2[i] << ","
                         << special_distances_to_destination[i] << "," << special_distances_to_c[i] << ","
                         << special_distances_to_a1[i]<< "," << special_distances_to_a2[i] << "\n";
        }

        special_file.close();

    // Thank you for your attention!

        return 0;
    }

