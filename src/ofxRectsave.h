#pragma once

#include "ofMain.h"
#include "ofxImGui.h"

typedef enum {RS_TOP_LEFT, RS_TOP_RIGHT, RS_BOTTOM_RIGHT, RS_BOTTOM_LEFT, RS_CENTER, RS_NONE} rs_move_from_t;

class ofxRectsave {
  public:
    void setup(string _filename, ofxImGui::Gui *ui);
    void setBounds(ofRectangle rect) {
      setInputBounds(rect);
      setOutputBounds(rect);
    }
    void setInputBounds(ofRectangle rect) {
      input_bounds = rect;
    }
    void setOutputBounds(ofRectangle rect) {
      output_bounds = rect;
    }
    void draw(ofEventArgs &eventArgs);
    void drawGui();
    void mousePressed(ofMouseEventArgs &eventArgs);
    void mouseReleased(ofMouseEventArgs &eventArgs);
    void mouseDragged(ofMouseEventArgs &eventArgs);
    void load();
    void save();
    void createRect();

    void toggleActive() {
      active = !active;
    }

    void setActive(bool _active) {
      active = _active;
    }

    bool getActive() {
      return active;
    }

    ofRectangle getRectangleByNameNormalized(string name);
    ofRectangle getRectangleByName(string name);
    ofRectangle getRectangleByName(string name, ofRectangle scaled_to_rect);

    vector<string> getNames();

    rs_move_from_t move_from; // which corner is being moved


  private:
    string filename;

    int cur_idx = -1;
    bool active;
    ofRectangle input_bounds;
    ofRectangle output_bounds;

    ofxImGui::Gui *ui;
    bool own_ui = false; // are we responsible for setting up this UI?
    vector<pair<string, ofRectangle> > rects;
};
