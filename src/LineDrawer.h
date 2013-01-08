//
//  LineDrawer.h
//  SmoothLines
//
//  Created by Kamen Dimitrov on 1/8/13.
//
//

#ifndef SmoothLines_LineDrawer_h
#define SmoothLines_LineDrawer_h

/*
 Draw Smooth Lines
 
 http://www.merowing.info/2012/04/drawing-smooth-lines-with-cocos2d-ios-inspired-by-paper/
*/

#include "ofMain.h"

struct LinePoints {
	ofVec2f point;
	float width;
	ofColor c;
};



class Line {
public:
	vector<LinePoints> points;
	vector<LinePoints> smoothPoints;
	
	
	void calcSmooth() {
		smoothPoints.clear();
		
		if(points.size() > 2) {
			for(unsigned int i=2;i < points.size();++i) {
				LinePoints prev2 = points[i - 2];
				LinePoints prev1 = points[i - 1];
				LinePoints cur = points[i];

				ofVec2f midPoint1 = (prev1.point + prev2.point)*0.5f;
				ofVec2f midPoint2 = (cur.point + prev1.point)*0.5f;
				
				int segDistance = 2;
				float distance = midPoint1.distance(midPoint2);
				int numberOfSegments = MIN(128, MAX(floorf(distance / segDistance),32));

				
				float t = 0.0f;
				float step = 1.0f / numberOfSegments;
				
				for(int j = 0;j < numberOfSegments; j++) {
					LinePoints lp;

					lp.point = (midPoint1*powf(1-t,2) + prev1.point*(2.0f*(1-t)*t)) + midPoint2*(t*t);
					lp.width = powf(1 - t, 2) * ((prev1.width + prev2.width) * 0.5f) + 2.0f * (1 - t) * t * prev1.width + t * t * ((cur.width + prev1.width) * 0.5f);
					lp.c = cur.c;
					smoothPoints.push_back(lp);
					
					t += step;
				}
				
				LinePoints finalPoint;
				
				finalPoint.point = midPoint2;
				finalPoint.width = (cur.width + prev1.width) * 0.5f;
				finalPoint.c = cur.c;
				smoothPoints.push_back(finalPoint);
				
				
			}
		}
		
	}
};

class LineDrawer {
public:
	vector<Line> drawings;
	bool bUseSmoothLines;
	
	
	
	ofMesh lines;
	
	int currentLine;
	
	LineDrawer() {
		bUseSmoothLines = false;
		lines.setMode(OF_PRIMITIVE_TRIANGLES);
	}
	
	void createLine(ofVec2f spos) {
		currentLine = drawings.size();
		Line l;
		drawings.push_back(l);
	}
	
	void addLine(ofVec2f _point, float _width, ofColor c) {
		
		LinePoints lp;
		lp.point = _point;
		lp.width = _width;
		lp.c = c;
		
		drawings[currentLine].points.push_back(lp);
	}
	
	void closeLine() {
	//	ofLog() << "Created Line with: " << drawings[currentLine].points.size() << " points!";
		currentLine = -1;
	}
	

	void addTriangle(ofVec2f a, ofVec2f b, ofVec2f c, float z, ofColor c1, ofColor c2, ofColor c3, ofMesh &m) {
		ofVec3f p1,p2,p3;
		
		p1 = a;
		p1.z = z;
		p2 = b;
		p2.z = z;
		p3 = c;
		p3.z = z;
		
		m.addVertex(p1);
		m.addVertex(p2);
		m.addVertex(p3);
		
		m.addColor(c1);
		m.addColor(c2);
		m.addColor(c3);
		
	}
	
	
	void drawSmoothLine(int i) {
		
		ofMesh lineMesh;
		lineMesh.setMode(OF_PRIMITIVE_TRIANGLES);
		
		if(drawings[i].smoothPoints.size() == 0 || i == currentLine)
			drawings[i].calcSmooth();
		
		if(drawings[i].smoothPoints.size() != 0) {		
			ofVec2f prevPoint = drawings[i].smoothPoints[0].point;
			ofVec2f prevC, prevD;
			ofVec2f prevG, prevI;
			float prevValue = drawings[i].smoothPoints[0].width;
			int nullValues =0;
			for(int j=1;j<drawings[i].smoothPoints.size();j++) {
				
				
				ofVec2f currentPoint = drawings[i].smoothPoints[j].point;
				float currentValue = drawings[i].smoothPoints[j].width;
				
				ofVec2f dir = currentPoint - prevPoint;
				if(dir.length() == 0) {
					nullValues++;
					continue;
				}
				ofVec2f perp = dir.getPerpendicular();
				
				perp.normalize();
				
				ofVec2f pA, pB, pC, pD;
				
				pA = prevPoint + (perp * prevValue* 0.5f);
				pB = prevPoint - (perp * prevValue * 0.5f);
				pC = currentPoint + (perp * currentValue * 0.5f);
				pD = currentPoint - (perp * currentValue * 0.5f);
				
				float alpha = ofMap(currentValue, 1,20,50,155);
				ofColor color = drawings[i].smoothPoints[j].c;
	
				
				if(j > 1) {
					pA = prevC;
					pB = prevD;
				} else if(j == 1) {
					// add circle
					ofVec2f circleDirection = prevPoint - currentPoint;
					fillHalfCircle(prevPoint, circleDirection, color, prevValue*0.5f, lineMesh);
				}
				
				

				
				addTriangle(pA, pB, pC, 0.0f, color, color, color, lineMesh);
				addTriangle(pB, pC, pD, 0.0f, color, color, color, lineMesh);
				
				
				
				// anti-alias
				
				float overdraw = 3.0;
				ofColor fadeColor = color;
				fadeColor.a = 0;
				
				ofVec2f pF = pA + perp * overdraw;
				ofVec2f pG = pC + perp * overdraw;
				ofVec2f pH = pB - perp * overdraw;
				ofVec2f pI = pD - perp * overdraw;
				
				if(j > 4) {
					pF = prevG;
					pH = prevI;
				}
				
				addTriangle(pF, pA, pG, 0.0f, fadeColor, color, fadeColor, lineMesh);
				addTriangle(pA, pG, pC, 0.0f, color, fadeColor, color, lineMesh);
				addTriangle(pB, pH, pD, 0.0f, color, fadeColor, color, lineMesh);
				addTriangle(pH, pD, pI, 0.0f, fadeColor, color, fadeColor, lineMesh);

				
				
				// line starts
				
				// line ends
				// draw half circle
				
				if(j == drawings[i].smoothPoints.size()-1) {
					ofVec2f circleDirection = currentPoint - prevPoint;
circleDirection.normalize();
				
					
					fillHalfCircle(currentPoint, circleDirection, color, currentValue*0.5f, lineMesh);
					
					
				}
				
				
				prevG = pG;
				prevI = pI;
				
				prevC = pC;
				prevD = pD;
				
				prevValue = currentValue;
				prevPoint = currentPoint;
			}

		}
		
		
		lineMesh.drawFaces();
	}
	
	void drawAll() {

		for(int i=0;i<drawings.size();i++) {
			drawSmoothLine(i);

		}	
	}
	
	
	void fillHalfCircle(ofVec2f center, ofVec2f direction, ofColor color, float radius, ofMesh &m) {
		int numSegments = 32; // circle resolution
		
		float anglePerSegement = (float)M_PI / (numSegments-1);
		ofVec2f perp = direction.perpendicular();
		float angle = acosf(perp.dot(ofVec2f(0,1)));
		float rightDot = perp.dot(ofVec2f(1,0));
		
		if(rightDot < 0.0f)
			angle *= -1;
		
		ofVec2f prevPoint = center;
		ofVec2f prevDir = ofVec2f(sinf(0), cosf(0));
		
		for(int i=0;i<numSegments;++i) {
			ofVec2f dir = ofVec2f(sin(angle), cos(angle));
			ofVec2f curPoint = ofVec2f(center.x + radius * dir.x, center.y + radius * dir.y);
			
			m.addVertex(center);
			m.addVertex(prevPoint);
			m.addVertex(curPoint);
			
			m.addColor(color);
			m.addColor(color);
			m.addColor(color);
			
			
			ofColor fadeColor = color;
			fadeColor.a = 0;
			
			// anti-aliasing
			float overdraw = 2.0;
			
			m.addVertex(prevPoint + prevDir*overdraw);
			m.addVertex(prevPoint);
			m.addVertex(curPoint + dir * overdraw);
			
			m.addColor(fadeColor);
			m.addColor(color);
			m.addColor(fadeColor);
			
			m.addVertex(prevPoint);
			m.addVertex(curPoint);
			m.addVertex(curPoint + dir*overdraw);
			
			m.addColor(color);
			m.addColor(color);
			m.addColor(fadeColor);

			prevPoint = curPoint;
			prevDir = dir;
			angle += anglePerSegement;
		}
	}

};


#endif
