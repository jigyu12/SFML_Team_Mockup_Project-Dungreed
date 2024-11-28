#pragma once

#define ABS(X) ((X) < 0 ? -(X) : (X))
#define EPSILON 0.000001

enum class Languages
{
	Korean,
	English,
	Japanese,
	Count,
};

enum class SortingLayers
{
	Background,
	Foreground,
	Default,
	UI,
};

enum class SceneIds
{
	None = -1,
	Game,
	Dev1,
	Dev2,
	Dev3,
	MapEdit,
	Count,
};

enum class Origins
{
	// T M B
	// L C R
	TL, TC, TR,
	ML, MC, MR,
	BL, BC, BR,
	Custom,
};

enum class Sides
{
	Left,
	Right,
	None,
};

struct CollisionState
{
	bool Up = false;
	bool Down = false;
	bool Left = false;
	bool Right = false;
};