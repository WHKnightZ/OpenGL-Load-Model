
// quadric object for GLU functions
extern GLUquadricObj* quadricObject;

void drawSphere(double radius);
void drawCuboid(double width, double height, double depth);
void drawCylinder(double baseRadius, double topRadius, double height);
void setShininess(double s);

class Ball
{
	double r, g, b; // colour
	double x, z;    // position
	double sx, sz;  // speed
	bool visible;   // state
	double radius;	// radius

public:
	Ball();
	void setColour(double r0, double g0, double b0);
	void setPosition(double x0, double z0);
	double getX();
	double getZ();
	void setSpeed(double sx0, double sz0);
	void setVisible(bool v);
	bool getVisible();
	double getRadius();
	bool moving();
	double distanceTo(double px, double pz);
	void collideHoles();
	void collideCushions();
	void collideBall(Ball& other);
	void updateSpeed(double timeStep);
	void updatePosition(double timeStep);
	void draw();
};
