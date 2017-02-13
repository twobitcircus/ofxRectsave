#include "ofxRectsave.h"
#include "ofxXmlSettings.h"


void ofxRectsave::setup(string _filename, ofxImGui::Gui *ui) {
  filename = _filename;
  if (ui == NULL) {
    ui = new ofxImGui::Gui();
    own_ui = true;
  }
  active = false;
  ofAddListener(ofEvents().draw, this, &ofxRectsave::draw, OF_EVENT_ORDER_AFTER_APP);  
  ofAddListener(ofEvents().mousePressed, this, &ofxRectsave::mousePressed, OF_EVENT_ORDER_AFTER_APP);
  ofAddListener(ofEvents().mouseReleased, this, &ofxRectsave::mouseReleased, OF_EVENT_ORDER_AFTER_APP);
  ofAddListener(ofEvents().mouseDragged, this, &ofxRectsave::mouseDragged, OF_EVENT_ORDER_AFTER_APP);  

  setBounds(ofRectangle(0, 0, ofGetWidth(), ofGetHeight()));

  load();

  move_from = RS_NONE;
  if (ui == NULL) {
    ui = new ofxImGui::Gui();
    ui->setup();
    own_ui = true;
  }
}

void ofxRectsave::drawGui() {
  ImGui::SetNextWindowSize(ImVec2(0,0), ImGuiSetCond_FirstUseEver);
  ImGui::Begin("Rectsave");
  if (ImGui::Button("save")) save();
  ImGui::SameLine();
  if (ImGui::Button("reload")) load();
  if (ImGui::Button("new")) createRect();

  vector<string> names = getNames();
  if (names.size() > 0) {
    const char *c_names[names.size()];
    for (int i=0; i<names.size(); i++) {
      c_names[i] = names[i].c_str();
    }
    ImGui::ListBox("rects", &cur_idx, c_names, names.size(), names.size());
    ImGui::Separator();
    if (cur_idx != -1) {
      string cur_name = rects[cur_idx].first;
      ofRectangle cur_rect = rects[cur_idx].second;

      char cur_name_c[64];
      snprintf(cur_name_c, 64, "%s", cur_name.c_str());
      ImGui::InputText("name", cur_name_c, 64);
      rects[cur_idx].first = cur_name_c;

      float top = float(cur_rect.getMinY());
      ImGui::InputFloat("top", &top);
      float left = float(cur_rect.getMinX());
      ImGui::InputFloat("left", &left);
      float bottom = float(cur_rect.getMaxY());
      ImGui::InputFloat("bottom", &bottom);
      float right = float(cur_rect.getMaxX());
      ImGui::InputFloat("right", &right);

      rects[cur_idx].second = ofRectangle(ofPoint(left, top), ofPoint(right, bottom));

      ImGui::LabelText("center", "%d, %d", int(cur_rect.getCenter().x), int(cur_rect.getCenter().y));
      if (ImGui::Button("delete")) {
        rects.erase(rects.begin() + cur_idx);
        if (rects.size() == 0) cur_idx = -1;
        else cur_idx = (cur_idx+1) % rects.size();
      }
    }
  }
  ImGui::End();
}

void ofxRectsave::createRect() {
  stringstream ss;
  ss << "unnamed" << int(ofRandom(9999));
  rects.push_back(pair<string, ofRectangle>(ss.str(), ofRectangle(0, 0, 1, 1)));
  cur_idx = rects.size()-1;
}

void ofxRectsave::draw(ofEventArgs &eventArgs) {
  if (!active) return;

  if (own_ui) {
    ui->begin();
    drawGui();
    ui->end();
  }

  if (cur_idx == -1) return;

  string cur_name = rects[cur_idx].first;
  ofRectangle cur_input_rect = getRectangleByName(cur_name, input_bounds);
  ofRectangle cur_output_rect = getRectangleByName(cur_name, output_bounds);

  ofPushStyle();
  ofPushMatrix();


  ofSetColor(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255));
  ofNoFill(); 
  ofDrawRectangle(cur_input_rect);

  ofFill();
  ofDrawCircle(cur_input_rect.getTopLeft(), 3);
  ofDrawCircle(cur_input_rect.getTopRight(), 3);
  ofDrawCircle(cur_input_rect.getBottomRight(), 3);
  ofDrawCircle(cur_input_rect.getBottomLeft(), 3);
  ofDrawCircle(cur_input_rect.getCenter(), 3);
  ofPopMatrix();
  ofPopStyle();
}

void ofxRectsave::mousePressed(ofMouseEventArgs &eventArgs) {
  if (!active) return;
  if (cur_idx == -1) return;

  string cur_name = rects[cur_idx].first;
  ofRectangle cur_input_rect = getRectangleByName(cur_name, input_bounds);

  ofVec2f down_at(eventArgs.x, eventArgs.y);
  //down_at.x = ofMap(down_at.x, input_bounds.getMinX(), input_bounds.getMaxX(), 0, 1);
  //down_at.y = ofMap(down_at.y, input_bounds.getMinY(), input_bounds.getMaxY(), 0, 1);

  vector<float> distances {
    down_at.distance(cur_input_rect.getTopLeft()),
    down_at.distance(cur_input_rect.getTopRight()),
    down_at.distance(cur_input_rect.getBottomRight()),
    down_at.distance(cur_input_rect.getBottomLeft()),
    down_at.distance(cur_input_rect.getCenter())
  };

  int move_from_idx = min_element(distances.begin(), distances.end()) - distances.begin();
  if (distances[move_from_idx] < 5)
    move_from = (rs_move_from_t)move_from_idx;
}

void ofxRectsave::mouseReleased(ofMouseEventArgs &eventArgs) {
  move_from = RS_NONE;
}

void ofxRectsave::mouseDragged(ofMouseEventArgs &eventArgs) {
  if (!active) return;
  if (cur_idx == -1) return;

  string cur_name = rects[cur_idx].first;
  ofRectangle cur_input_rect = getRectangleByName(cur_name, input_bounds);
  ofRectangle next_input_rect;

  ofPoint drag_at(eventArgs.x, eventArgs.y);
  //drag_at.x = ofMap(drag_at.x, input_bounds.getMinX(), input_bounds.getMaxX(), 0, 1);
  //drag_at.y = ofMap(drag_at.y, input_bounds.getMinY(), input_bounds.getMaxY(), 0, 1);
  switch (move_from) {
    case RS_TOP_LEFT:
      next_input_rect.set(drag_at, cur_input_rect.getBottomRight());
      break;
    case RS_TOP_RIGHT:
      next_input_rect.set(
        ofVec2f(cur_input_rect.getMinX(), drag_at.y),
        ofVec2f(drag_at.x, cur_input_rect.getMaxY())
      );
      break;
    case RS_BOTTOM_RIGHT:
      next_input_rect.set(cur_input_rect.getTopLeft(), drag_at);
      break;
    case RS_BOTTOM_LEFT:
      next_input_rect.set(
        ofVec2f(drag_at.x, cur_input_rect.getMinY()),
        ofVec2f(cur_input_rect.getMaxX(), drag_at.y)
      );
      break;
    case RS_CENTER:
      next_input_rect.setFromCenter(drag_at, cur_input_rect.getWidth(), cur_input_rect.getHeight());
      break;
    case RS_NONE:
      next_input_rect = cur_input_rect;
  }
  rects[cur_idx].second = ofRectangle(
      ofMap(next_input_rect.getMinX(), input_bounds.getMinX(), input_bounds.getMaxX(), 0, 1),
      ofMap(next_input_rect.getMinY(), input_bounds.getMinY(), input_bounds.getMaxY(), 0, 1),
      ofMap(next_input_rect.getWidth(), 0, input_bounds.getWidth(), 0, 1),
      ofMap(next_input_rect.getHeight(), 0, input_bounds.getHeight(), 0, 1)
  );
}

void ofxRectsave::load() {
  ofxXmlSettings xml;
  xml.loadFile(filename);
  rects.clear();
  for (int i=0; i<xml.getNumTags("rect"); i++) {
    xml.pushTag("rect", i);
    string name = xml.getValue("name", "unknown");
    float top = xml.getValue("top", 0.0);
    float left = xml.getValue("left", 0.0);
    float bottom = xml.getValue("bottom", 1.0);
    float right = xml.getValue("right", 1.0);
    rects.push_back(pair<string, ofRectangle>(
      name,
      ofRectangle(ofPoint(left, top), ofPoint(right, bottom))
    ));
    xml.popTag();
  }
}

bool rect_comp(pair<string, ofRectangle> &a, pair<string, ofRectangle> &b) {
  return a.first < b.first;
}

void ofxRectsave::save() {
  sort(rects.begin(), rects.end(), rect_comp);
  ofxXmlSettings xml;
  for (int i=0; i<rects.size(); i++) {
    string cur_name = rects[i].first;
    ofRectangle cur_rect = rects[i].second;
    
    xml.addTag("rect");
    xml.pushTag("rect", i);
    xml.setValue("name", cur_name);
    xml.setValue("top", cur_rect.getMinY()); 
    xml.setValue("left", cur_rect.getMinX()); 
    xml.setValue("bottom", cur_rect.getMaxY()); 
    xml.setValue("right", cur_rect.getMaxX()); 
    xml.popTag();
  }
  xml.save(filename);
}


ofRectangle ofxRectsave::getRectangleByNameNormalized(string name) {
  for (auto &p : rects) {
    if (p.first == name)
      return p.second;
  }
  return ofRectangle(0.25, 0.25,0.75, 0.75);
}

ofRectangle ofxRectsave::getRectangleByName(string name) {
  return getRectangleByName(name, output_bounds);
}


ofRectangle ofxRectsave::getRectangleByName(string name, ofRectangle scaled_to_rect) {
  ofRectangle rect = getRectangleByNameNormalized(name);
  rect.set(
      ofMap(rect.getMinX(), 0, 1, scaled_to_rect.getMinX(), scaled_to_rect.getMaxX()),
      ofMap(rect.getMinY(), 0, 1, scaled_to_rect.getMinY(), scaled_to_rect.getMaxY()),
      ofMap(rect.getWidth(), 0, 1, 0, scaled_to_rect.getWidth()),
      ofMap(rect.getHeight(), 0, 1, 0, scaled_to_rect.getHeight())
  );
  return rect;
}


vector<string> ofxRectsave::getNames() {
  vector<string> names;
  for (auto &p : rects) {
    names.push_back(p.first);
  }
  return names;
}
