#include "ofApp.h"
#include "Forces/ParticleForceRegistry.h"
#include "Collisions/CollisionHandler.h"
#include "Forces/ParticleDeplacement.cpp"
#include "cmath"

//--------------------------------------------------------------
void ofApp::setup() {
	// Tests
	Tests::ExecuteTests();

	// Set command text
	commandText = "Bouger le blob avec les fleches directionnelles de droite et de gauche!";

	// Setup lists
	primitives = std::list<std::pair<of3dPrimitive*, int*>>(); // display primitive on each draw()

	// Setup cam and basic primitives
	const float floorWidth = ofGetScreenWidth() * 100;
	const float floorHeight = 5;
	floor = ofBoxPrimitive(floorWidth, floorHeight, 0);
	floor.setPosition(Vector3D(0, -10).v3());

	// Colors & Text

	// Setup colors
	colors[0] = Vector3D(0, 0, 0);
	colors[1] = Vector3D(255, 255, 255);
	colors[2] = Vector3D(125, 125, 125);

	// Setup cam variables
	cameraPosition = Vector3D(0, 0, 500);
	float fovRad = cam.getFov() * PI / 180;
	// Pythagoras to get displayed width with fov and z of camera 	
	viewWidth = tan(fovRad / 2) * 2 * cameraPosition.z();

	// Setup Physics
	forceRegistry = new ParticleForceRegistry();
	collisionHandler = new CollisionHandler();

	m_gravity = Vector3D(0, -9.8, 0);
	gravity = new ParticleGravity(Vector3D(0, -40));

	// setup blob
	Particle* blobCore = new Particle(10, Vector3D(0, 100), Vector3D(), .01);
	blob = Blob(blobCore);

	int i = 0;
	int* colorMode;
	for (Particle* particle : blob.m_particles) {
		colorMode = new int(1);
		if (i == 0) {
			colorMode = new int(0);
			i++;
		}

		primitives.push_back(std::pair<of3dPrimitive*, int*>(particle, colorMode));
		particles.push_back(particle);
	}

	// add floor
	generateFloor(getLayout());
}

//--------------------------------------------------------------
void ofApp::update() {
	fps = ofGetFrameRate();

	float duration = fps == 0 ? 0 : 1/fps;

	updateForces();
	float deltaX = blob.getCore()->getPosition().x();
	//Update particles
	for (Particle* particle : particles) {
		particle->Update();
	}

	collisionHandler->handleCollision(particles, duration, forceRegistry);

	deltaX = blob.getCore()->getPosition().x() - deltaX;
	Vector3D deltaPosCamBlob = Vector3D(cam.getPosition()) - blob.getCore()->getPosition();
	if (abs(deltaPosCamBlob.x()) >= viewWidth / 3) {
		cameraPosition += Vector3D(deltaX);
	}
	cam.setPosition(cameraPosition.v3());

}

//--------------------------------------------------------------
void ofApp::draw() {
	// begin camera job
	cam.begin();

	// display command text
	ofSetColor(255, 255, 255);
	ofDrawBitmapString(ofToString(commandText), -viewWidth / 2, viewWidth / 4);

	int* colorMode;

	// display primitives with correct color
	for (std::pair<of3dPrimitive*, int*> primitive : primitives)
	{
		colorMode = primitive.second;
		ofSetColor(colors[(*colorMode)].x(), colors[(*colorMode)].y(), colors[(*colorMode)].z());

		// display primitive
		primitive.first->draw();

	}

	// end camera job
	cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	// Default direction +x
	float deplacementNorm = 50;

	// move on arrow key press
	switch (key)
	{
	case 57358: // stride right
		break;
	case 57356: // stride left
		deplacementNorm *= -1;
		break;
	default:
		return;
	}

	ParticleDeplacement* movementX = new ParticleDeplacement(deplacementNorm);
	forceRegistry->add(blob.getCore(), movementX);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	// keep coherent display when resizing viewport
	cam.setPosition(Vector3D(0, 0, 1500).v3());
	cam.move(Vector3D(ofGetWidth() * .5, ofGetHeight() * .5).v3());
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

//--------------------------------------------------------------
void ofApp::updateForces() {
	float duration = ofGetFrameRate() == 0. ? 0. : 1 / ofGetFrameRate();

	blob.linkParticles(forceRegistry, collisionHandler);


	for (Particle* particle : particles)
	{
		forceRegistry->add(particle, gravity);
	}


	forceRegistry->updateForces(duration);
}

//--------------------------------------------------------------
void ofApp::generateFloor(std::list<std::pair<int*, Vector3D*>> layout) {

	Particle* floorParticle;
	int* floorMode = new int(2);

	for (std::pair<int*, Vector3D*> particlePair : layout) {
		floorParticle = new Particle((*particlePair.first), (*particlePair.second), Vector3D(), 0.);
		primitives.push_back(std::pair<of3dPrimitive*, int*>(floorParticle, floorMode));
		particles.push_back(floorParticle);
	}
}

std::list<std::pair<int*, Vector3D*>> ofApp::getLayout() {
	return std::list<std::pair<int*, Vector3D*>> (
		{ 
			std::pair<int*, Vector3D*>(new int(200), new Vector3D(0, -250)),
			std::pair<int*, Vector3D*>(new int(200), new Vector3D(200, -150)),
			std::pair<int*, Vector3D*>(new int(200), new Vector3D(400, -170)),
			std::pair<int*, Vector3D*>(new int(200), new Vector3D(-150, -250)),
			std::pair<int*, Vector3D*>(new int(300), new Vector3D(-500, -250)),
			std::pair<int*, Vector3D*>(new int(300), new Vector3D(500, -350)),
		}
	);
}