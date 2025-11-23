#include <iostream>
#include <thread>
#include <functional>
#include <chrono>
#include <mutex>
#include <filesystem>
#include <algorithm>
#include <boost/gil.hpp>
#include <boost/gil/extension/io/jpeg.hpp>
#include <boost/gil/extension/io/png.hpp>
#include "clock.h"
#include "workers.h"

std::mutex m;

namespace fs = std::filesystem;
namespace gil = boost::gil;

std::vector<std::string> get_files(std::string_view input_folder) {
	std::vector<std::string> v;
	for (const auto& entry : fs::directory_iterator(input_folder)) {
        if (entry.is_regular_file()) {
            v.push_back(entry.path());
		}
	}	
	return v;
}


void process_image(const std::string& input, std::string target_folder) {
    fs::path input_path(input);
    fs::path output_path = fs::path(target_folder) / input_path.filename();
    gil::rgb8_image_t img;
    gil::read_image(input, img, gil::jpeg_tag{});  

    // conversion en grayscale
    gil::gray8_image_t gray(img.dimensions());
    gil::copy_and_convert_pixels(view(img), view(gray));

    gil::write_view(output_path.string(), view(gray), gil::png_tag{}); 
}


void producer(Workers& w, std::string_view input_folder, std::string_view target_folder) {
	// This thread will read all images from a given folder
	for (auto& file : get_files(input_folder)) {
		std::cout << file << std::endl;
		auto t= [file, target_folder](){
			process_image(file, std::string(target_folder));
		};
		w.add_task(std::move(t));
	}
}

void single_thread(std::string_view input_folder, std::string_view target_folder) {
	// This thread will read all images from a given folder
	for (auto& file : get_files(input_folder)) {
		std::cout << file << std::endl;
		process_image(file, std::string(target_folder));
	}
}



int main(int argc, char** argv) {
	if (argc != 3) {
		std::cerr << "Usage: ./img_process input_folder output_folder" << std::endl;
		exit(1);
	}
	{
		Clock ck;
		const uint32_t N_THREADS = std::max(2u, std::thread::hardware_concurrency());
    	std::cout << "Avalaible threads : " << N_THREADS << std::endl;

		Workers w {N_THREADS};
	
		producer(w, argv[1], argv[2]);
		std::cout <<"Multithreading => ";
	}
	{
		Clock ck;
		single_thread(argv[1], argv[2]);
		std::cout << "Single thread => ";
	}
	return 0;
}


