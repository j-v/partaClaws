#pragma once

#include "ofMain.h"
#include "ofxOculusRift.h"
#include "ofxGui.h"

#include <vector>

using namespace std;

class Particle
{
public:
	Particle(ofVec3f pP, ofVec3f pV)
	{
		p = pP;
		v = pV;
		a = ofVec3f::zero();

		vNorm = v.normalized();
	}

	ofVec3f p;
	ofVec3f v;
	ofVec3f a;
	ofVec3f vNorm;

};

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
private:

	void initParticles(int numParticles);

	vector<Particle> particles;
	ofEasyCam			cam;
	ofxOculusRift	oculus;

	void drawScene();

	ofParameter<float> particleSize;
	ofParameter<float> maxV;
	ofParameter<float> attractiveForce;
	ofParameter<float> repulsiveForce;
	ofParameter<float> thresholdCenter;
	ofParameter<float> interRepulsiveForce;
	ofParameter<float> thresholdInterParticleSquared;
	ofParameter<int> numParticles;
	ofParameter<ofColor> bgColor1;
	ofParameter<ofColor> bgColor2;
	ofParameter<ofColor> particleColor;
	ofParameter<ofColor> ambientLightColor;
	ofxPanel gui;

	ofLight light;
	ofLight spotLight;

	bool useOculus;
};
