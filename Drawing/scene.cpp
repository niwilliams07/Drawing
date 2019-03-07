#include <iostream>
#include <algorithm>
#include <GL/glut.h>
#include "scene.h"



int TransformNode::count = 0;
map<int, TransformNode*> TransformNode::table;

TransformNode::TransformNode(TransformNode* p)
	:parent(p), shapeNode(NULL)
{
	selected = false;
	transform = new Matrix();
	identifier = count++;
	table[identifier] = this;
}

TransformNode::TransformNode(TransformNode* p, ShapeNode* s, Matrix* t)
	:parent(p), shapeNode(s), transform(t)
{
	selected = false;
	identifier = count++;
	table[identifier] = this;
}


TransformNode::~TransformNode()
{
	/*delete transform;
	delete shapeNode; 
	for (list<TransformNode*>::const_iterator iter = children.begin();
		iter != children.end();
		++iter) {
			delete *iter;
		
	}*/
	//This is not a necesary function and causes the program to crash when deleting copies. 
}


void TransformNode::translate(double deltaX, double deltaY)
{
	transform = Matrix::translation(deltaX, deltaY)->multiply(transform);
}

void TransformNode::rotate(double theta)
{
	transform = Matrix::rotation(theta)->multiply(transform);
}

void TransformNode::shear(double shearXY, double shearYX)
{
	transform = Matrix::shearing(shearXY, shearYX)->multiply(transform);
}

void TransformNode::scale(double scaleX, double scaleY)
{
	transform = Matrix::scaling(scaleX, scaleY)->multiply(transform);
}

void TransformNode::draw(bool displayHelpers) const
{
	gPush(transform);
	glPushName(identifier);
	bool highlight = getHighlight();

	if (selected) {
		setHighlight(selected);
	}

	if (shapeNode != NULL)shapeNode->draw();
	
	for (list<TransformNode*> ::const_iterator iter = children.begin();
		iter != children.end();
		++iter)
		(*iter)->draw(displayHelpers);

	setHighlight(highlight);
	glPopName();
	gPop();
}

TransformNode* TransformNode::getParent() const 
{ 
	return parent;
}

void TransformNode::setParent(TransformNode* p) 
{ 
	parent = p;
	this->parent->addChild(this);
}

void TransformNode::changeParent(TransformNode* newParent)
{
	this->getParent()->removeChild(this);
	this->setParent(newParent);

}

void TransformNode::groupObjects(set<TransformNode*>& groupMembers)
{
	TransformNode* temp = new TransformNode(this);
	for (set<TransformNode*> ::iterator iter = groupMembers.begin();
		iter != groupMembers.end();
		iter++) {
		(*temp).children.push_back(*iter);
	}
	this->addChild(temp);
}

Matrix* TransformNode::getTransform() const
{
   return transform;
}

TransformNode* TransformNode::clone() const
{
	/*ShapeNode* temp = shapeNode;
	Matrix* temp1 = transform;
	TransformNode* const copy = new TransformNode(NULL,temp,temp1);
	return copy;*/
	//couldn't figure out how to make copies where ~TranformNode would not crash when deleting a shapenode
	//fixed this by getting rid of the ~TransformNode function entirely. the program runs just fine without it
	TransformNode* const copy = new TransformNode(NULL, shapeNode, transform);
	return copy; 
}

void TransformNode::addChild(TransformNode* child)
{
	children.push_back(child);
}

void TransformNode::removeChild(TransformNode* child)
{
	children.remove(child);
}

TransformNode* TransformNode::firstChild() const
{
	if (!children.empty()) return children.front();
	else return NULL;
}

TransformNode* TransformNode::lastChild() const
{
	if (!children.empty()) return children.back();
	else return NULL;
}

TransformNode* TransformNode::nextChild(TransformNode* child) const
{
	list<TransformNode*> ::const_iterator iter = children.begin();
	while (*iter != child) {
		iter++;
	}
	if (iter != children.end()) {
		TransformNode* nextchild = *(++iter);
		return nextchild;
	}
	else return NULL;

}

TransformNode* TransformNode::previousChild(TransformNode* child) const
{
	list<TransformNode*>::const_iterator iter = children.begin();

	while (*iter != child) {
		iter++;
	}

	if (iter != children.begin()) {
		TransformNode* prevChild = *(--iter);
		return prevChild;
	}
	else return NULL;
}


void TransformNode::select() 
{
	selected = true;
}

void TransformNode::deSelect() 
{
	selected = false;
}

TransformNode* TransformNode::nodeLookup(int identifier)
{
   return table[identifier];
}


ShapeNode::ShapeNode(colorType c) 
	:color(c), transformNode(NULL)
{
}

void ShapeNode::setTransformNode(TransformNode* tn)
{
	transformNode = tn;
}

TransformNode* ShapeNode::getTransformNode()
{
	return transformNode;
}

Line::Line(double xx0, double yy0, double xx1, double yy1, colorType c)
	:ShapeNode(c), x0(xx0), y0(yy0), x1(xx1), y1(yy1)
{
}


ShapeNode* Line::clone() const
{
	return (Line*)(x0, y0, x1, y1, color);
}

void Line::draw() const
{
	setColor(color);
	drawLine(x0, y0, x1, y1);
}



Rectangle::Rectangle(double xx0, double yy0, double xx1, double yy1, colorType c)
	: ShapeNode(c), x0(xx0), x1(xx1), y0(yy0), y1(yy1)
{
}


ShapeNode* Rectangle::clone()  const
{
   return (Rectangle*)(x0, y0, x1, y1, color);
}

void Rectangle::draw() const
{
	setColor(color);
	drawRectangle(x0, y0, x1, y1);
}

Circle::Circle(double ccX, double ccY, double r, colorType c) 
	: ShapeNode(c), cX(ccX), cY(ccY), radius(r)

{}

ShapeNode* Circle::clone() const
{
	return (Circle*)(cX, cY, radius, color);
}

void Circle::draw() const
{
	setColor(color);
	drawCircle(cX, cY, radius);
}


Polygon::Polygon(const list<Vector*>& vs, colorType c) 
	: ShapeNode(c)
{
	for (list<Vector*> ::const_iterator iter = vs.begin();
		iter != vs.end();
		iter++)
		vertices.push_back(new Vector(**iter));
}

Polygon::~Polygon()
{
	for (list<Vector*> ::iterator iter = vertices.begin();
		iter != vertices.end();
		++iter)
		delete (*iter);
}

ShapeNode* Polygon::clone() const
{
	return (Polygon*)(vertices, color);
}

void Polygon::draw() const
{
	setColor(color);
	drawPolygon(vertices);
}

