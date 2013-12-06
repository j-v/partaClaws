#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(0);
	ofSetLogLevel( OF_LOG_VERBOSE );
	ofSetVerticalSync( true );
	ofHideCursor();

	// Initialize particles
	int num_particles = 120;
	initParticles(num_particles);
	

	cam.setFarClip(10000);
	//cam.setNearClip(0.001);
	cam.begin();
	cam.end();

	useOculus = false; // change me
	oculus.baseCamera = &cam;

	if (useOculus)
	{	
		oculus.setup();
	}

	gui.setup("panel");
	gui.add(particleSize.set("particleSize", 20, 0, 50));
	gui.add(maxV.set("maxV", 10, 0, 100));
	gui.add(attractiveForce.set("attractiveForce", 0.0001, 0, 0.001));
	gui.add(repulsiveForce.set("repulsiveForce", 0.0005, 0, 0.005));
	gui.add(thresholdCenter.set("thresholdCenter", 100, 0, 1000));
	gui.add(interRepulsiveForce.set("interRepulsiveForce", 0.03, 0, 0.50));
	gui.add(thresholdInterParticleSquared.set("thresholdInterParticleSquared", 100, 0, 100000));
	gui.add(numParticles.set("numParticles", num_particles, 0, 1000));
	gui.add(bgColor1.set("bg2",ofColor::black,ofColor::black, ofColor::white));
	gui.add(bgColor2.set("bg1",ofColor::aqua,ofColor::black, ofColor::white));
	gui.add(particleColor.set("particle color", ofColor(255,255,255),ofColor::black, ofColor::white));
	gui.add(ambientLightColor.set("ambient",ofColor::gold,ofColor::black, ofColor::white));
	
	
	// TODO make spotlight work
	spotLight.setSpotlight();
	spotLight.setGlobalPosition(0,0,0);
	spotLight.setOrientation(ofVec3f(1,1,1));
	spotLight.setSpecularColor(ofFloatColor::blueViolet);
	//spotLight.set

	if (useOculus)
		ofHideCursor();
	else
		ofShowCursor();

	cam.rotateAround(360, ofVec3f(0,0,1), ofVec3f(0,0,0));
}

void testApp::initParticles(int numParticles)
{
	float radius = 100;
	particles.clear();
	for (int i=0; i < numParticles; i++)
	{
		particles.push_back(Particle(ofVec3f(ofRandomf()*radius, ofRandomf()*radius, ofRandomf()*radius), ofVec3f::zero()));
	}
}

//--------------------------------------------------------------
void testApp::update(){
	light.setAmbientColor(ambientLightColor.get());

	vector<Particle>::iterator it;
	ofVec3f cent = ofVec3f::zero();
	for (it = particles.begin(); it != particles.end(); ++it)
	{
		// attract/repulse to center
		ofVec3f pos_diff = (it->p - cent);
		float d_cent = pos_diff.length();
		if (d_cent > thresholdCenter)
		{
			// attract
			it->a -= pos_diff.normalized() * ((d_cent - thresholdCenter) * attractiveForce);
		}
		else
		{
			it->a += pos_diff.normalized() * ((thresholdCenter - d_cent) * repulsiveForce) ;
		}

		// repulse from each other
		vector<Particle>::iterator it1 = it;
		for (++it1; it1 != particles.end(); ++it1)
		{
			ofVec3f dir = it1->p - it->p;
			float dSquared = dir.lengthSquared();
			if (dSquared < thresholdInterParticleSquared)
			{
				float F = (thresholdInterParticleSquared/dSquared -1.0f) * interRepulsiveForce;
				dir = dir.normalized() * F;
				it->a -= dir;
				it1->a += dir;
			}
		}

		it->v += it->a;
		it->vNorm = it->v.normalized();
		// limit speed
		if (it->v.lengthSquared() > maxV * maxV)
		{
			it->v = it->vNorm * maxV;
		}

		it->p += it->v;
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	if (useOculus)
	{
		glEnable(GL_DEPTH_TEST);

		oculus.beginBackground();
		ofBackgroundGradient(bgColor1, bgColor2);
		oculus.endBackground();

		oculus.beginOverlay();
		
		oculus.endOverlay();

		oculus.beginLeftEye();
		drawScene();
		oculus.endLeftEye();
		
		oculus.beginRightEye();
		drawScene();
		oculus.endRightEye();

		
		oculus.draw();
	}
	else
	{
		ofBackgroundGradient(bgColor1, bgColor2);
		cam.begin();
		
		drawScene();
		cam.end();
		gui.draw();
	}
}

void testApp::drawScene()
{

		//cam.begin();
	//ofBackgroundGradient(ofColor::azure, ofColor::aliceBlue);


	ofPushStyle();
	light.enable();
	spotLight.enable();
	for (int i=0; i < particles.size(); i++)
	{
		
		//ofSetColor(255,255,255,127);
		ofSetColor(255,255,255,255);
		//ofPushMatrix();
		//ofTranslate(particles[i]);
		//
		//ofSphere(particleSize);
		//
		//ofPopMatrix();

		ofDrawSphere(particles[i].p, particleSize);
		//ofDrawArrow(particles[i].p, particles[i].p + particles[i].vNorm * particleSize, particleSize);
	}
	//cam.end();
	light.disable();
	spotLight.disable();
	ofSetColor(255,255,255,255);
	//billboard and draw the mouse
	if(useOculus && oculus.isSetup()){
		
		ofPushMatrix();
		ofSetColor(255, 0, 0);
		oculus.multBillboardMatrix();
		ofCircle(0,0,.5);
		ofPopMatrix();
	
	}
	
	ofPopStyle();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
	if ((char)key == 'o')
	{
		useOculus = !useOculus;
		if (useOculus)
		{

			ofHideCursor();
			if (!oculus.isSetup())
			{
			    bool res = oculus.setup();
				// TODO check res
			}
		}
		else
			ofShowCursor();
	}
	else if ((char)key == 'r')
	{
		initParticles(numParticles);
	}
	if(key == 's') {
		gui.saveToFile("settings.xml");
	}
	if(key == 'l') {
		gui.loadFromFile("settings.xml");
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
