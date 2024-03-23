# Welcome!
This repo contains 2 separate projects written in C++. Follow along for more information!  


# Part 1

# School_managament_system
School management system. Simple console program written in C++.

## Description
This project is a console-based school management system implemented in C++. It was created as a means to gain knowledge and practice C++ programming.


## Installation and Usage
To install and run this project, you will need a C++ compiler. You can use any text editor or IDE of your choice.
This project is a console application. After running the compiled file, you will be presented with a menu of options. Follow the prompts to use the application.

## Contributing
Contributions are welcome. Please feel free to fork the project and create a pull request with your changes.

## Demo

https://github.com/MikheilKvizhinadze2001/School_managament_system/assets/85734592/83696060-d657-4548-bb0e-021b3aeeb751


# Part 2
# Route estimation using API

### Task description:
Pre-defined start point (GPS coordinate: 44.07698, -103.21505)  and destination (GPS coordinate: 43.665134772619126, -116.73921890223438).
Ego vehicle speed - 50kph.
Along the route we have 1 static circle 'C' (center point: 42.55029319411949, -108.10396510030051, radius:200km)
and 2 moving circles, A1 and A2 (radius: 400km for both)
Moving center point of A1: 44.26279167885404, -105.52274156011913
Speed of center point of A1: 70kph
Speed of center point of A2: 5kph
Both circles follow heading of ego vehicle.

Also we define 3 functions: f_c, f_1 and f_2 like this:

If distance d to C is within radius of area C, we evaluate function f_c = 200/(4*Pi*d/1000000)^3

If distance d to A1 is within radius of area A1, we evaluate function f_1: 100/(4*Pi*d/1000000)^2

If distance d to A2 is within radius of area A2, we evaluate function f_2: 300/(4*Pi*d/1000000)^2
  
Furthermore, we continuously store values of Continuously store ego location, function values, and distances to: destination,
center of C, center of A1, center of A2.
And along the route we check if any function result f_c, f_1, f_2 is at least for 3s of travel time 100% higher than any other function results:
If such events occur, we store corresponding ego location, function values, and distances to: destination,
center of C, center of A1, center of A2.

To reproduce the code, you need to obtain Google maps API key and use it in the code.
Results are descried in PPT file.






## License
This project is licensed under the MIT License.
