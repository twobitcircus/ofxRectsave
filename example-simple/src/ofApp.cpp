#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  rectsave.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
  ofSetFrameRate(30);
  ofSetVerticalSync(true);

}

//--------------------------------------------------------------
void ofApp::draw(){
  ofBackground(0);

  ofRectangle bounds(60, 90, ofGetWidth()/2, ofGetHeight()/2);
  rectsave.setBounds(bounds);

  ofPushStyle();
  ofSetColor(ofColor::red);
  ofNoFill();
  ofDrawRectangle(bounds);
  ofPopStyle();

  ofPushMatrix();
  ofTranslate(bounds.getTopLeft());
  ofScale(bounds.getWidth()/ofGetWidth(), bounds.getHeight()/ofGetHeight());
  ofSetColor(ofColor::red);
  ofDrawLine(0,0, ofGetWidth(), ofGetHeight());
  ofDrawLine(ofGetWidth(), 0, 0, ofGetHeight());

  ofRectangle r = rectsave.getRectangleByName("test");
  ofPolyline p;
  for (int x=0; x<r.width; x++) {
    int y = r.height*ofNoise(ofGetElapsedTimef(), x/100.0);
    p.addVertex(x,y);
  }
  ofPushMatrix();
  ofSetColor(255);
  ofTranslate(r.getTopLeft());
  p.draw();
  ofPopMatrix();

  ofPopMatrix();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  switch (key) {
    case '|':
      rectsave.toggleActive();
      break;
  }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
