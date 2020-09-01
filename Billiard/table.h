
class Table
{
	Ball balls[16];    // 16 balls
	int time;          // internal time for physics simulation
	double stickAngle; // current stick angle

public:
	Table();
	void setStickAngle(double angle);
	void reset();
	int visibleNumber();
	bool moving();
	void shoot();
	void update(int currentTime);
	void draw();
};
