#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(0);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);
	ofNoFill();

	this->radius = 250;

	this->font.loadFont("fonts/Kazesawa-Bold.ttf", 230, true, true, true);
	string word = "B";

	auto path_list = font.getStringAsPoints(word, true, false);
	ofColor color;
	for (auto path : path_list) {

		auto outline = path.getOutline();
		for (int outline_index = 0; outline_index < outline.size(); outline_index++) {

			outline[outline_index] = outline[outline_index].getResampledByCount(250);
			auto vertices = outline[outline_index].getVertices();
			for (int vertices_index = 0; vertices_index < vertices.size(); vertices_index++) {

				this->base_location_list.push_back(glm::vec2(
				    ofGetWindowWidth() * 0.5 + vertices[vertices_index].x - this->font.stringWidth(word) * 0.5, 
					ofGetWindowHeight() * 0.5 + vertices[vertices_index].y + this->font.stringHeight(word) * 0.5));
				color.setHsb((int)ofMap(vertices_index, 0, vertices.size(), 0, 255 * 3) % 255, 255, 255);
				this->color_list.push_back(color);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	this->log_list.clear();

	for (int i = 0; i < this->base_location_list.size(); i++) {

		vector<glm::vec2> log;
		log.push_back(this->base_location_list[i]);
		this->log_list.push_back(log);
	}

	int step = 1;
	for (int i = 0; i < this->log_list.size(); i++) {

		int k = 0;
		while (true) {

			auto base_deg = RAD_TO_DEG * atan2(this->log_list[i].front().y - ofGetHeight() * 0.5, this->log_list[i].front().x - ofGetWindowWidth() * 0.5);
			auto deg = ofMap(ofNoise(glm::vec3(this->log_list[i].back() * 0.02, ofGetFrameNum() * 0.008 + k * 0.001)), 0, 1, base_deg - 90, base_deg + 90);
			auto next = this->log_list[i].back() + glm::vec2(step * cos(deg * DEG_TO_RAD), step * sin(deg * DEG_TO_RAD));

			if (glm::distance(glm::vec2(ofGetWindowWidth() * 0.5, ofGetWindowHeight() * 0.5), next) < this->radius) {

				this->log_list[i].push_back(next);
			}
			else {

				break;
			}

			k++;
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	for (int i = 0; i < this->log_list.size(); i++) {

		ofSetColor(this->color_list[i]);

		ofBeginShape();
		ofVertices(this->log_list[i]);
		ofEndShape();
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}