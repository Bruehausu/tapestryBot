#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    csvMatch.setString(0,0,"id");
    csvMatch.setString(0,0,"match 1");
    csvMatch.setString(0,0,"match 2");
    csvMatch.setString(0,0,"match 3");
    
    int index = 1;
    int data_size = 100000;
    
    vector<int> self_loops;
    
    csvMatch.saveFile( ofToDataPath( "output.csv" ));
    
    ofPixels p;
    //ofLoadImage(p, "flickr/00000/0000001.jpg");
    ofLoadImage(p, "thisdoesntexist.jpg");
    
    cout << "loaded image 1. width: " << p.getWidth() << endl;
    
    map< int, vector<int>> top_colors;
    map< int, vector<int>> bot_colors;
    
    for(int i = 0; i < data_size; i++){
        
        if(i % 1000 == 0){
            cout << "processed " << i << " images ids" << endl;
        }
        
        ofLoadImage(p, constructPath(i));
        if (p.getWidth() == 500){
            vector<int> top;
            vector<int> bot;
            int bot_ind = p.getHeight() - 1;
            
            for(int j = 0; j < 500; j++){
                top.push_back(p.getColor(j, 0).getHex());
                bot.push_back(p.getColor(j, bot_ind).getHex());
            }
            
            if (getDiff(top, bot) > 8000){
                top_colors[i] = top;
                bot_colors[i] = bot;
            }
        }
    }
    
    cout << "finished condensing color strings" << endl;
    
    for(int i = 0; i < data_size; i++){
        
        if(i % 1000 == 0){
            cout << "matched " << i << " image ids" << endl;
        }
        
        if (top_colors.count(i)){
            int best_found = i;
            int best_diff = 100000000;
            
            int second_found = i;
            int second_diff = 100000000;
            
            int third_found = i;
            int third_diff = 100000000;
            
            for(int j = 0; j < data_size; j++){
                if (bot_colors.count(j)){
                    int diff = getDiff(top_colors[i], bot_colors[j]);
                    if (diff < best_diff){
                        third_found = second_found;
                        third_diff = second_diff;
                        second_found = best_found;
                        second_diff = best_diff;
                        best_found = j;
                        best_diff = diff;
                    } else if (diff < second_diff){
                        third_found = second_found;
                        third_diff = second_diff;
                        second_found = j;
                        second_diff = diff;
                    } else if (diff < third_diff){
                        third_found = j;
                        third_diff = diff;
                    }
                }
            }
            
            if( i != best_found){
                csvMatch.setInt(index, 0, i);
                csvMatch.setInt(index, 1, best_found);
                csvMatch.setInt(index, 2, second_found);
                csvMatch.setInt(index, 3, third_found);
                index += 1;
            } else {
                cout << "image " << i << "is a loop" << endl;
                self_loops.push_back(i);
            }
        }
    }
    
    cout << "removing unuseable images. " << csvMatch.numRows << "csv entries" << endl;
    
    for(int i = 1; i < csvMatch.numRows; i++){
        if(i % 1000 == 0){
            cout << "checked " << i << " image ids" << endl;
        }
        
        for(int j = 1; j < 4; j++){
            int curr = csvMatch.getInt(i, j);
            for(int k = 0; k < self_loops.size(); k++){
                if (self_loops[k] == curr){
                    csvMatch.setInt(i, j, 0);
                    break;
                }
            }
        }
    }
    
    csvMatch.saveFile(ofToDataPath("output.csv"));

}

//--------------------------------------------------------------
string ofApp::constructPath(int i){
    string folder = ofToString(i/1000);
    while(folder.length() < 5){
        folder = '0'+ folder;
    }
    string file = ofToString(i);
    while(file.length() < 7){
        file = '0'+ file;
    }
    
    return ("flickr/" + folder + "/" + file + ".jpg");
}

//--------------------------------------------------------------
int ofApp::getDiff(vector<int> a, vector<int> b){
    int sum = 0;
    ofColor a_col;
    ofColor b_col;
    for(int i = 0; i < 500; i++){
        a_col = ofColor::fromHex(a[i]);
        b_col = ofColor::fromHex(b[i]);
        sum += ( ((a_col.r-b_col.r) * (a_col.r-b_col.r)) +
                 ((a_col.g-b_col.g) * (a_col.g-b_col.g)) +
                 ((a_col.b-b_col.b) * (a_col.b-b_col.b)) );
    }
    return sum;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
