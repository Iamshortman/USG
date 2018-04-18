#ifndef ROTATION_HPP
#define ROTATION_HPP

typedef uint8_t Direction;
const Direction d_forward = 0;
const Direction d_backward = 1;
const Direction d_upward = 2;
const Direction d_downward = 3;
const Direction d_leftward = 4;
const Direction d_rightward = 5;

const vector3B Directions3B[] =
{
	vector3B(0, 0, 1),
	vector3B(0, 0, -1),
	vector3B(0, 1, 0),
	vector3B(0, -1, 0),
	vector3B(1, 0, 0),
	vector3B(-1, 0, 0),
};

//This number flips the direction enum to the opposite one
inline Direction flipDirection(Direction direction)
{
	if (direction % 2)
	{
		//If number is odd sub 1
		return direction - 1;
	}
	else
	{
		//If number is even (or zero) add 1
		return direction + 1;
	}
}

#endif // ROTATION_HPP
