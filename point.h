class Point
{
private: 
	
public:
    float x, y, z;
	Point(){  this->x = x;
    }
    Point(float x, float y, float z){
    	this->x = x;
    	this->y = y;
    	this->z = z;
    };
    // getters
    float getX(){return x;}
    float getY(){return y;};
	float getZ(){return z;};
    void setVs(float x, float y, float z){
        this->x = x;
        this->y = y;
        this->z = z;}

	
};