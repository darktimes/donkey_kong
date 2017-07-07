#include <iostream>
#include <stdlib.h>
#include <SOIL/SOIL.h>
#include <memory>
#include <string.h>
#include <fstream>
#include <vector>
#include "image.hpp"
#include "color.hpp"

using namespace std;
using namespace ImageProcessing;

void appenedPrefix(string& fileName) {
	fileName =  "output/t-" + fileName;
}

int main(int argc, char** argv) {
	std::vector<string> files;
	for (auto i = 1; i < argc; i++) {
		files.push_back(string(argv[i]));
	}	
	ofstream result;

	while(files.size() != 0) {
		string current_file = files.back();
		shared_ptr<Image> image(new Image(current_file.c_str()));
		appenedPrefix(current_file);
		remove(current_file.c_str());
		result.open(current_file.c_str());
		cout<<"loaded file: "<<current_file<<endl;
		
		result<<image->getWidth()<<";"<<image->getHeight()<<";";
		
		for (auto i = 0; i < image->getWidth() * image->getHeight(); i++) {
			Color cc = image->getColor(i);
			result<<cc.red<<","<<cc.green<<","<<cc.blue<<",";
		}
		result<<endl;
		cout<<"processed file: "<<current_file<<endl;
		files.pop_back();
		result.close();
	}	

	result.close();
	return 0;
}
