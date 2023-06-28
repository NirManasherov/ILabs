#include <string>	//string
#include <utility>	//pair
#include <iostream> //cout


class Shapes
{//virtual interface class
public: 

	explicit Shapes(std::string shape_name_, std::pair<size_t, size_t> point_);
	virtual ~Shapes(){};
	
	//copy constructor and assignment operator are disabled
	
	virtual void draw() const; 
	void move(size_t right_offset_);

private: 

	const std::string shape_name;
	std::pair<size_t, size_t> point;
	
	Shapes(const Shapes& o_); //disables
	Shapes& operator=(const Shapes& o_); //disabled
};

class Line: public Shapes
{
public:
	explicit Line(size_t length_, std::pair<size_t, size_t> point_);
										   
	~Line(){}; 
private:
	size_t length; 
};

class Circle: public Shapes
{
public:
	explicit Circle(size_t diameter_, std::pair<size_t, size_t> point_);
											   
	~Circle(){}; 
private:
	size_t diameter; 
};

class Rectangle: public Shapes
{
public:
	explicit Rectangle(size_t length_a_, size_t length_b_, 
	   										  std::pair<size_t, size_t> point_);
											   
	~Rectangle(){}; 
private:
	size_t length_a;
	size_t length_b; 
};

class Square: public Shapes
{
public:
	explicit Square(size_t length_, std::pair<size_t, size_t> point_);
											   
	~Square(){}; 
private:
	size_t length; 
};

/////////////////////////////////////////IMP////////////////////////////////////

Shapes::Shapes(std::string shape_name_, std::pair<size_t, size_t> point_):
                                       shape_name(shape_name_), point(point_) {}


void Shapes::draw() const
{
    std::cout << "shape name: " <<  shape_name << std::endl; 
    std::cout << "center point: x: " << point.first << "y: " << point.second <<
                                                                      std::endl; 
}

void Shapes::move(size_t right_offset_)
{
    point.first += right_offset_; 
}

Line::Line(size_t length_, std::pair<size_t, size_t> point_):
                                      Shapes("Line", point_), length(length_) {}

Circle::Circle(size_t diameter_, std::pair<size_t, size_t> point_): 
                                Shapes("Circle", point_), diameter(diameter_) {}
                                
Rectangle::Rectangle(size_t length_a_, size_t length_b_, 
std::pair<size_t, size_t> point_):Shapes("Rectangle", point_), 
                                     length_a(length_a_), length_b(length_b_) {}
                                     
Square::Square(size_t length_, std::pair<size_t, size_t> point_): 
                                    Shapes("Square", point_), length(length_) {}


int main()
{
    return 0; 
}
