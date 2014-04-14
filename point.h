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
    Point average(Point m){
        float i,j,k;
        i = (this->x * m.getX())/2;
        j = (this->y * m.getY())/2;
        k = (this->z * m.getZ())/2;

        Point np(i, j,k);
        return np;
    }
    bool isNull(){
        if (this->x == -1 && this->y == -1 && this->z == -1){
            return true;
        }
        else
            return false;
    }

	
};